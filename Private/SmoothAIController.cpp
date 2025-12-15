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

#include "SmoothAIController.h"
#include "Kismet/KismetMathLibrary.h"

ASmoothAIController::ASmoothAIController()
{
    PrimaryActorTick.bCanEverTick = true;
    EHasFocusTarget = EAI_FocusTarget_Enum::NoFocus;
    SmoothFocusInterpSpeed = 4.0f;
    InterpType = EInterpType_Enum::EaseOut;
    DegreesPerSecond = 180.0f;

    Self_EyeLevelHeightOffsetRelative = 60.0f;
    Other_EyeLevelHeightOffsetRelative = 60.0f;
}

void ASmoothAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    //Has no focus or controllign pawn
    if (EHasFocusTarget == EAI_FocusTarget_Enum::NoFocus || !GetPawn())
    {
        return;
    }

    const FVector PawnLocation = GetPawn()->GetActorLocation() + FVector(Self_EyeLevelHeightOffsetRelative, 0, 0);
    FVector TargetLocation;
    //The code will here always have a focus, so only two options
    if (EHasFocusTarget == EAI_FocusTarget_Enum::HasTargetLocation) {
        TargetLocation = DesiredFocusLocation;
    }
    else { //HasTargetActor
        if (FocusActor.IsValid()) {
            TargetLocation = FocusActor->GetActorLocation() + FVector(Other_EyeLevelHeightOffsetRelative, 0, 0);
        }
        else {
            ClearFocus();
            return;
        }
    }

    //Get the rotation
    const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PawnLocation, TargetLocation);
    const FRotator CurrentRotation = GetControlRotation();

    const FRotator SmoothRotation = InterpRotation(GetControlRotation(), TargetRotation, DeltaSeconds);

    SetControlRotation(SmoothRotation);
}

void ASmoothAIController::SetFocus(AActor* NewFocus, EAIFocusPriority::Type InPriority)
{
    if (!NewFocus)
    {
        ClearFocus(InPriority);
        return;
    }

    FocusActor = NewFocus;
    EHasFocusTarget = EAI_FocusTarget_Enum::HasTargetActor;
}

void ASmoothAIController::SetFocalPoint(const FVector& InFocalPoint, EAIFocusPriority::Type InPriority)
{
    FocusActor = nullptr;
    DesiredFocusLocation = InFocalPoint;
    EHasFocusTarget = EAI_FocusTarget_Enum::HasTargetLocation;
}

void ASmoothAIController::ClearFocus(EAIFocusPriority::Type InPriority)
{
    Super::ClearFocus(InPriority);

    FocusActor = nullptr;
    EHasFocusTarget = EAI_FocusTarget_Enum::NoFocus;
}

FRotator ASmoothAIController::InterpRotation(const FRotator& Current, const FRotator& Target, float DeltaSeconds) const
{
    switch (InterpType)
    {
    case EInterpType_Enum::Linear:
    {
        return FMath::Lerp(Current, Target, DeltaSeconds * SmoothFocusInterpSpeed);
    }

    case EInterpType_Enum::EaseIn:
    {
        const float alpha = FMath::Clamp(DeltaSeconds * SmoothFocusInterpSpeed, 0.0f, 1.0f);
        return FMath::InterpEaseIn(Current, Target, alpha, 1.0f);
    }

    case EInterpType_Enum::EaseOut:
    {
        return FMath::RInterpTo(Current, Target, DeltaSeconds, SmoothFocusInterpSpeed);
    }

    case EInterpType_Enum::EaseInOut:
    {
        const float alpha = FMath::Clamp(DeltaSeconds * SmoothFocusInterpSpeed, 0.0f, 1.0f);
        return FMath::InterpEaseInOut(Current, Target, alpha, 1.0f);
    }

    case EInterpType_Enum::ConstantSpeed:
    {
        return FMath::RInterpConstantTo(Current, Target, DeltaSeconds, DegreesPerSecond);
    }

    default:
        return Target;
    }
}

void ASmoothAIController::RandomizeSmoothFocusSettings(float MinInterpSpeed, float MaxInterpSpeed, const TArray<EInterpType_Enum>& AllowedInterpTypes)
{
    //IS valid checks
    if (MinInterpSpeed > MaxInterpSpeed)
    {
        Swap(MinInterpSpeed, MaxInterpSpeed);
    }
    else if (MinInterpSpeed == MaxInterpSpeed) {
        MaxInterpSpeed = MinInterpSpeed + 1;
    }

    //Randomize values
    SmoothFocusInterpSpeed = FMath::Clamp(FMath::FRandRange(MinInterpSpeed, MaxInterpSpeed), 0.001, 250);

    if (AllowedInterpTypes.Num() > 0)
    {
        const int8 Index = FMath::RandRange(0, AllowedInterpTypes.Num() - 1);
        InterpType = AllowedInterpTypes[Index];
    }
    else {
        InterpType = EInterpType_Enum::EaseOut;
    }
}

void ASmoothAIController::RandomizeSmoothFocusSettingsWithDegrees(float MinInterpSpeed, float MaxInterpSpeed, const TArray<EInterpType_Enum>& AllowedInterpTypes, float MinDegreesPerSec, float MaxDegreesPerSec)
{
    RandomizeSmoothFocusSettings(MinInterpSpeed, MaxInterpSpeed, AllowedInterpTypes);

    //IS valid checks
    if (MinDegreesPerSec > MaxDegreesPerSec)
    {
        Swap(MinDegreesPerSec, MaxDegreesPerSec);
    }
    else if (MinDegreesPerSec == MaxDegreesPerSec) {
        MaxDegreesPerSec = MinDegreesPerSec + 1;
    }

    //Randomize values
    if (InterpType == EInterpType_Enum::ConstantSpeed)
    {
        DegreesPerSecond = FMath::Clamp(FMath::FRandRange(MinDegreesPerSec, MaxDegreesPerSec), 1.0f, 360.0f);
    }
}

//Here is the "fix" located
void ASmoothAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
    //Code from Unreal Engine 5 source code
    APawn* const MyPawn = GetPawn();
    if (MyPawn)
    {
        FRotator NewControlRotation = GetControlRotation();

        const FVector FocalPoint = GetFocalPoint();
        if (FAISystem::IsValidLocation(FocalPoint))
        {
            NewControlRotation = (FocalPoint - MyPawn->GetPawnViewLocation()).Rotation();
        }
        else if (bSetControlRotationFromPawnOrientation)
        {
            NewControlRotation = MyPawn->GetActorRotation();
        }

        // Commented this out, as this makes the pitch not changeable with focusing on a actor
        /*if (NewControlRotation.Pitch != 0 && Cast<APawn>(GetFocusActor()) == nullptr)
        {
            NewControlRotation.Pitch = 0.f;
        }*/

        SetControlRotation(NewControlRotation);

        if (bUpdatePawn)
        {
            const FRotator CurrentPawnRotation = MyPawn->GetActorRotation();

            if (CurrentPawnRotation.Equals(NewControlRotation, 1e-3f) == false)
            {
                MyPawn->FaceRotation(NewControlRotation, DeltaTime);
            }
        }
    }
}
