#include <Edge\PCH.h>
#include <Edge\Win32.h>
#include <Edge\Crashes.h>
#include <Edge\Registry.h>
#include <Edge\Logger.h>

namespace Edge::Win32
{
    void WarnIfNotSuccess(HRESULT hr, const std::string_view msg)
    {
        if (!SUCCEEDED(hr))
        {
            Registry::Get<Logger>()->Warn(msg);
        }
    }

	WindowClass::WindowClass(std::string_view name, WNDPROC window_procedure)
		: m_name{name}
	{
		WNDCLASSEXA desc{};
        desc.cbSize = sizeof(desc);
        desc.style = CS_HREDRAW | CS_VREDRAW;
        desc.lpfnWndProc = window_procedure;
        //desc.cbClsExtra = ;
        //desc.cbWndExtra = ;
        desc.hInstance = GetModuleHandleA(NULL);
        desc.hIcon = LoadIconA(NULL, IDI_APPLICATION);
        desc.hCursor = LoadCursorA(NULL, IDC_ARROW);
        //desc.hbrBackground = ;
        //desc.lpszMenuName = ;
        desc.lpszClassName = m_name.c_str();
        desc.hIconSm = LoadIconA(NULL, IDI_APPLICATION);

        // register window class
        ATOM success{ RegisterClassExA(&desc) };
        Edge_AssertMsg(success, "Failed to register Win32 window class");
	}
	WindowClass::~WindowClass()
	{
		UnregisterClassA(m_name.c_str(), GetModuleHandleA(NULL));
	}

    Window::Window(DWORD ex_style, const std::string& clazz, const std::string& name, DWORD style, int client_w, int client_h, void* param)
        : m_handle{}
    {
        // compute actual window dimensions from client dimensions and style
        int w{}, h{};
        {
            RECT rect{ 0, 0, client_w, client_h };
            BOOL success{ AdjustWindowRectEx(&rect, style, FALSE, ex_style) };
            Edge_AssertMsg(success, "Failed to compute Win32 window dimensions from client dimensions and style");
            w = rect.right - rect.left;
            h = rect.bottom - rect.top;
        }

        // create window
        m_handle = CreateWindowExA(ex_style, clazz.c_str(), name.c_str(), style, CW_USEDEFAULT, CW_USEDEFAULT, w, h, NULL, NULL, GetModuleHandleA(NULL), param);
        Edge_AssertMsg(m_handle, "Failed to create Win32 window");
    }
    Window::~Window()
    {
        DestroyWindow(m_handle); // NOTE: may fail
    }
}
