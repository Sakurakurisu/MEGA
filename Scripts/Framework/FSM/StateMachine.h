#pragma once

#include "Component.h"
#include "E_StateType.h"
#include <memory>
#include <unordered_map>

class BaseState;

class StateMachine : public Component
{
public:
StateMachine() = default;

// 開始時に呼び出されるメソッド
void Start() override;

// 固定更新時に呼び出されるメソッド
void FixedUpdate() override;

// 毎フレーム更新時に呼び出されるメソッド
void Update() override;

// 状態を変更するメソッド
void ChangeState(E_StateType newStateType);

// 状態をスタックに追加するメソッド
void PushState(E_StateType newStateType);

// 状態をスタックから削除するメソッド
void PopState();

// 状態を追加するメソッド
StateMachine& AddState(E_StateType stateType, BaseState* state);

// デフォルト状態を設定するメソッド
StateMachine& SetDefaultState(E_StateType stateType);

// 現在の状態タイプを取得するメソッド
E_StateType GetCurrentStateType() const { return currentStateType; }

private:
E_StateType currentStateType = E_StateType::Default;
BaseState* currentState = nullptr;
std::unordered_map<E_StateType, std::unique_ptr<BaseState>> states;

std::vector<E_StateType> stateStack;
};
