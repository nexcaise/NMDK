#pragma once

#include "mc/_HeaderOutputPredefine.h"

class IFoodItemComponent {
  public:
    int getNutrition() const;
    float getSaturationModifier() const;
    bool canAlwaysEat() const;
};
