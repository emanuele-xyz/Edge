#pragma once

#include <memory>

namespace Edge::Gfx
{
	class Device
	{
	public:
		Device(void* hwnd);
		~Device();
		Device(const Device&) = delete;
		Device(Device&&) noexcept = delete;
		Device& operator=(const Device&) = delete;
		Device& operator=(Device&&) noexcept = delete;
	private:
		class Impl;
		std::unique_ptr<Impl> m_impl;
	};
}
