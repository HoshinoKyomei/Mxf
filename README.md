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

## 框架修改
- Lyra
  - PawnExt 
    - RegisterInitState移至初始化组件
  - Input
    - BindAtion支持蓝图
      - 添加FunctionInputActions
  - PawnData
    - 起始位置默认位于PlayerState
    - 添加InputSet
  - Character切换至MoverPawn
    - 使用Mover的GameplayTags替代原移动模式Tags
- GASP_Mover
  - 解绑IA输入
    - 可通过重载支持InputActionValue
  - 添加自定义移动模式Cover
    - 修改输入代理接口
   
---
扩展InputSet，InputActionEvent可使用数据资产绑定蓝图中编写的函数/事件至输入映射
