#pragma once

#include <Edge/Crashes.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <wrl/client.h>
namespace wrl = Microsoft::WRL;

#include <format>
#include <string>
#include <string_view>

#define EdgeWin32_AssertHRMsg(hr, msg) Edge_AssertMsg(SUCCEEDED(hr), msg)
#define EdgeWin32_AssertHRMsgFmt(hr, fmt, ...) EdgeWin32_AssertHRMsg(hr, std::format((fmt), __VA_ARGS__))
#define EdgeWin32_AssertHR(hr) EdgeWin32_AssertHRMsg(hr, "HRESULT is not successful: " #hr)

namespace Edge::Win32
{
	// TODO: we can do better than this bullshit
	void WarnIfNotSuccess(HRESULT hr, const std::string_view msg);

	class Event
	{
	public:
		Event(BOOL manual_reset, BOOL initial_state, SECURITY_ATTRIBUTES* security_attributes = NULL, const char* name = NULL);
		~Event();
		Event(const Event&) = delete;
		Event(Event&&) noexcept = delete;
		Event& operator=(const Event&) = delete;
		Event& operator=(Event&&) noexcept = delete;
	private:
		HANDLE m_event;
	};

	class WindowClass
	{
	public:
		WindowClass(std::string_view name, WNDPROC window_procedure);
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass(WindowClass&&) noexcept = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		WindowClass& operator=(WindowClass&&) noexcept = delete;
	private:
		std::string m_name;
	};

	class Window
	{
	public:
		Window(DWORD ex_style, const std::string& clazz, const std::string& name, DWORD style, int client_w, int client_h, void* param);
		~Window();
		Window(const Window&) = delete;
		Window(Window&&) noexcept = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) noexcept = delete;
	public:
		HWND Handle() const noexcept { return m_handle; }
	private:
		HWND m_handle;
	};
}
