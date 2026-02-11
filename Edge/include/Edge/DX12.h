#pragma once

#include <Edge\Win32.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#if defined(_DEBUG)
#include <dxgidebug.h>
#endif
#include <d3dcompiler.h> // TODO: do we need it?

#include <d3dx12.h>

namespace Edge::DX12
{
	class DescriptorHeap
	{
	public:
		DescriptorHeap(ID3D12Device14* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT size, BOOL shader_visible);
		~DescriptorHeap() = default;
		DescriptorHeap(const DescriptorHeap&) = delete;
		DescriptorHeap(DescriptorHeap&&) noexcept = delete;
		DescriptorHeap& operator=(const DescriptorHeap&) = delete;
		DescriptorHeap& operator=(DescriptorHeap&&) noexcept = delete;
	public:
		D3D12_CPU_DESCRIPTOR_HANDLE At(UINT index);
	private:
		wrl::ComPtr<ID3D12DescriptorHeap> m_heap;
		UINT m_descriptor_size;
	};

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
	wrl::ComPtr<IDXGISwapChain4> CreateDXGISwapChain(IDXGIFactory7* factory, ID3D12CommandQueue* direct_queue, HWND hwnd, UINT buffer_count);
	wrl::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ID3D12Device14* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT size, BOOL shader_visible);
	wrl::ComPtr<ID3D12DescriptorHeap> CreateRTVHeap(ID3D12Device14* device, UINT size, BOOL shader_visible);
	wrl::ComPtr<ID3D12Resource> GetSwapChainBuffer(IDXGISwapChain4* swap_chain, UINT buffer_idx);
}
