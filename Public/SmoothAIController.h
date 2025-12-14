//MIT License
//Copyright(c) 2025 Pixel
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI_FocusTarget_Enum.h"
#include "InterpType_Enum.h"
#include "SmoothAIController.generated.h"

/**
 * 
 */
UCLASS()
class PLAYGROUND_API ASmoothAIController : public AAIController
{
	GENERATED_BODY()
	
public:
    ASmoothAIController();

    virtual void Tick(float DeltaSeconds) override;
    virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn);

    void SetFocus(AActor* NewFocus, EAIFocusPriority::Type InPriority = EAIFocusPriority::Gameplay);
    void SetFocalPoint(const FVector& InFocalPoint, EAIFocusPriority::Type InPriority = EAIFocusPriority::Gameplay);
    void ClearFocus(EAIFocusPriority::Type InPriority = EAIFocusPriority::Gameplay);

private:
    FVector DesiredFocusLocation;
    TWeakObjectPtr<AActor> FocusActor;
    EAI_FocusTarget_Enum EHasFocusTarget;

    UFUNCTION()
    FRotator InterpRotation(const FRotator& Current, const FRotator& Target, float DeltaSeconds) const;

    UFUNCTION(BlueprintCallable, Category = "Smooth Focus")
    void RandomizeSmoothFocusSettings(float MinInterpSpeed, float MaxInterpSpeed, const TArray<EInterpType_Enum>& AllowedInterpTypes);

    UFUNCTION(BlueprintCallable, Category = "Smooth Focus")
    void RandomizeSmoothFocusSettingsWithDegrees(float MinInterpSpeed, float MaxInterpSpeed, const TArray<EInterpType_Enum>& AllowedInterpTypes, float MinDegreesPerSec, float MaxDegreesPerSec);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Smooth Focus", meta = (AllowPrivateAccess = "true", ClampMin = "0.001", ClampMax = "250"))
    float SmoothFocusInterpSpeed;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Smooth Focus", meta = (AllowPrivateAccess = "true"))
    EInterpType_Enum InterpType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Smooth Focus", meta = (AllowPrivateAccess = "true", EditCondition = "InterpType == EInterpType_Enum::ConstantSpeed", EditConditionHides, ClampMin = "1.0", ClampMax = "360.0"))
    float DegreesPerSecond;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Smooth Focus | Eye Level", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "200"))
    float Self_EyeLevelHeightOffsetRelative;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Smooth Focus | Eye Level", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "200"))
    float Other_EyeLevelHeightOffsetRelative;
};
