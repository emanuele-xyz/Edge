#pragma once

#include <format>
#include <string_view>
#include <utility>

namespace Edge
{
	enum class LogLevel
	{
		Debug,
		Info,
		Warn,
		Error,
		Critical,
	};

	class Logger
	{
	public:
		Logger() noexcept;
		~Logger() = default;
		Logger(const Logger&) = delete;
		Logger(Logger&&) noexcept = delete;
		Logger& operator=(const Logger&) = delete;
		Logger& operator=(Logger&&) noexcept = delete;
	public:
		void Log(LogLevel lvl, std::string_view msg) noexcept;
		void Debug(std::string_view msg) noexcept    { Log(LogLevel::Debug, msg);    }
		void Info(std::string_view msg) noexcept     { Log(LogLevel::Info, msg);     }
		void Warn(std::string_view msg) noexcept     { Log(LogLevel::Warn, msg);     }
		void Error(std::string_view msg) noexcept    { Log(LogLevel::Error, msg);    }
		void Critical(std::string_view msg) noexcept { Log(LogLevel::Critical, msg); }
	public:
		template<typename... Args>
		void Log(LogLevel lvl, std::format_string<Args...> fmt, Args&&... args) noexcept
		{
			try
			{
				Log(lvl, std::format(fmt, std::forward<Args>(args)...));
			}
			catch (...)
			{
				// if logging failed, do nothing
			}
		}
		template<typename... Args>
		void Debug(std::format_string<Args...> fmt, Args&&... args) noexcept
		{ 
			Log(LogLevel::Debug, fmt, std::forward<Args>(args)...); 
		}
		template<typename... Args>
		void Info(std::format_string<Args...> fmt, Args&&... args) noexcept
		{ 
			Log(LogLevel::Info, fmt, std::forward<Args>(args)...); 
		}
		template<typename... Args>
		void Warn(std::format_string<Args...> fmt, Args&&... args) noexcept
		{ 
			Log(LogLevel::Warn, fmt, std::forward<Args>(args)...);
		}
		template<typename... Args>
		void Error(std::format_string<Args...> fmt, Args&&... args) noexcept
		{ 
			Log(LogLevel::Error, fmt, std::forward<Args>(args)...);
		}
		template<typename... Args>
		void Critical(std::format_string<Args...> fmt, Args&&... args) noexcept
		{ 
			Log(LogLevel::Critical, fmt, std::forward<Args>(args)...);
		}
	};
}
