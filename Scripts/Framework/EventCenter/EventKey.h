#pragma once

#include <typeindex>
#include "E_EventType.h"

struct EventKey {
	E_EventType type{};
	std::type_index signature = std::type_index(typeid(void));

	bool operator==(const EventKey& other) const {
		return type == other.type && signature == other.signature;
	}
};

namespace std {
	template <>
	struct hash<EventKey> {
		std::size_t operator()(const EventKey& key) const {
			return std::hash<int>()(static_cast<int>(key.type)) ^ key.signature.hash_code();
		}
	};
}
