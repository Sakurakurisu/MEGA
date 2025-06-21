#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <type_traits>
#include <cstdint>
#include <cstring>
#include <utility>
#include <unordered_map>

// イベントデリゲート：複数の関数・メンバ関数を登録し、一括呼び出しできる
template<typename... Args>
class DelegateAction
{
public:
	// 関数ポインタ（グローバル関数や静的関数）を登録
	void Add(void(*func)(Args...))
	{
		CallbackHolder holder;
		holder.callback = func;
		holder.objectPtr = 0;
		holder.methodPtr = reinterpret_cast<std::uintptr_t>(func); // 一意のIDとして使う
		callbacks.emplace_back(holder);
	}

	// std::function（ラムダやバインド関数など）を登録
	void Add(const std::function<void(Args...)>& func)
	{
		CallbackHolder holder;
		holder.callback = func;
		holder.objectPtr = 0;
		holder.methodPtr = 0; // 判別不可なので0
		callbacks.emplace_back(holder);
	}

	// 任意のクラスのメンバ関数を登録
	template<typename T>
	void Add(T* object, void (T::* method)(Args...))
	{
		CallbackHolder holder;
		holder.callback = [object, method](Args... args)
			{
				(object->*method)(args...); // 呼び出しをラップ
			};
		holder.objectPtr = reinterpret_cast<std::uintptr_t>(object);
		holder.methodPtr = memberFunctionToUIntptr(method); // 一意識別用
		callbacks.emplace_back(holder);
	}

	// 関数ポインタを削除
	void Remove(void(*func)(Args...))
	{
		std::uintptr_t targetMethodPtr = reinterpret_cast<std::uintptr_t>(func);
		std::erase_if(callbacks, [=](const CallbackHolder& holder)
					  {
						  return holder.objectPtr == nullptr && holder.methodPtr == targetMethodPtr;
					  });
	}

	// 登録済みのメンバ関数を削除
	template<typename T>
	void Remove(T* object, void (T::* method)(Args...))
	{
		std::uintptr_t targetObjectPtr = reinterpret_cast<std::uintptr_t>(object);
		std::uintptr_t targetMethodPtr = memberFunctionToUIntptr(method);
		std::erase_if(callbacks, [=](const CallbackHolder& holder)
					  {
						  return holder.objectPtr == targetObjectPtr && holder.methodPtr == targetMethodPtr;
					  });
	}

	// 登録されたすべてのコールバックを呼び出す
	void Invoke(Args... args)
	{
		auto currentCallbacks = callbacks; // 呼び出し中に変更されても安全なようにコピー
		for (auto& holder : currentCallbacks)
		{
			if (holder.callback)
				holder.callback(std::forward<Args>(args)...);
		}
	}

private:
	struct CallbackHolder
	{
		std::function<void(Args...)> callback = nullptr;
		std::uintptr_t objectPtr{};  // オブジェクトのアドレス
		std::uintptr_t methodPtr{};  // メソッドの識別用キー（ポインタをuintptr_tに変換）
	};

	// メンバ関数ポインタをuintptr_tに変換（比較用IDとして使用）
	template<typename T>
	std::uintptr_t memberFunctionToUIntptr(void (T::* method)(Args...))
	{
		using MethodType = void (T::*)(Args...);
		static_assert(sizeof(MethodType) <= sizeof(std::uintptr_t), "Method pointer too big");
		std::uintptr_t ptr = 0;
		std::memcpy(&ptr, &method, sizeof(method)); // 安全なコピー
		return ptr;
	}

	std::vector<CallbackHolder> callbacks; // コールバック登録リスト
};
