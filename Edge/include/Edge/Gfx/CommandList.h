#pragma once

#include <Edge/Gfx/CommandListType.h>
#include <Edge/Gfx/RenderTarget.h>

#include <Edge/Math.h>

namespace Edge::Gfx
{
	class CopyCommands
	{
	public:
		void CopyBufferRegion(/*TODO: args*/);
	};

	class ComputeCommands
	{
	public:
		void Dispatch(/*TODO: args*/);
	};

	class GraphicsCommands
	{
	public:
		void ClearRenderTarget(RenderTarget render_target, v4 color);
	};

	class CommandList
	{
	private:

	};

	class CopyCommandList : public CopyCommands
	{
	};

	class ComputeCommandList : public ComputeCommands
	{
	};

	class MainCommandList : public CopyCommands, public ComputeCommands, public GraphicsCommands
	{
	};

	class CopyCommandList
	{
	public:
	};

	class ComputeCommandList
	{

	};


	class CommandList
	{
	public:
		explicit CommandList(CommandListType type) noexcept : m_type{ type }, m_list{} {}
		CommandList(CommandListType type, void* list) noexcept : m_type{ type }, m_list{ list } {}
	public:
		void ClearRenderTarget(RenderTarget render_target, v4 color);
	private:
		CommandListType m_type;
		void* m_list;
	};
}
