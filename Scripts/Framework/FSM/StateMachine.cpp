#include "StateMachine.h"  
#include "BaseState.h"  
#include "Debug.h"  
#include "GameTime.h"  

void StateMachine::Start()  
{  
   // 現在の状態を検索し、見つかった場合は初期化する  
   auto it = states.find(currentStateType);  
   if (it != states.end())  
   {  
       currentState = it->second.get();  
       currentState->Enter();  
   }  
   else  
       Debug::Log("Default state not found in the StateMachine." + static_cast<int>(currentStateType), Debug::LogLevel::WARN);  
}  

void StateMachine::FixedUpdate()  
{  
   // 現在の状態が存在する場合、FixedUpdateを呼び出す  
   if (currentState)  
       currentState->FixedUpdate();  
}  

void StateMachine::Update()  
{  
   // 現在の状態が存在する場合、Updateを呼び出す  
   if (currentState)  
       currentState->Update();  
}  

void StateMachine::ChangeState(E_StateType newStateType)  
{  
   // 同じ状態への変更を無視する  
   if (currentStateType == newStateType) return;  

   // 現在の状態を終了する  
   if (currentState)  
       currentState->Exit();  

   currentStateType = newStateType;  

   // 新しい状態を検索し、見つかった場合は初期化する  
   auto it = states.find(currentStateType);  
   if (it != states.end())  
   {  
       currentState = it->second.get();  
       currentState->Enter();  
   }  
   else  
       Debug::Log("State not found in the StateMachine.", Debug::LogLevel::WARN);  
}  

void StateMachine::PushState(E_StateType newStateType)  
{  
   // 現在の状態をスタックに保存し、終了する  
   if (currentState)  
   {  
       currentState->Exit();  
       stateStack.push_back(currentStateType);  
   }  

   // 新しい状態を検索し、見つかった場合は初期化する  
   auto it = states.find(newStateType);  
   if (it != states.end())  
   {  
       currentStateType = newStateType;  
       currentState = it->second.get();  
       currentState->Enter();  
   }  
   else  
       Debug::Log("Pushed state not found in the StateMachine.", Debug::LogLevel::WARN);  
}  

void StateMachine::PopState()  
{  
   // 現在の状態を終了する  
   if (currentState)  
       currentState->Exit();  

   // スタックが空でない場合、前の状態に戻る  
   if (!stateStack.empty())  
   {  
       E_StateType previousStateType = stateStack.back();  
       stateStack.pop_back();  

       auto it = states.find(previousStateType);  
       if (it != states.end())  
       {  
           currentStateType = previousStateType;  
           currentState = it->second.get();  
           currentState->Enter();  
       }  
       else  
           Debug::Log("Popped state not found in the StateMachine.", Debug::LogLevel::WARN);  
   }  
   else  
       Debug::Log("State stack is empty. Cannot pop state.", Debug::LogLevel::WARN);  
}  

StateMachine& StateMachine::AddState(E_StateType stateType, BaseState* state)  
{  
   // 状態が既に存在する場合、警告を出力する  
   if (states.find(stateType) != states.end())  
       Debug::Log("State already exists in the StateMachine.", Debug::LogLevel::WARN);  

   states.emplace(stateType, std::unique_ptr<BaseState>(state));  

   // 状態にStateMachineを設定する  
   state->SetStateMachine(this);  

   return *this;  
}  

StateMachine& StateMachine::SetDefaultState(E_StateType stateType)  
{  
   // デフォルト状態が存在しない場合、警告を出力する  
   if (states.find(stateType) == states.end())  
       Debug::Log("State not found in the StateMachine.", Debug::LogLevel::WARN);  

   currentStateType = stateType;  

   return *this;  
}