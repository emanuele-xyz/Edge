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
	constexpr static const char* D3DFeatureLevelToStr(D3D_FEATURE_LEVEL lvl)
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
		}
		return str;
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
		EdgeWin32_AssertHRMsg(hr, std::format("Failed to create D3D12 device with for feature level {}", D3DFeatureLevelToStr(required_feature_level)));
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
}

// TODO: look for asserts with std::format inside and create new macros Edge_AssertMsgFmt or something like that
