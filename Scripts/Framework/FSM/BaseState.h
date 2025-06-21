#pragma once  
#include <iostream>  
#include <memory>  
#include <cassert>  
#include "GameObject.h"  
#include "Animator.h"  
#include "StateMachine.h"  

class StateMachine;  

class BaseState  
{  
public:  
   virtual ~BaseState() = default;  

   // 状態が開始されるときに呼び出される  
   virtual void Enter() {}  

   // 毎フレーム更新処理を行う  
   virtual void Update() {}  

   // 固定フレームレートで更新処理を行う  
   virtual void FixedUpdate() {}  

   // 状態が終了するときに呼び出される  
   virtual void Exit() {}  

   // 状態マシンを設定する  
   void SetStateMachine(StateMachine* fsm)  
   {  
       stateMachine = fsm;  
       gameObject = stateMachine->gameObject;  
   }  

protected:  
   StateMachine* stateMachine = nullptr;  
   GameObject* gameObject = nullptr;  
};