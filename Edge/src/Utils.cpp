#include <Edge/PCH.h>
#include <Edge/Utils.h>
#include <Edge/Crashes.h>
#include <Edge/Win32.h>

namespace Edge
{
	std::string WCharToString(const wchar_t* wstr)
	{
		if (!wstr) return std::string{};
		int size{ WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr) };
		std::string res(size - 1, 0);
		int written{ WideCharToMultiByte(CP_UTF8, 0, wstr, -1, res.data(), size, nullptr, nullptr) };
		Edge_AssertMsg(size == written, "Error converting from wchar string to char string");
		return res;
	}
}
