#pragma once

#include <Edge\Win32.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#if defined(_DEBUG)
#include <dxgidebug.h>
#endif
#include <d3dcompiler.h> // TODO: do we need it?

#include <d3dx12.h>

#include <unordered_map>

namespace Edge::DX12
{
	class ResourceStateTracker
	{
	public:
		ResourceStateTracker() : m_resource_state_table{} {}
		~ResourceStateTracker() = default;
		ResourceStateTracker(const ResourceStateTracker&) = delete;
		ResourceStateTracker(ResourceStateTracker&&) noexcept = delete;
		ResourceStateTracker& operator=(const ResourceStateTracker&) = delete;
		ResourceStateTracker& operator=(ResourceStateTracker&&) noexcept = delete;
	public:
		void TrackResource(ID3D12Resource* resource, D3D12_RESOURCE_STATES state);
		void UntrackResource(ID3D12Resource* resource);
		void TransitionResource(ID3D12GraphicsCommandList* command_list, ID3D12Resource* resource, D3D12_RESOURCE_STATES new_state);
	private:
		struct ResourceStateInfo
		{
		public:
			ResourceStateInfo(D3D12_RESOURCE_STATES prev_state, D3D12_RESOURCE_STATES curr_state) : previous_state{ prev_state }, current_state{ curr_state } {}
			ResourceStateInfo(D3D12_RESOURCE_STATES state) : ResourceStateInfo{ state, state } {}
			ResourceStateInfo() : ResourceStateInfo{ D3D12_RESOURCE_STATE_COMMON } {};
		public:
			void Set(D3D12_RESOURCE_STATES new_state) { previous_state = current_state; current_state = new_state; }
		public:
			D3D12_RESOURCE_STATES previous_state;
			D3D12_RESOURCE_STATES current_state;
		};
	private:
		std::unordered_map<ID3D12Resource*, ResourceStateInfo> m_resource_state_table;
	};

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
	wrl::ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(ID3D12Device14* device, D3D12_COMMAND_LIST_TYPE type);
	wrl::ComPtr<ID3D12CommandAllocator> CreateDirectCommandAllocator(ID3D12Device14* device);
	wrl::ComPtr<ID3D12GraphicsCommandList> CreateGraphicsCommandList(ID3D12Device14* device, ID3D12CommandAllocator* allocator, D3D12_COMMAND_LIST_TYPE type, bool closed = true);
	wrl::ComPtr<ID3D12GraphicsCommandList> CreateDirectGraphicsCommandList(ID3D12Device14* device, ID3D12CommandAllocator* allocator, bool closed = true);
	wrl::ComPtr<ID3D12Fence> CreateFence(ID3D12Device14* device, UINT64 value, D3D12_FENCE_FLAGS flags = D3D12_FENCE_FLAG_NONE);
}
