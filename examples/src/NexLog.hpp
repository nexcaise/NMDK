#pragma once

// File logger untuk mod NexCaise.
//
// Menulis ke:
//   /storage/emulated/0/Download/NexCaise/logs/NmdkExamples.log
// Kalau folder itu tidak bisa ditulis (izin storage ditolak), otomatis jatuh ke:
//   /storage/emulated/0/Android/data/<package>/files/NexCaise/logs/NmdkExamples.log
// Setiap baris juga tetap dikirim ke logcat.

#include <android/log.h>

#include <fmt/format.h>

#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <exception>
#include <filesystem>
#include <mutex>
#include <string>
#include <string_view>

#include <sys/stat.h>
#include <unistd.h>

namespace nexlog {

enum class Level { Debug, Info, Warn, Error, Fatal };

inline constexpr const char *kTag = "NmdkExamples";
inline constexpr const char *kPrimaryDir =
    "/storage/emulated/0/Download/NexCaise/logs";
inline constexpr const char *kFileName = "NmdkExamples.log";
inline constexpr const char *kOldFileName = "NmdkExamples.old.log";
inline constexpr long kMaxFileBytes = 2L * 1024 * 1024;

namespace detail {

struct State {
  std::mutex mutex;
  FILE *file = nullptr;
  bool attempted = false;
  std::string path;
};

inline State &state() {
  static State s;
  return s;
}

inline int androidPriority(Level level) {
  switch (level) {
  case Level::Debug: return ANDROID_LOG_DEBUG;
  case Level::Info:  return ANDROID_LOG_INFO;
  case Level::Warn:  return ANDROID_LOG_WARN;
  case Level::Error: return ANDROID_LOG_ERROR;
  case Level::Fatal: return ANDROID_LOG_FATAL;
  }
  return ANDROID_LOG_INFO;
}

inline char levelChar(Level level) {
  switch (level) {
  case Level::Debug: return 'D';
  case Level::Info:  return 'I';
  case Level::Warn:  return 'W';
  case Level::Error: return 'E';
  case Level::Fatal: return 'F';
  }
  return '?';
}

// Nama package dari /proc/self/cmdline (buang suffix ":process" kalau ada).
inline std::string packageName() {
  FILE *f = std::fopen("/proc/self/cmdline", "r");
  if (!f) {
    return "com.mojang.minecraftpe";
  }
  char buf[256] = {};
  std::fgets(buf, sizeof(buf), f);
  std::fclose(f);
  std::string name(buf);
  if (const auto colon = name.find(':'); colon != std::string::npos) {
    name.resize(colon);
  }
  return name.empty() ? "com.mojang.minecraftpe" : name;
}

inline FILE *tryOpenIn(const std::string &dir, std::string &outPath) {
  namespace fs = std::filesystem;

  std::error_code ec;
  fs::create_directories(dir, ec);
  if (ec) {
    __android_log_print(ANDROID_LOG_WARN, kTag,
                        "nexlog: cannot create '%s': %s", dir.c_str(),
                        ec.message().c_str());
    return nullptr;
  }

  const std::string path = dir + "/" + kFileName;

  // Rotasi sederhana: file lama > 2 MB dipindah jadi .old.log
  struct stat st {};
  if (::stat(path.c_str(), &st) == 0 && st.st_size > kMaxFileBytes) {
    const std::string oldPath = dir + "/" + kOldFileName;
    std::remove(oldPath.c_str());
    std::rename(path.c_str(), oldPath.c_str());
  }

  FILE *f = std::fopen(path.c_str(), "ae");
  if (!f) {
    __android_log_print(ANDROID_LOG_WARN, kTag,
                        "nexlog: cannot open '%s': %s (errno=%d)",
                        path.c_str(), std::strerror(errno), errno);
    return nullptr;
  }

  outPath = path;
  return f;
}

// Harus dipanggil dengan mutex dipegang.
inline void openLocked(State &s) {
  if (s.file || s.attempted) {
    return;
  }
  s.attempted = true;

  s.file = tryOpenIn(kPrimaryDir, s.path);
  if (!s.file) {
    const std::string fallback = "/storage/emulated/0/Android/data/" +
                                 packageName() + "/files/NexCaise/logs";
    s.file = tryOpenIn(fallback, s.path);
  }

  if (!s.file) {
    __android_log_print(ANDROID_LOG_ERROR, kTag,
                        "nexlog: no writable log location, file logging off");
    return;
  }

  std::fprintf(s.file, "\n===== session start (pid %d) =====\n",
               static_cast<int>(::getpid()));
  std::fflush(s.file);
  __android_log_print(ANDROID_LOG_INFO, kTag, "nexlog: writing to %s",
                      s.path.c_str());
}

} // namespace detail

inline void write(Level level, std::string_view message) {
  auto &s = detail::state();

  // logcat tetap jalan (kalau ternyata bekerja di device lain)
  __android_log_print(detail::androidPriority(level), kTag, "%.*s",
                      static_cast<int>(message.size()), message.data());

  std::lock_guard lock(s.mutex);
  detail::openLocked(s);
  if (!s.file) {
    return;
  }

  timespec ts{};
  clock_gettime(CLOCK_REALTIME, &ts);
  tm local{};
  localtime_r(&ts.tv_sec, &local);

  char stamp[32];
  std::strftime(stamp, sizeof(stamp), "%Y-%m-%d %H:%M:%S", &local);

  std::fprintf(s.file, "%s.%03ld [%c] [%d] %.*s\n", stamp,
               ts.tv_nsec / 1000000L, detail::levelChar(level),
               static_cast<int>(::gettid()), static_cast<int>(message.size()),
               message.data());
  std::fflush(s.file); // flush tiap baris supaya log tidak hilang kalau game crash
}

// Gaya fmt: nexlog::info("Loaded {} hooks", n);
template <typename... Args>
inline void log(Level level, std::string_view fmtStr, Args &&...args) {
  try {
    const auto msg = fmt::vformat(fmt::string_view(fmtStr.data(), fmtStr.size()),
                                  fmt::make_format_args(args...));
    write(level, msg);
  } catch (const std::exception &ex) {
    write(Level::Error, std::string("log format failed: ") + ex.what());
    write(level, fmtStr);
  }
}

template <typename... Args>
inline void debug(std::string_view f, Args &&...a) { log(Level::Debug, f, a...); }
template <typename... Args>
inline void info(std::string_view f, Args &&...a) { log(Level::Info, f, a...); }
template <typename... Args>
inline void warn(std::string_view f, Args &&...a) { log(Level::Warn, f, a...); }
template <typename... Args>
inline void error(std::string_view f, Args &&...a) { log(Level::Error, f, a...); }

// Gaya printf: nexlog::logf(Level::Info, "mode=%d", x);
__attribute__((format(printf, 2, 3)))
inline void logf(Level level, const char *fmtStr, ...) {
  char stackBuf[512];
  va_list args;
  va_start(args, fmtStr);
  va_list copy;
  va_copy(copy, args);
  const int needed = std::vsnprintf(stackBuf, sizeof(stackBuf), fmtStr, args);
  va_end(args);

  if (needed < 0) {
    va_end(copy);
    write(level, fmtStr);
    return;
  }
  if (static_cast<size_t>(needed) < sizeof(stackBuf)) {
    va_end(copy);
    write(level, std::string_view(stackBuf, static_cast<size_t>(needed)));
    return;
  }

  std::string big(static_cast<size_t>(needed) + 1, '\0');
  std::vsnprintf(big.data(), big.size(), fmtStr, copy);
  va_end(copy);
  big.resize(static_cast<size_t>(needed));
  write(level, big);
}

// Buka file lebih awal (opsional). Mengembalikan path file, atau "" kalau gagal.
inline std::string init() {
  auto &s = detail::state();
  std::lock_guard lock(s.mutex);
  detail::openLocked(s);
  return s.file ? s.path : std::string{};
}

inline void shutdown() {
  auto &s = detail::state();
  std::lock_guard lock(s.mutex);
  if (s.file) {
    std::fprintf(s.file, "===== session end =====\n");
    std::fclose(s.file);
    s.file = nullptr;
  }
  s.attempted = false;
}

} // namespace nexlog

#define LOGD(...) ::nexlog::logf(::nexlog::Level::Debug, __VA_ARGS__)
#define LOGI(...) ::nexlog::logf(::nexlog::Level::Info, __VA_ARGS__)
#define LOGW(...) ::nexlog::logf(::nexlog::Level::Warn, __VA_ARGS__)
#define LOGE(...) ::nexlog::logf(::nexlog::Level::Error, __VA_ARGS__)
#define LOGF(...) ::nexlog::logf(::nexlog::Level::Fatal, __VA_ARGS__)
