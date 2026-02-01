#pragma once

#include <memory>
#include <string_view>

namespace Edge
{
	class Window
	{
	public:
		Window(const std::string& title, int w, int h);
		~Window();
		Window(const Window&) = delete;
		Window(Window&&) noexcept = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) noexcept = delete;
	public:
		bool ShouldClose() const noexcept;
		bool DidResize() const noexcept;
	public:
		void Update();
	private:
		class Impl;
		std::unique_ptr<Impl> m_impl;
	};
}
