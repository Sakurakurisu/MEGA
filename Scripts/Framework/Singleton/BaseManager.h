#pragma once

template <typename T>
class BaseManager
{
protected:
	BaseManager() = default;
	~BaseManager() = default;

	BaseManager(const BaseManager&) = delete;
	BaseManager& operator=(const BaseManager&) = delete;

public:
	static T instance;
};

template <typename T>
T BaseManager<T>::instance;
