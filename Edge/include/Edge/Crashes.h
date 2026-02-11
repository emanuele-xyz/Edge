#pragma once

#include <format>
#include <stdexcept>
#include <string>
#include <string_view>

#define Edge_Crash(msg) throw ::Edge::Crash{__FILE__, __LINE__, msg}
#define Edge_Unreachable() Edge_Crash("Unreachable code path")
#define Edge_AssertMsg(p, msg) do { if (!(p)) Edge_Crash(msg); } while (false)
#define Edge_AssertMsgFmt(p, fmt, ...) Edge_AssertMsg(p, std::format((fmt), __VA_ARGS__))
#define Edge_Assert(p) Edge_AssertMsg(p, "Assertion failed: " #p)

namespace Edge
{
	class Crash : public std::runtime_error
	{
	public:
		Crash(const std::string_view file, int line, const std::string& msg) :
			std::runtime_error{ std::format("{}({}): {}", file, line, msg) }
		{}
	};
}
