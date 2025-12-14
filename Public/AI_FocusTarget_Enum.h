#pragma once

#include "CoreMinimal.h"
#include "AI_FocusTarget_Enum.generated.h"

UENUM(BlueprintType)
enum class EAI_FocusTarget_Enum : uint8
{
    NoFocus,
    HasTargetLocation,
    HasTargetActor
};