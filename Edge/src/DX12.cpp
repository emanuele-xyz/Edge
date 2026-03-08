#include <Edge\PCH.h>
#include <Edge\DX12.h>
#include <Edge\Registry.h>
#include <Edge\Logger.h>

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#if defined(_DEBUG)
#pragma comment(lib, "dxguid")
#endif
#pragma comment(lib, "d3dcompiler") // TODO: do we need it

namespace Edge::DX12
{
	static const char* D3DFeatureLevelToStr(D3D_FEATURE_LEVEL lvl)
	{
		const char* str{};
		switch (lvl)
		{
		case D3D_FEATURE_LEVEL_1_0_GENERIC: { str = "D3D_FEATURE_LEVEL_1_0_GENERIC"; } break;
		case D3D_FEATURE_LEVEL_1_0_CORE: { str = "D3D_FEATURE_LEVEL_1_0_CORE"; } break;
		case D3D_FEATURE_LEVEL_9_1: { str = "D3D_FEATURE_LEVEL_9_1"; } break;
		case D3D_FEATURE_LEVEL_9_2: { str = "D3D_FEATURE_LEVEL_9_2"; } break;
		case D3D_FEATURE_LEVEL_9_3: { str = "D3D_FEATURE_LEVEL_9_3"; } break;
		case D3D_FEATURE_LEVEL_10_0: { str = "D3D_FEATURE_LEVEL_10_0"; } break;
		case D3D_FEATURE_LEVEL_10_1: { str = "D3D_FEATURE_LEVEL_10_1"; } break;
		case D3D_FEATURE_LEVEL_11_0: { str = "D3D_FEATURE_LEVEL_11_0"; } break;
		case D3D_FEATURE_LEVEL_11_1: { str = "D3D_FEATURE_LEVEL_11_1"; } break;
		case D3D_FEATURE_LEVEL_12_0: { str = "D3D_FEATURE_LEVEL_12_0"; } break;
		case D3D_FEATURE_LEVEL_12_1: { str = "D3D_FEATURE_LEVEL_12_1"; } break;
		case D3D_FEATURE_LEVEL_12_2: { str = "D3D_FEATURE_LEVEL_12_2"; } break;
		default: { Edge_Unreachable(); } break;
		}
		return str;
	}
	static const char* D3D12DescriptorTypeToStr(D3D12_DESCRIPTOR_HEAP_TYPE type)
	{
		const char* str{};
		switch (type)
		{
		case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV: { str = "D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV"; } break;
		case D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER: { str = "D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER"; } break;
		case D3D12_DESCRIPTOR_HEAP_TYPE_RTV: { str = "D3D12_DESCRIPTOR_HEAP_TYPE_RTV"; } break;
		case D3D12_DESCRIPTOR_HEAP_TYPE_DSV: { str = "D3D12_DESCRIPTOR_HEAP_TYPE_DSV"; } break;
		case D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES: { str = "D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES"; } break;
		default: { Edge_Unreachable(); } break;
		}
		return str;
	}
	static const char* D3D12GetCommandListTypeStr(D3D12_COMMAND_LIST_TYPE type)
	{
		const char* str{};
		switch (type)
		{
		case D3D12_COMMAND_LIST_TYPE_DIRECT: { str = "D3D12_COMMAND_LIST_TYPE_DIRECT"; } break;
		case D3D12_COMMAND_LIST_TYPE_BUNDLE: { str = "D3D12_COMMAND_LIST_TYPE_BUNDLE"; } break;
		case D3D12_COMMAND_LIST_TYPE_COMPUTE: { str = "D3D12_COMMAND_LIST_TYPE_COMPUTE"; } break;
		case D3D12_COMMAND_LIST_TYPE_COPY: { str = "D3D12_COMMAND_LIST_TYPE_COPY"; } break;
		case D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE: { str = "D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE"; } break;
		case D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS: { str = "D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS"; } break;
		case D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE: { str = "D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE"; } break;
		case D3D12_COMMAND_LIST_TYPE_NONE: { str = "D3D12_COMMAND_LIST_TYPE_NONE"; } break;
		default: { Edge_Unreachable(); } break;
		}
		return str;
	}
	static const char* D3D12GetFenceFlagsStr(D3D12_FENCE_FLAGS flags)
	{
		const char* str{};
		switch (flags)
		{
		case D3D12_FENCE_FLAG_NONE: { str = "D3D12_FENCE_FLAG_NONE"; } break;
		case D3D12_FENCE_FLAG_SHARED: { str = "D3D12_FENCE_FLAG_SHARED"; } break;
		case D3D12_FENCE_FLAG_SHARED_CROSS_ADAPTER: { str = "D3D12_FENCE_FLAG_SHARED_CROSS_ADAPTER"; } break;
		case D3D12_FENCE_FLAG_NON_MONITORED: { str = "D3D12_FENCE_FLAG_NON_MONITORED"; } break;
		default: { Edge_Unreachable(); } break;
		}
		return str;
	}

	DescriptorHeap::DescriptorHeap(ID3D12Device14* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT size, BOOL shader_visible)
		: m_heap{ CreateDescriptorHeap(device, type, size, shader_visible) }
		, m_descriptor_size{ device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) }
	{
	}
	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::At(UINT index)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE start{ m_heap->GetCPUDescriptorHandleForHeapStart() };
		return D3D12_CPU_DESCRIPTOR_HANDLE{ start.ptr + index * m_descriptor_size };
	}

	void EnableDXGIDebug()
	{
		HRESULT hr{ S_OK };
		wrl::ComPtr<IDXGIInfoQueue> debug{};
		hr = DXGIGetDebugInterface1(0, IID_PPV_ARGS(debug.ReleaseAndGetAddressOf()));
		Win32::WarnIfNotSuccess(hr, "Failed to get DXGI info queue");
		if (SUCCEEDED(hr))
		{
			hr = debug->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, TRUE);
			Win32::WarnIfNotSuccess(hr, "Failed to set debug break on DXGI and D3D corruptions");
			hr = debug->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, TRUE);
			Win32::WarnIfNotSuccess(hr, "Failed to set debug break on DXGI and D3D errors");
			hr = debug->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING, TRUE);
			Win32::WarnIfNotSuccess(hr, "Failed to set debug break on DXGI and D3D warnings");
		}

	}
	void ReportDXGIAndD3DLiveCOMObjects()
	{
		HRESULT hr{ S_OK };
		wrl::ComPtr<IDXGIDebug1> debug{};
		hr = DXGIGetDebugInterface1(0, IID_PPV_ARGS(debug.ReleaseAndGetAddressOf()));
		Win32::WarnIfNotSuccess(hr, "Failed to get DXGI debug interface");
		if (SUCCEEDED(hr))
		{
			hr = debug->ReportLiveObjects(DXGI_DEBUG_ALL, static_cast<DXGI_DEBUG_RLO_FLAGS>(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
			Win32::WarnIfNotSuccess(hr, "Failed to report DXGI and D3D live COM objects");
		}
	}
	void EnableD3D12Debug()
	{
		wrl::ComPtr<ID3D12Debug6> debug{};
		HRESULT hr{ D3D12GetDebugInterface(IID_PPV_ARGS(debug.ReleaseAndGetAddressOf())) };
		Win32::WarnIfNotSuccess(hr, "Failed to get D3D12 debug interface");
		if (SUCCEEDED(hr))
		{
			debug->EnableDebugLayer();
			//debug->SetEnableAutoName
			//debug->SetEnableGPUBasedValidation
		}
	}
	wrl::ComPtr<IDXGIFactory7> CreateDXGIFactory()
	{
		UINT flags{};
		#if defined(_DEBUG)
		flags |= DXGI_CREATE_FACTORY_DEBUG;
		#endif
		wrl::ComPtr<IDXGIFactory7> factory;
		HRESULT hr{ CreateDXGIFactory2(flags, IID_PPV_ARGS(factory.ReleaseAndGetAddressOf())) };
		EdgeWin32_AssertHRMsg(hr, "Failed to create DXGI factory");
		return factory;
	}
	wrl::ComPtr<IDXGIAdapter4> GetDXGIAdapter(IDXGIFactory7* factory)
	{
		wrl::ComPtr<IDXGIAdapter4> adapter{};
		HRESULT hr{ factory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf())) };
		EdgeWin32_AssertHRMsg(hr, "Failed to get DXGI adapter");
		return adapter;
	}
	wrl::ComPtr<ID3D12Device14> CreateD3D12Device(IUnknown* adapter)
	{
		wrl::ComPtr<ID3D12Device14> device{};
		D3D_FEATURE_LEVEL required_feature_level{ D3D_FEATURE_LEVEL_12_2 };
		HRESULT hr{ D3D12CreateDevice(adapter, required_feature_level, IID_PPV_ARGS(device.ReleaseAndGetAddressOf())) };
		EdgeWin32_AssertHRMsgFmt(hr, "Failed to create D3D12 device with for feature level {}", D3DFeatureLevelToStr(required_feature_level));
		return device;
	}
	wrl::ComPtr<ID3D12CommandQueue> CreateD3D12CommandQueue(ID3D12Device14* device, const D3D12_COMMAND_QUEUE_DESC& desc)
	{
		wrl::ComPtr<ID3D12CommandQueue> queue;
		HRESULT hr{ device->CreateCommandQueue(&desc, IID_PPV_ARGS(queue.ReleaseAndGetAddressOf())) };
		EdgeWin32_AssertHRMsg(hr, "Failed to create D3D12 command queue"); // TODO: convert desc to string and output it
		return queue;
	}
	wrl::ComPtr<ID3D12CommandQueue> CreateD3D12DirectQueue(ID3D12Device14* device)
	{
		D3D12_COMMAND_QUEUE_DESC desc{};
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		//desc.Priority = ;
		//desc.Flags = ;
		//desc.NodeMask = ;
		return DX12::CreateD3D12CommandQueue(device, desc);
	}
	wrl::ComPtr<ID3D12CommandQueue> CreateD3D12CopyQueue(ID3D12Device14* device)
	{
		D3D12_COMMAND_QUEUE_DESC desc{};
		desc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
		//desc.Priority = ;
		//desc.Flags = ;
		//desc.NodeMask = ;
		return DX12::CreateD3D12CommandQueue(device, desc);
	}
	wrl::ComPtr<ID3D12CommandQueue> CreateD3D12ComputeQueue(ID3D12Device14* device)
	{
		D3D12_COMMAND_QUEUE_DESC desc{};
		desc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
		//desc.Priority = ;
		//desc.Flags = ;
		//desc.NodeMask = ;
		return DX12::CreateD3D12CommandQueue(device, desc);
	}
	wrl::ComPtr<IDXGISwapChain4> CreateDXGISwapChain(IDXGIFactory7* factory, ID3D12CommandQueue* direct_queue, HWND hwnd, UINT buffer_count)
	{
		DXGI_SWAP_CHAIN_DESC1 desc{};
		//desc.Width = 0;
		//desc.Height = 0;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // TODO: sRGB
		//desc.Stereo = ;
		desc.SampleDesc = DXGI_SAMPLE_DESC{ .Count = 1, .Quality = 0 };
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = buffer_count;
		desc.Scaling = DXGI_SCALING_NONE;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		//desc.Flags = ;

		HRESULT hr{ S_OK };
		wrl::ComPtr<IDXGISwapChain1> swap_chain1{};
		hr = factory->CreateSwapChainForHwnd(direct_queue, hwnd, &desc, NULL, NULL, swap_chain1.ReleaseAndGetAddressOf());
		EdgeWin32_AssertHRMsg(hr, "Failed to create DXGI swap chain");
		wrl::ComPtr<IDXGISwapChain4> swap_chain4{};
		hr = swap_chain1->QueryInterface(swap_chain4.ReleaseAndGetAddressOf());
		EdgeWin32_AssertHRMsg(hr, "Failed to query for latest DXGI swap chain interface");

		// Disable Alt+Enter full screen transitions
		hr = factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
		Win32::WarnIfNotSuccess(hr, "Failed to disable Alt+Enter full screen transitions");

		return swap_chain4;
	}
	wrl::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ID3D12Device14* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT size, BOOL shader_visible)
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.Type = type;
		desc.NumDescriptors = size;
		desc.Flags = shader_visible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		//UINT desc.NodeMask = ;
		wrl::ComPtr<ID3D12DescriptorHeap> heap{};
		HRESULT hr{ device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(heap.ReleaseAndGetAddressOf())) };
		EdgeWin32_AssertHRMsgFmt(hr, "Failed to create D3D12 descriptor heap - type: {} - size: {} - is_shader_visible: {}", D3D12DescriptorTypeToStr(type), size, shader_visible);
		return heap;
	}
	wrl::ComPtr<ID3D12DescriptorHeap> CreateRTVHeap(ID3D12Device14* device, UINT size, BOOL shader_visible)
	{
		return CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, size, shader_visible);
	}
	wrl::ComPtr<ID3D12Resource> GetSwapChainBuffer(IDXGISwapChain4* swap_chain, UINT buffer_idx)
	{
		wrl::ComPtr<ID3D12Resource> res;
		HRESULT hr{ swap_chain->GetBuffer(buffer_idx, IID_PPV_ARGS(res.ReleaseAndGetAddressOf())) };
		EdgeWin32_AssertHRMsgFmt(hr, "Failed to get DXGI swap chain buffer {}", buffer_idx);
		return res;
	}
	wrl::ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(ID3D12Device14* device, D3D12_COMMAND_LIST_TYPE type)
	{
		wrl::ComPtr<ID3D12CommandAllocator> allocator{};
		HRESULT hr{ device->CreateCommandAllocator(type, IID_PPV_ARGS(allocator.ReleaseAndGetAddressOf())) };
		EdgeWin32_AssertHRMsgFmt(hr, "Failed to create D3D12 command allocator - type: {}", D3D12GetCommandListTypeStr(type));
		return allocator;
	}
	wrl::ComPtr<ID3D12CommandAllocator> CreateDirectCommandAllocator(ID3D12Device14* device)
	{
		return CreateCommandAllocator(device, D3D12_COMMAND_LIST_TYPE_DIRECT);
	}
	wrl::ComPtr<ID3D12GraphicsCommandList> CreateGraphicsCommandList(ID3D12Device14* device, ID3D12CommandAllocator* allocator, D3D12_COMMAND_LIST_TYPE type, bool closed)
	{
		wrl::ComPtr<ID3D12GraphicsCommandList> list{};
		HRESULT hr{ device->CreateCommandList(0, type, allocator, nullptr, IID_PPV_ARGS(list.ReleaseAndGetAddressOf())) };
		EdgeWin32_AssertHRMsgFmt(hr, "Failed to create D3D12 command list - allocator: {} - type: {}", static_cast<void*>(allocator), D3D12GetCommandListTypeStr(type));
		if (closed)
		{
			list->Close();
		}
		return list;
	}
	wrl::ComPtr<ID3D12GraphicsCommandList> CreateDirectGraphicsCommandList(ID3D12Device14* device, ID3D12CommandAllocator* allocator, bool closed)
	{
		return CreateGraphicsCommandList(device, allocator, D3D12_COMMAND_LIST_TYPE_DIRECT, closed);
	}
	wrl::ComPtr<ID3D12Fence> CreateFence(ID3D12Device14* device, UINT64 value, D3D12_FENCE_FLAGS flags)
	{
		wrl::ComPtr<ID3D12Fence> fence{};
		HRESULT hr{ device->CreateFence(value, flags, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf())) };
		EdgeWin32_AssertHRMsgFmt(hr, "Failed to create D3D12 fence - initial value: {} - flags: {}", value, D3D12GetFenceFlagsStr(flags));
		return fence;
	}
}
