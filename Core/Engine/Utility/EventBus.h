#pragma once

#include <functional>
#include <typeindex>
#include <unordered_map>
#include <vector>

class EventBus {
public:
	template<typename Event>
	void Subscribe(std::function<void(const Event&)> handler) {

		m_handlers[ typeid(Event) ].push_back(
			[handler](const void* event) {
				handler( *static_cast<const Event*>(event) );
			}
		);
	}

	template<typename Event>
	void Publish(const Event& event) {

		auto it = m_handlers.find(typeid(Event));

		if( it == m_handlers.end() ) return;

		for( auto& handler : it->second ) {
			handler(&event);
		}
	}

private:
	using Handler = std::function<void(const void*)>;

	std::unordered_map<
		std::type_index,
		std::vector<Handler>
	> m_handlers;
};