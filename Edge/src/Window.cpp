#include <Edge\PCH.h>
#include <Edge\Window.h>
#include <Edge\Win32.h>

namespace Edge
{
	struct WindowStateFlags
	{
		bool should_close;
		bool did_resize;
	};

	class Window::Impl
	{
	public:
		static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	public:
		Impl(const std::string& title, int w, int h);
		~Impl() = default;
		Impl(const Impl&) = delete;
		Impl(Impl&&) noexcept = delete;
		Impl& operator=(const Impl&) = delete;
		Impl& operator=(Impl&&) noexcept = delete;
	public:
		const WindowStateFlags& Flags() const noexcept { return m_flags; }
	public:
		void Update();
	private:
		WindowStateFlags m_flags;
		Win32::WindowClass m_class;
		Win32::Window m_window;
	};

	LRESULT Window::Impl::WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		LRESULT result{};
		auto window{ reinterpret_cast<Window::Impl*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA)) };

		switch (msg)
		{
		case WM_CREATE:
		{
			// save pointer passed when creating the win32 window as the window user data
			LPCREATESTRUCT create{ reinterpret_cast<LPCREATESTRUCT>(lparam) };
			SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create->lpCreateParams));
		} break;
		case WM_DESTROY:
		{
			if (window)
			{
				window->m_flags.should_close = true;
			}
		} break;
		case WM_SIZE:
		{
			if (window)
			{
				window->m_flags.did_resize = true;
			}
		} break;
		default:
		{
			result = DefWindowProcA(hwnd, msg, wparam, lparam);
		} break;
		}

		return result;
	}


	Window::Impl::Impl(const std::string& title, int w, int h)
		: m_flags{}
		, m_class{ title, WindowProcedure }
		, m_window{ WS_EX_OVERLAPPEDWINDOW, title, title, WS_OVERLAPPEDWINDOW | WS_VISIBLE, w, h, this } // TODO: hardcoded style
	{
	}
	void Window::Impl::Update()
	{
		// reset window state flags
		m_flags = {};

		// dispatch pendign window messages
		MSG msg{};
		while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	Window::Window(const std::string& title, int w, int h)
		: m_impl{ std::make_unique<Impl>(title, w, h) }
	{
	}
	Window::~Window()
	{
		// do nothing (needed for PIMPL)
	}
	void Window::Update()
	{
		m_impl->Update();
	}
	bool Window::ShouldClose() const noexcept
	{
		return m_impl->Flags().should_close;
	}
	bool Window::DidResize() const noexcept
	{
		return m_impl->Flags().did_resize;
	}
}
