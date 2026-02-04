#pragma once

#include <mutex>
#include <typeindex>
#include <unordered_map>

namespace Edge
{
	class Registry
	{
	public:
		template<typename T>
		[[nodiscard]] static bool Add(T* service)
		{
			// service must not be null
			bool success{ service != nullptr };
			if (success)
			{
				// get registry instance and lock mutex
				auto& inst{ Instance() };
				std::lock_guard lock{ inst.m_mutex };
				// register service
				auto result{ inst.m_services.try_emplace(typeid(T), service) };
				success = result.second;
			}
			return success;
		}
		template<typename T>
		static T* Get()
		{
			// get registry instance and lock mutex
			auto& inst{ Instance() };
			std::lock_guard lock{ inst.m_mutex };
			// fetch requested service, if any
			auto it{ inst.m_services.find(typeid(T)) };
			return static_cast<T*>(it != inst.m_services.end() ? it->second : nullptr);
		}
	private:
		static Registry& Instance()
		{
			static Registry singleton{};
			return singleton;
		}
	private:
		Registry() = default;
		~Registry() = default;
		Registry(const Registry&) = delete;
		Registry(Registry&&) noexcept = delete;
		Registry& operator=(const Registry&) = delete;
		Registry& operator=(Registry&&) noexcept = delete;
	private:
		std::mutex m_mutex;
		std::unordered_map<std::type_index, void*> m_services;
	};
}
