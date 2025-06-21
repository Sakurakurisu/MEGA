#pragma once
#include "TweenManager.h"
#include "PropertyTweener.h"
#include "Sequence.h"
#include <memory>
#include <DirectXMath.h>
#include "Transform.h"
#include "Rigidbody2D.h"
#include "GameObject.h"
#include "Animator.h"
#include "ShakePositionTween.h"
#include "SpriteRenderer.h"

class DOTween
{
public:
	// 任意のプロパティに対する補間処理を生成
	template<typename T>
	static std::shared_ptr<PropertyTweener<T>> To(
		std::function<T()> getter,                    // 現在値の取得関数
		std::function<void(const T&)> setter,         // 値の設定関数
		const T& toValue,                             // 補間終了値
		float duration)                               // 補間時間
	{
		auto tweener = std::make_shared<PropertyTweener<T>>(std::move(getter), std::move(setter), toValue);
		tweener->SetDuration(duration);
		TweenManager::instance.AddTween(tweener);     // 補間管理に登録
		return tweener;
	}

	// シーケンスの生成（複数補間を順次実行）
	static std::shared_ptr<Sequence> CreateSequence()
	{
		auto seq = std::make_shared<Sequence>();
		TweenManager::instance.AddTween(seq);
		return seq;
	}

	static void UpdateAll() 
	{
		TweenManager::instance.Update();
	}

	static void KillAll()
	{
		TweenManager::instance.KillAll();
	}

	static void PauseAll()
	{
		TweenManager::instance.PauseAll();
	}

	static void ResumeAll() 
	{
		TweenManager::instance.ResumeAll();
	}

	// Transformの位置を補間
	static std::shared_ptr<PropertyTweener<XMFLOAT3>> DOMove(Transform& transform, const XMFLOAT3& toValue, float duration)
	{
		auto tweener = std::make_shared<PropertyTweener<XMFLOAT3>>(
			[&transform]()
			{
				return transform.position;
			},  // getter
			[&transform](const XMFLOAT3& val)
			{
				transform.position = val;
			},  // setter
			toValue 
		);
		tweener->SetDuration(duration);

		TweenManager::instance.AddTween(tweener);

		tweener->Play();

		return tweener;
	}

	// TransformのX位置を補間
	static std::shared_ptr<PropertyTweener<float>> DOMoveX(Transform* transform, const float& toValue, float duration)
	{
		auto tweener = std::make_shared<PropertyTweener<float>>(
			[transform]() -> float
			{
				return transform->gameObject->transform.position.x;
			},
			[transform](const float& val)
			{
				XMFLOAT3 newPos = XMFLOAT3(val, transform->position.y, transform->position.z);
				transform->position = newPos;
			},
			toValue
		);

		tweener->SetDuration(duration);
		TweenManager::instance.AddTween(tweener);
		tweener->Play();

		return tweener;
	}

	// Y方向補間（DOMoveXと同様）
	static std::shared_ptr<PropertyTweener<float>> DOMoveY(Transform* transform, const float& toValue, float duration)
	{
		auto tweener = std::make_shared<PropertyTweener<float>>(
			[transform]() -> float
			{
				return transform->gameObject->transform.position.y;
			},
			[transform](const float& val)
			{
				XMFLOAT3 newPos = XMFLOAT3(transform->position.x, val, transform->position.z);
				transform->position = newPos;
			},
			toValue
		);

		tweener->SetDuration(duration);
		TweenManager::instance.AddTween(tweener);
		tweener->Play();

		return tweener;
	}

	// Rigidbody2Dによる移動補間
	static std::shared_ptr<PropertyTweener<DirectX::XMFLOAT3>> DOMove(Rigidbody2D* rb, const DirectX::XMFLOAT3& toValue, float duration)
	{
		auto tweener = std::make_shared<PropertyTweener<DirectX::XMFLOAT3>>(
			[rb]() -> DirectX::XMFLOAT3
			{
				return rb->gameObject->transform.position;
			},
			[rb](const DirectX::XMFLOAT3& val)
			{
				XMFLOAT2 newPos = XMFLOAT2(val.x, val.y);
				rb->MovePosition(newPos);
			},
			toValue
		);

		tweener->SetDuration(duration);
		TweenManager::instance.AddTween(tweener);
		tweener->Play();

		return tweener;
	}

	// ローカル座標での移動補間
	static std::shared_ptr<PropertyTweener<DirectX::XMFLOAT3>> DOLocalMove(Transform* transform, const DirectX::XMFLOAT3& toValue, float duration)
	{
		auto tweener = std::make_shared<PropertyTweener<DirectX::XMFLOAT3>>(
			[transform]() -> DirectX::XMFLOAT3
			{
				return transform->localPosition;
			},
			[transform](const DirectX::XMFLOAT3& val)
			{
				transform->localPosition = val;
			},
			toValue
		);

		tweener->SetDuration(duration);
		TweenManager::instance.AddTween(tweener);
		tweener->Play();

		return tweener;
	}

	// スケール補間
	static std::shared_ptr<PropertyTweener<XMFLOAT3>> DOScale(Transform& transform, const XMFLOAT3& toValue, float duration)
	{
		auto tweener = std::make_shared<PropertyTweener<XMFLOAT3>>(
			[&transform]()
			{
				return transform.scale;
			},
			[&transform](const XMFLOAT3& val)
			{
				transform.scale = val;
			},
			toValue
		);
		tweener->SetDuration(duration);

		TweenManager::instance.AddTween(tweener);

		tweener->Play();

		return tweener;
	}

	// 回転補間
	static std::shared_ptr<PropertyTweener<float>> DORotation(Transform& transform, float toValue, float duration)
	{
		auto tweener = std::make_shared<PropertyTweener<float>>(
			[&transform]()
			{
				return transform.rotation;
			},  // getter
			[&transform](float val)
			{
				transform.rotation = val;
			},  // setter
			toValue
		);
		tweener->SetDuration(duration);

		TweenManager::instance.AddTween(tweener);

		tweener->Play();

		return tweener;
	}

	// アニメーター再生速度を一時的に変化（ヒットストップなど）
	static std::shared_ptr<PropertyTweener<float>> DOHitStop(Animator* animator, float toValue, float duration)
	{
		auto tweener = std::make_shared<PropertyTweener<float>>(
			[animator]()
			{
				return animator->GetPlaySpeed();
			},
			[animator](float val)
			{
				animator->SetPlaySpeed(val);
			},
			toValue
		);
		tweener->SetDuration(duration)
			->SetLoops(2, LoopType::Yoyo)      // 往復補間
			->SetEase(EaseType::ExpOut);       // 緩やかな終了
		TweenManager::instance.AddTween(tweener);
		tweener->Play();
		return tweener;
	}

	// 位置を振動させるTween（Shake）
	static std::shared_ptr<ShakePositionTween> DOShakePosition(
		Transform& transform,
		float duration,
		float strength,
		int vibrato,
		bool fadeOut)
	{
		auto tween = std::make_shared<ShakePositionTween>(
			[&transform]()
			{
				return transform.position;
			},
			[&transform](const XMFLOAT3& val)
			{
				transform.position = val;
			},
			duration, strength, vibrato, fadeOut);

		TweenManager::instance.AddTween(tween);

		tween->Play();

		return tween;
	}

	// ローカル座標の振動
	static std::shared_ptr<ShakePositionTween> DOShakeLocalPosition(
		Transform& transform,
		float duration,
		float strength,
		int vibrato,
		bool fadeOut)
	{
		auto tween = std::make_shared<ShakePositionTween>(
			[&transform]()
			{
				return transform.localPosition;
			},
			[&transform](const XMFLOAT3& val)
			{
				transform.localPosition = val;
			},
			duration, strength, vibrato, fadeOut);

		TweenManager::instance.AddTween(tween);

		tween->Play();

		return tween;
	}

	// SpriteRendererの透明度を補間
	static std::shared_ptr<PropertyTweener<float>> DOFade(SpriteRenderer* spriteRenderer, float toValue, float duration)
	{
		auto tweener = std::make_shared<PropertyTweener<float>>(
			[spriteRenderer]()
			{
				return spriteRenderer->GetAlpha();
			},  // getter
			[spriteRenderer](float val)
			{
				spriteRenderer->SetAlpha(val);
			},  // setter
			toValue 
		);
		tweener->SetDuration(duration);

		TweenManager::instance.AddTween(tweener);

		tweener->Play();

		return tweener;
	}

	// SpriteRendererの色を補間
	static std::shared_ptr<PropertyTweener<XMFLOAT4>> DOColor(SpriteRenderer* spriteRenderer, XMFLOAT4 toValue, float duration)
	{
		auto tweener = std::make_shared<PropertyTweener<XMFLOAT4>>(
			[spriteRenderer]()
			{
				return spriteRenderer->GetColor();
			},  // getter
			[spriteRenderer](XMFLOAT4 val)
			{
				spriteRenderer->SetColor(val);
			},  // setter
			toValue
		);
		tweener->SetDuration(duration);

		TweenManager::instance.AddTween(tweener);

		tweener->Play();

		return tweener;
	}

	// マテリアル色の補間
	static std::shared_ptr<PropertyTweener<XMFLOAT4>> DOColor(Material& material, XMFLOAT4 toValue, float duration)
	{
		auto tweener = std::make_shared<PropertyTweener<XMFLOAT4>>(
			[&material]() 
			{
				return material.diffuse;
			},
			[&material](XMFLOAT4 val)
			{
				material.diffuse = val;
			},
			toValue
		);
		tweener->SetDuration(duration);
		TweenManager::instance.AddTween(tweener);
		tweener->Play();
		return tweener;
	}

};
