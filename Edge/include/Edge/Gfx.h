#pragma once

#include <memory>

namespace Edge
{
	/*
		Graphics module
	*/
	class Gfx
	{
	public:
		Gfx(void* hwnd);
		~Gfx();
		Gfx(const Gfx&) = delete;
		Gfx(Gfx&&) noexcept = delete;
		Gfx& operator=(const Gfx&) = delete;
		Gfx& operator=(Gfx&&) noexcept = delete;
	private:
		class Impl;
		std::unique_ptr<Impl> m_impl;
	};
}
