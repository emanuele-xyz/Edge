#pragma once

#include <Edge/Gfx/CommandListType.h>
#include <Edge/Gfx/RenderTarget.h>

namespace Edge::Gfx
{
	class CommandList
	{
	public:
		explicit CommandList(CommandListType type) noexcept : m_type{ type }, m_list{} {}
		CommandList(CommandListType type, void* list) noexcept : m_type{ type }, m_list{ list } {}
	public:
		void ClearRenderTarget(RenderTarget render_target, );
	private:
		CommandListType m_type;
		void* m_list;
	};
}
