#pragma once

#include "BaseManager.h"
#include "EventInfo.h"
#include "EventKey.h"
#include <unordered_map>
#include <memory>
#include <functional>
#include <typeinfo>

class EventCenter : public BaseManager<EventCenter>
{
friend class BaseManager<EventCenter>;

public:
// イベントをトリガーする（引数あり）
template<typename... Args>
void EventTrigger(E_EventType eventName, Args... args);

// イベントをトリガーする（引数なし）
void EventTrigger(E_EventType eventName);

// イベントリスナーを追加する（引数なし）
void AddEventListener(E_EventType eventName, const std::function<void()>& func);

// イベントリスナーを追加する（オブジェクトメソッド、引数なし）
template<typename Obj>
void AddEventListener(E_EventType eventName, Obj* obj, void (Obj::* method)());

// イベントリスナーを追加する（引数あり）
template<typename... Args>
void AddEventListener(E_EventType eventName, const std::function<void(Args...)>& func);

// イベントリスナーを追加する（オブジェクトメソッド、引数あり）
template<typename... Args, typename Obj>
void AddEventListener(E_EventType eventName, Obj* obj, void (Obj::* method)(Args...));

// イベントリスナーを削除する（オブジェクトメソッド、引数なし）
template<typename Obj>
void RemoveEventListener(E_EventType eventName, Obj* obj, void (Obj::* method)());

// イベントリスナーを削除する（オブジェクトメソッド、引数あり）
template<typename... Args, typename Obj>
void RemoveEventListener(E_EventType eventName, Obj* obj, void (Obj::* method)(Args...));

// 全てのイベントリスナーをクリアする
void Clear();

// 特定のイベントリスナーをクリアする
void Clear(E_EventType eventName);

private:
EventCenter() = default;
~EventCenter() = default;
EventCenter(const EventCenter&) = delete;
EventCenter& operator=(const EventCenter&) = delete;

std::unordered_map<EventKey, std::shared_ptr<EventInfoBase>> eventDic;
};

#include "EventCenter.inl"
