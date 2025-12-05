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

## Frame structure
- ModularActor
  - BaseActor<Character, PlayerState, PlayerController>
    - BP_BaseActor(Blueprint)
- PawnComponent
  - BasePawnComponent
    - BP_BaseComponent(Blueprint)