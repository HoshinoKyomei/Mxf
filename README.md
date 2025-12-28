# MxfGame

Developed with Unreal Engine 5.7

## Plugins
- Unreal
	- GameplayAbilities
	- GameplayCameras
    - GameplayStateTree
	- GameFeatures
	- ModularGameplay
    - CommonUI
- Custom
	- ModularGameplayActors
    - CommonGame
    - CommonUser
    - ToonCharacterShader
    - GameAnimationSystem
    - MannequinsAssetPack

## Frame structure
- ModularActor
  - BaseActor<Character, PlayerState, PlayerController>
    - BP_BaseActor(Blueprint)
- PawnComponent
  - BasePawnComponent
    - BP_BaseComponent(Blueprint)