#pragma once

#include <Edge/Gfx/CommandList.h>
#include <Edge/Gfx/CommandListType.h>
#include <Edge/Gfx/RenderTarget.h>

#include <memory>

namespace Edge::Gfx
{
	class Handle
	{
	public:
		Handle(void* hwnd);
		~Handle();
		Handle(const Handle&) = delete;
		Handle(Handle&&) noexcept = delete;
		Handle& operator=(const Handle&) = delete;
		Handle& operator=(Handle&&) noexcept = delete;
	public:
		CommandList GetCommandList(CommandListType type);
		void SubmitCommandList(CommandList list);
		RenderTarget GetCurrentSwapChainBufferRenderTarget();
		void Present(bool vsync);
	private:
		class Impl;
		std::unique_ptr<Impl> m_impl;
	};
}
