#pragma once

#include "Component.h"
#include "E_AnimType.h"
#include "GameObject.h"
#include "AnimationClip.h"

class SpriteRenderer;

using FrameEvent = std::function<void()>;

class Animator : public Component
{
public:
Animator() = default;

// アニメーターの初期化を行う
void Start() override;

// アニメーションの更新処理を行う
void Update() override;

// 指定されたアニメーションを再生する
void PlayAnimation(E_AnimType type);

// 現在のアニメーションを停止する
void StopAnimation();

// アニメーションを追加する
Animator& AddAnim(E_AnimType type, std::shared_ptr<AnimationClip> anim);

// 再生速度を設定する
Animator& SetPlaySpeed(float speed);

// 再生速度を取得する
float GetPlaySpeed() const;

// アニメーションが再生中かどうかを確認する
bool IsPlaying() const;

// フレームイベントを追加する
Animator& AddFrameEvent(E_AnimType type, int frame, FrameEvent event);

private:
void ResetCurrentAnim();

std::unordered_map<E_AnimType, std::shared_ptr<AnimationClip>> anims;

const AnimationClip* currentAnim = nullptr;
SpriteRenderer* spriteRenderer = nullptr;

bool playing = false;

float timeCounter = 0.0f;
int idxFrame = 0;

float animTimePerFrame = 0.0f;
int animFrameCount = 0;
bool animIsLoop = false;

float playSpeed = 1.0f;

E_AnimType currentAnimType;

std::unordered_map<E_AnimType, std::unordered_map<int, std::vector<FrameEvent>>> animFrameEvents;
};
