#pragma once

#include <Edge\Win32.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#if defined(_DEBUG)
#include <dxgidebug.h>
#endif
#include <d3dcompiler.h> // TODO: do we need it?


namespace Edge::DX12
{
	void EnableDXGIDebug();
	void EnableD3D12Debug();
	void ReportDXGIAndD3DLiveCOMObjects();
	wrl::ComPtr<IDXGIFactory7> CreateDXGIFactory();
	wrl::ComPtr<IDXGIAdapter4> GetDXGIAdapter(IDXGIFactory7* factory);
	wrl::ComPtr<ID3D12Device14> CreateD3D12Device(IUnknown* adapter);
	wrl::ComPtr<ID3D12CommandQueue> CreateD3D12CommandQueue(ID3D12Device14* device, const D3D12_COMMAND_QUEUE_DESC& desc);
	wrl::ComPtr<ID3D12CommandQueue> CreateD3D12DirectQueue(ID3D12Device14* device);
	wrl::ComPtr<ID3D12CommandQueue> CreateD3D12CopyQueue(ID3D12Device14* device);
	wrl::ComPtr<ID3D12CommandQueue> CreateD3D12ComputeQueue(ID3D12Device14* device);
}
