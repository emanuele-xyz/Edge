#pragma once

#include <functional>
#include <utility>
#include <type_traits>

namespace Edge
{
	class OnConstruct
	{
	public:
		template <typename Callable>
		explicit OnConstruct(Callable&& func)
		{
			std::invoke(std::forward<Callable>(func));
		}
		~OnConstruct() = default;
		OnConstruct(const OnConstruct&) = delete;
		OnConstruct(OnConstruct&&) noexcept = delete;
		OnConstruct& operator=(const OnConstruct&) = delete;
		OnConstruct& operator=(OnConstruct&&) noexcept = delete;
	};


	template <typename Callable>
	class OnDestruct
	{
	public:
		template <typename U>
		explicit OnDestruct(U&& func)
			: m_func{ std::forward<U>(func) }
		{
		}
		~OnDestruct()
		{
			try
			{
				std::invoke(std::move(m_func));
			}
			catch (...)
			{
				// Swallow the exception
			}
		}
		OnDestruct(const OnDestruct&) = delete;
		OnDestruct(OnDestruct&&) noexcept = delete;
		OnDestruct& operator=(const OnDestruct&) = delete;
		OnDestruct& operator=(OnDestruct&&) noexcept = delete;
	private:
		std::decay_t<Callable> m_func;
	};

	template <typename U>
	OnDestruct(U&&) -> OnDestruct<std::decay_t<U>>;

	std::string WCharToString(const wchar_t* wstr);
}
