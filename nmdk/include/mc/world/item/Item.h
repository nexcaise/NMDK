#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/world/item/components/IFoodItemComponent.h"

class Item {
  public:
    CLASS_FIELD(std::string, mRawNameId, 0xB0);
    CLASS_FIELD(std::string, mNamespace, 0xD0);
    CLASS_FIELD(short, mId, 0x8A);

  public:
    std::string getFullName() {
        std::string name = mNamespace;
        name += ":";
        name += mRawNameId;
        return name;
    }

  public:
    bool isFood() const;
    ::IFoodItemComponent *getFood() const;
};
