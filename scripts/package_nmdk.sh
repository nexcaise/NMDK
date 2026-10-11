#!/usr/bin/env bash
#
# Packages the NMDK mod itself into dist/nmdk.levipack
#
# Contents of the package:
#   libnmdk.so      <- nmdk/libs/arm64-v8a/libnmdk.so
#   manifest.json   <- nmdk/mod/manifest.json (version synced from nmdk.json)
#   LICENSE         <- nmdk/mod/LICENSE
#   README.md       <- README.md (version line synced from nmdk.json)
#   CREDITS/        <- CREDITS/
#
# Usage: bash scripts/package_nmdk.sh [output_dir]   (default: dist)

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

OUT_DIR="${1:-dist}"
PACK_DIR="$OUT_DIR/nmdk_pack"
PACK_FILE="$OUT_DIR/nmdk.levipack"

SO_FILE="nmdk/libs/arm64-v8a/libnmdk.so"
MANIFEST_SRC="nmdk/mod/manifest.json"
LICENSE_SRC="nmdk/mod/LICENSE"

for cmd in zip python3; do
    command -v "$cmd" >/dev/null 2>&1 || {
        echo "Error: '$cmd' command not found." >&2
        exit 1
    }
done

for f in nmdk.json "$SO_FILE" "$MANIFEST_SRC" README.md; do
    [ -f "$f" ] || { echo "Error: required file '$f' not found." >&2; exit 1; }
done
[ -d CREDITS ] || { echo "Error: 'CREDITS' directory not found." >&2; exit 1; }

# Read the version from nmdk.json (single source of truth).
VERSION="$(python3 - <<'PY'
import json, sys
v = json.load(open("nmdk.json", encoding="utf-8")).get("version")
if not isinstance(v, str) or not v.strip():
    sys.exit("nmdk.json has no valid \"version\" field")
print(v.strip())
PY
)"

echo "NMDK version: $VERSION"

rm -rf "$PACK_DIR" "$PACK_FILE"
mkdir -p "$PACK_DIR"

# Library
cp "$SO_FILE" "$PACK_DIR/libnmdk.so"

# License (falls back to the root LICENSE)
if [ -f "$LICENSE_SRC" ]; then
    cp "$LICENSE_SRC" "$PACK_DIR/LICENSE"
else
    cp LICENSE "$PACK_DIR/LICENSE"
fi

# Manifest: force "version" to match nmdk.json
# (works for the "${nmdk.version}" placeholder as well as a hard-coded value)
NMDK_VERSION="$VERSION" python3 - "$MANIFEST_SRC" "$PACK_DIR/manifest.json" <<'PY'
import json, os, sys
src, dst = sys.argv[1], sys.argv[2]
with open(src, encoding="utf-8") as f:
    manifest = json.load(f)
manifest["version"] = os.environ["NMDK_VERSION"]
with open(dst, "w", encoding="utf-8") as f:
    json.dump(manifest, f, indent=2, ensure_ascii=False)
    f.write("\n")
PY

# README (the "> Version: vX.Y.Z" line is synced as well) and CREDITS
sed -E "s/^(> Version: v).*/\1${VERSION}/" README.md > "$PACK_DIR/README.md"
cp -r CREDITS "$PACK_DIR/CREDITS"

# Build the .levipack (a zip with the files at the root)
(
    cd "$PACK_DIR"
    zip -r -q "../nmdk.levipack" .
)

echo "Package contents:"
unzip -l "$PACK_FILE"
echo "Mod package : $PACK_FILE"

if [ -n "${GITHUB_OUTPUT:-}" ]; then
    echo "nmdk_pack_file=$PACK_FILE" >> "$GITHUB_OUTPUT"
    echo "nmdk_version=$VERSION" >> "$GITHUB_OUTPUT"
fi
