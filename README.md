# Smooth AI Controller for Unreal Engine 5 

> [!NOTE]
> Will probably also work for Unreal Engine 4 with minimal setup

An AI controller for smooth turning that allows AIs to rotate toward target actors or points. Unlike the default AI Controller focus code, this controller interpolates rotation smoothly over time, giving your AI smooth looking motion instead of snapping instantly

## Features
- Smooth yaw and pitch rotation towards a target actor or world location
- Fully editable via blueprint defaults
  - Customizable interpolation speed
  - Customizable interpolation type:
    - Exponential (default, most natural)
    - Linear
    - Constant speed (degrees/sec)
  - Customizable degrees per second
  - Eye level height offset for the AI
  - Eye level height offset for target actors
- Randomization functions for per-AI variation:
  - RandomizeSmoothFocusSettings
    - Random interp speed between min/max
    - Random interp type from a array
  - RandomizeSmoothFocusSettingsWithDegrees
    - Random interp speed between min/max
    - Random interp type from a array
    - Random degrees per second between min/max
   
## Fixes
- Changes focus behavior to allow pitch changes, fixing the "AI only rotates in yaw" problem

## Installation
- Copy SmoothAIController.h and SmoothAIController.cpp into your project’s Source folder
- In your AI characters settings set:
  ```
  bUseControllerRotationYaw = true;
  GetCharacterMovement()->bOrientRotationToMovement = false;
  ```
  This prevents movement logic from fighting the controller rotation
- In your AI character Blueprint, set AIController class to SmoothAIController

## Usage
You can use the build in **SetFocus()**, **SetFocalPoint()** and **ClearFocus()** functions, as the code replaces those function with its own code

## License
### MIT License
feel free to use, modify, and integrate it into your UE5/UE4 projects
