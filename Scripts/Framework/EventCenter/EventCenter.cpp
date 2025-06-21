#include "EventCenter.h"

// イベントをトリガーする関数
void EventCenter::EventTrigger(E_EventType eventName)
{
   EventKey key{ eventName, typeid(std::tuple<>) };
   auto it = eventDic.find(key);
   if (it != eventDic.end())
   {
       // イベント情報を取得し、アクションを実行
       auto eventInfo = std::dynamic_pointer_cast<EventInfo>(it->second);
       if (eventInfo)
           eventInfo->actions.Invoke();
   }
}

// 全てのイベントをクリアする関数
void EventCenter::Clear()
{
   eventDic.clear();
}

// 特定のイベントタイプをクリアする関数
void EventCenter::Clear(E_EventType eventName)
{
   for (auto it = eventDic.begin(); it != eventDic.end(); )
   {
       if (it->first.type == eventName)
           it = eventDic.erase(it); // 該当イベントを削除
       else
           ++it; // 次の要素に進む
   }
}