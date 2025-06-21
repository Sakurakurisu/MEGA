// EventCenter.inl  
#pragma once  

#include "EventCenter.h"  

// イベントをトリガーする関数  
template<typename... Args>  
void EventCenter::EventTrigger(E_EventType eventName, Args... args)  
{  
   EventKey key{ eventName, typeid(std::tuple<Args...>) };  
   auto it = eventDic.find(key);  
   if (it != eventDic.end())  
   {  
       auto eventInfo = std::dynamic_pointer_cast<EventInfoT<Args...>>(it->second);  
       if (eventInfo)  
           eventInfo->actions.Invoke(args...);  
   }  
}  

// オブジェクトのメソッドをイベントリスナーとして追加する関数（引数なし）  
template<typename Obj>  
void EventCenter::AddEventListener(E_EventType eventName, Obj* obj, void (Obj::* method)())  
{  
   EventKey key{ eventName, typeid(std::tuple<>) };  
   auto it = eventDic.find(key);  
   if (it != eventDic.end())  
   {  
       auto eventInfo = std::dynamic_pointer_cast<EventInfo>(it->second);  
       if (eventInfo)  
           eventInfo->actions.Add(obj, method);  
   }  
   else  
   {  
       auto eventInfo = std::make_shared<EventInfo>();  
       eventDic[key] = eventInfo;  
       eventInfo->actions.Add(obj, method);  
   }  
}  

// 関数オブジェクトをイベントリスナーとして追加する関数  
template<typename... Args>  
void EventCenter::AddEventListener(E_EventType eventName, const std::function<void(Args...)>& func)  
{  
   EventKey key{ eventName, typeid(std::tuple<Args...>) };  
   auto it = eventDic.find(key);  
   if (it != eventDic.end())  
   {  
       auto eventInfo = std::dynamic_pointer_cast<EventInfoT<Args...>>(it->second);  
       if (eventInfo)  
           eventInfo->actions.Add(func);  
   }  
   else  
   {  
       auto eventInfo = std::make_shared<EventInfoT<Args...>>();  
       eventDic[key] = eventInfo;  
       eventInfo->actions.Add(func);  
   }  
}  

// オブジェクトのメソッドをイベントリスナーとして追加する関数（引数あり）  
template<typename... Args, typename Obj>  
void EventCenter::AddEventListener(E_EventType eventName, Obj* obj, void (Obj::* method)(Args...))  
{  
   EventKey key{ eventName, typeid(std::tuple<Args...>) };  
   auto it = eventDic.find(key);  
   if (it != eventDic.end())  
   {  
       auto eventInfo = std::dynamic_pointer_cast<EventInfoT<Args...>>(it->second);  
       if (eventInfo)  
           eventInfo->actions.Add(obj, method);  
   }  
   else  
   {  
       auto eventInfo = std::make_shared<EventInfoT<Args...>>();  
       eventDic[key] = eventInfo;  
       eventInfo->actions.Add(obj, method);  
   }  
}  

// オブジェクトのメソッドをイベントリスナーから削除する関数（引数なし）  
template<typename Obj>  
void EventCenter::RemoveEventListener(E_EventType eventName, Obj* obj, void (Obj::* method)())  
{  
   EventKey key{ eventName, typeid(std::tuple<>) };  
   auto it = eventDic.find(key);  
   if (it != eventDic.end())  
   {  
       auto eventInfo = std::dynamic_pointer_cast<EventInfo>(it->second);  
       if (eventInfo)  
           eventInfo->actions.Remove(obj, method);  
   }  
}  

// オブジェクトのメソッドをイベントリスナーから削除する関数（引数あり）  
template<typename... Args, typename Obj>  
void EventCenter::RemoveEventListener(E_EventType eventName, Obj* obj, void (Obj::* method)(Args...))  
{  
   EventKey key{ eventName, typeid(std::tuple<Args...>) };  
   auto it = eventDic.find(key);  
   if (it != eventDic.end())  
   {  
       auto eventInfo = std::dynamic_pointer_cast<EventInfoT<Args...>>(it->second);  
       if (eventInfo)  
           eventInfo->actions.Remove(obj, method);  
   }  
}