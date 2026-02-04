#include <Edge\PCH.h>
#include <Edge\Gfx.h>
#include <Edge\DX12.h>
#include <Edge\Utils.h>
#include <Edge\Registry.h>
#include <Edge\Logger.h>

namespace Edge
{
	class ReportDXGIAndD3DLiveCOMObjectsOnDestruct
	{
	public:
		ReportDXGIAndD3DLiveCOMObjectsOnDestruct() = default;
		~ReportDXGIAndD3DLiveCOMObjectsOnDestruct() { DX12::ReportDXGIAndD3DLiveCOMObjects(); }
		ReportDXGIAndD3DLiveCOMObjectsOnDestruct(const ReportDXGIAndD3DLiveCOMObjectsOnDestruct&) = delete;
		ReportDXGIAndD3DLiveCOMObjectsOnDestruct(ReportDXGIAndD3DLiveCOMObjectsOnDestruct&&) noexcept = delete;
		ReportDXGIAndD3DLiveCOMObjectsOnDestruct& operator=(const ReportDXGIAndD3DLiveCOMObjectsOnDestruct&) = delete;
		ReportDXGIAndD3DLiveCOMObjectsOnDestruct& operator=(ReportDXGIAndD3DLiveCOMObjectsOnDestruct&&) noexcept = delete;
	};

	class Gfx::Impl
	{
	public:
		Impl();
		~Impl() = default;
		Impl(const Impl&) = delete;
		Impl(Impl&&) noexcept = delete;
		Impl& operator=(const Impl&) = delete;
		Impl& operator=(Impl&&) noexcept = delete;
	private:
		#if defined(_DEBUG)
		// Note: Must be the first member, because we want it to be destructed last.
		// Otherwise the other members of the class will be reported as leaking.
		ReportDXGIAndD3DLiveCOMObjectsOnDestruct m_on_destruct;
		#endif
		wrl::ComPtr<IDXGIFactory7> m_dxgi_factory;
		wrl::ComPtr<IDXGIAdapter4> m_adapter;
		DXGI_ADAPTER_DESC3 m_adapter_desc;
		wrl::ComPtr<ID3D12Device14> m_device;
		wrl::ComPtr<ID3D12CommandQueue> m_command_queue;
	};

	Gfx::Impl::Impl()
	{
		HRESULT hr{ S_OK };
		#if defined(_DEBUG)
		DX12::EnableDXGIDebug();
		DX12::EnableD3D12Debug();
		#endif
		m_dxgi_factory = DX12::CreateDXGIFactory();
		m_adapter = DX12::GetDXGIAdapter(m_dxgi_factory.Get());
		hr = m_adapter->GetDesc3(&m_adapter_desc);
		Win32::WarnIfNotSuccess(hr, "Failed to get DXGI adapter description");
		if (SUCCEEDED(hr))
		{
			Registry::Get<Logger>()->Info("Selected DXGI adapter: {}", WCharToString(m_adapter_desc.Description));
		}
		m_device = DX12::CreateD3D12Device(m_adapter.Get());
		m_command_queue = DX12::CreateD3D12DirectQueue(m_device.Get());

		// TODO: to be implemented
	}

	Gfx::Gfx()
		: m_impl{ std::make_unique<Impl>() }
	{
	}
	Gfx::~Gfx()
	{
		// do nothing (needed for PIMPL)
	}
}