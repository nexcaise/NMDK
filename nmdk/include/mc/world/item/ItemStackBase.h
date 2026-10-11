#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/common/SharedPtr.h"
#include "mc/common/WeakPtr.h"
#include "mc/world/item/Item.h"

class CompoundTag;

class ItemStackBase {
  public:
    WeakPtr<Item> mItem;
    CompoundTag *mUserData;
    const void *mBlock;
    short mAuxValue;
    uchar mCount;
    bool mValid_DeprecatedSeeComment;
    bool mShowPickUp;
    bool mWasPickedUp;
    uchar _pad_26[0x28 - 0x26];
    std::chrono::steady_clock::time_point mPickupTime;

  public:
    ItemStackBase();
    ItemStackBase(const ItemStackBase &);
    ItemStackBase &operator=(const ItemStackBase &);
};
