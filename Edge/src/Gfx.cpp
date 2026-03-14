#include <Edge\PCH.h>
#include <Edge\Gfx.h>
#include <Edge\Utils.h>
#include <Edge\DX12.h>
#include <Edge\Registry.h>
#include <Edge\Logger.h>

namespace Edge::Gfx
{
	class Handle::Impl
	{
	public:
		Impl(void* hwnd);
		~Impl() = default;
		Impl(const Impl&) = delete;
		Impl(Impl&&) noexcept = delete;
		Impl& operator=(const Impl&) = delete;
		Impl& operator=(Impl&&) noexcept = delete;
	private:
		// TODO: pass frame count as input
		constexpr static UINT FRAME_COUNT{ 2 };
		#if defined(_DEBUG)
		// Note: Must be the first member, because we want it to be destructed last.
		// Otherwise the other members of the class will be reported as leaking.
		OnDestruct<void(void)> m_report_live_objects_on_destruct{ DX12::ReportDXGIAndD3DLiveCOMObjects };
		OnConstruct m_enable_dxgi_debug_on_construct{ DX12::EnableDXGIDebug };
		OnConstruct m_enable_d3d_debug_on_construct{ DX12::EnableD3D12Debug };
		#endif
		wrl::ComPtr<IDXGIFactory7> m_dxgi_factory;
		wrl::ComPtr<IDXGIAdapter4> m_adapter;
		DXGI_ADAPTER_DESC3 m_adapter_desc;
		wrl::ComPtr<ID3D12Device14> m_device;
		wrl::ComPtr<ID3D12CommandQueue> m_direct_command_queue;
		wrl::ComPtr<IDXGISwapChain4> m_swap_chain;
		DX12::ResourceStateTracker m_resource_state_tracker;
		DX12::DescriptorHeap m_rtv_heap_frame;
		wrl::ComPtr<ID3D12Resource> m_swap_chain_buffer[FRAME_COUNT];
		wrl::ComPtr<ID3D12CommandAllocator> m_direct_command_allocator;
		wrl::ComPtr<ID3D12GraphicsCommandList> m_direct_command_list;
		UINT64 m_fence_value;
		wrl::ComPtr<ID3D12Fence> m_fence;
		Win32::Event m_fence_event;
	};
	Handle::Impl::Impl(void* hwnd)
		: m_dxgi_factory{ DX12::CreateDXGIFactory() }
		, m_adapter{ DX12::GetDXGIAdapter(m_dxgi_factory.Get()) }
		, m_adapter_desc{}
		, m_device{ DX12::CreateD3D12Device(m_adapter.Get()) }
		, m_direct_command_queue{ DX12::CreateD3D12DirectQueue(m_device.Get()) }
		, m_swap_chain{ DX12::CreateDXGISwapChain(m_dxgi_factory.Get(), m_direct_command_queue.Get(), static_cast<HWND>(hwnd), FRAME_COUNT) }
		, m_resource_state_tracker{}
		, m_rtv_heap_frame{ m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, FRAME_COUNT, FALSE }
		, m_swap_chain_buffer{}
		, m_direct_command_allocator{ DX12::CreateDirectCommandAllocator(m_device.Get()) }
		, m_direct_command_list{ DX12::CreateDirectGraphicsCommandList(m_device.Get(), m_direct_command_allocator.Get()) }
		, m_fence_value{}
		, m_fence{ DX12::CreateFence(m_device.Get(), m_fence_value++) }
		, m_fence_event{ FALSE, FALSE }
	{
		// Fetch and log adapter description
		HRESULT hr{ S_OK };
		hr = m_adapter->GetDesc3(&m_adapter_desc);
		Win32::WarnIfNotSuccess(hr, "Failed to get DXGI adapter description");
		if (SUCCEEDED(hr))
		{
			Registry::Get<Logger>()->Info("Selected DXGI adapter: {}", WCharToString(m_adapter_desc.Description));
		}

		// Fetch swap chain buffers and start tracking their resource states
		for (UINT i{}; i < FRAME_COUNT; i++)
		{
			wrl::ComPtr<ID3D12Resource> buf{ DX12::GetSwapChainBuffer(m_swap_chain.Get(), i) };
			m_swap_chain_buffer[i] = buf;
			m_resource_state_tracker.TrackResource(buf.Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON);
		}

		// Create swap chain buffers RTVs
		for (UINT i{}; i < FRAME_COUNT; i++)
		{
			ID3D12Resource* buf{ m_swap_chain_buffer[i].Get() };
			m_device->CreateRenderTargetView(buf, nullptr, m_rtv_heap_frame.At(i));
		}
	}

	void CommandList::ClearRenderTarget(RenderTarget /*render_target*/, v4 /*color*/)
	{
		// TODO: to be implemented
	}

	Handle::Handle(void* hwnd)
		: m_impl{ std::make_unique<Impl>(hwnd) }
	{
	}
	Handle::~Handle()
	{
		// do nothing (needed for PIMPL)
	}
	CommandList Handle::GetCommandList(CommandListType type)
	{
		// TODO: to be implemented
		return CommandList{ type };
	}
	void Handle::SubmitCommandList(CommandList /*list*/)
	{
		// TODO: to be implemented
	}
	RenderTarget Handle::GetCurrentSwapChainBufferRenderTarget()
	{
		// TODO: to be implemented
		return RenderTarget{};
	}
	void Handle::Present(bool /*vsync*/)
	{
		// TODO: to be implemented
	}
}