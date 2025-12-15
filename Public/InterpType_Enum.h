#pragma once

#include "CoreMinimal.h"
#include "InterpType_Enum.generated.h"

UENUM(BlueprintType)
enum class EInterpType_Enum : uint8
{
    Linear,
    EaseIn,
    EaseOut,
    EaseInOut,
    ConstantSpeed
};