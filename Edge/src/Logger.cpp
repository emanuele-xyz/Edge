#include <Edge\PCH.h>
#include <Edge\Logger.h>

#define FMT_UNICODE 0
#include <spdlog/spdlog.h>

namespace Edge
{
	Logger::Logger() noexcept
	{
		// Set spdlog pattern
		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");

		// Log also debug level messages for debug builds
		#if defined(_DEBUG)
		spdlog::set_level(spdlog::level::debug);
		#endif
	}
	void Logger::Log(LogLevel lvl, std::string_view msg) noexcept
	{
		try
		{
			switch (lvl)
			{
			case LogLevel::Debug: { spdlog::debug(msg); } break;
			case LogLevel::Info: { spdlog::info(msg); } break;
			case LogLevel::Warn: { spdlog::warn(msg); } break;
			case LogLevel::Error: { spdlog::error(msg); } break;
			case LogLevel::Critical: { spdlog::critical(msg); } break;
			}
		}
		catch (...)
		{
			// If logging failed, do nothing
		}
	}
}