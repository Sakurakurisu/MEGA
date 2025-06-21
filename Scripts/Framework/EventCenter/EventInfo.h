#pragma once

#include "DelegateAction.h"

class EventInfoBase
{
public:
	virtual ~EventInfoBase() = default;
};

template<typename... Args>
class EventInfoT : public EventInfoBase
{
public:
	DelegateAction<Args...> actions;

	EventInfoT() = default;

	EventInfoT(const std::function<void(Args...)>& action)
	{
		actions.Add(action);
	}
};

class EventInfo : public EventInfoBase
{
public:
	DelegateAction<> actions;

	EventInfo() = default;

	EventInfo(const std::function<void()>& action)
	{
		actions.Add(action);
	}
};