#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>
#include <string_view>

namespace Edge
{
	class Win32WindowClass
	{
	public:
		Win32WindowClass(std::string_view name, WNDPROC window_procedure);
		~Win32WindowClass();
		Win32WindowClass(const Win32WindowClass&) = delete;
		Win32WindowClass(Win32WindowClass&&) noexcept = delete;
		Win32WindowClass& operator=(const Win32WindowClass&) = delete;
		Win32WindowClass& operator=(Win32WindowClass&&) noexcept = delete;
	private:
		std::string m_name;
	};

	class Win32Window
	{
	public:
		Win32Window(DWORD ex_style, const std::string& clazz, const std::string& name, DWORD style, int client_w, int client_h, void* param);
		~Win32Window();
		Win32Window(const Win32Window&) = delete;
		Win32Window(Win32Window&&) noexcept = delete;
		Win32Window& operator=(const Win32Window&) = delete;
		Win32Window& operator=(Win32Window&&) noexcept = delete;
	private:
		HWND m_handle;
	};
}
