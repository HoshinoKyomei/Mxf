# MxfGame

Developed with Unreal Engine 5.7

## Plugins
- Unreal
	- GameplayAbilities
	- GameplayCameras
	- GameFeatures
	- ModularGameplay
- Custom
	- ModularGameplayActors
    - TemplateResources
    - ToonCharacterShader

## Frame structure
- ModularActor
  - BaseActor<Character, PlayerState, PlayerController>
    - BP_BaseActor(Blueprint)
- PawnComponent
  - BasePawnComponent
    - BP_BaseComponent(Blueprint)