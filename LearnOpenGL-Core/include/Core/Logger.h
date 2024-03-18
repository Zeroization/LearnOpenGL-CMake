#pragma once

#include <spdlog/spdlog.h>

namespace GLCore
{
	class Logger
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getLogger() { return s_logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_logger;
	};
}

#define LOG_TRACE(...)		GLCore::Logger::getLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)		GLCore::Logger::getLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)       GLCore::Logger::getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)       GLCore::Logger::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)      GLCore::Logger::getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)   GLCore::Logger::getLogger()->critical(__VA_ARGS__)
