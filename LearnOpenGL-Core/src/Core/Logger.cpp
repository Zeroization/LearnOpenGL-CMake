#include "Core/Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace GLCore
{
	std::shared_ptr<spdlog::logger> Logger::s_logger;

	void Logger::init()
	{
		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		// auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("runtime.log", true);

		spdlog::logger logger("", {console_sink /*, file_sink */});
		s_logger = std::make_shared<spdlog::logger>(logger);
		s_logger->set_level(spdlog::level::trace);
	}
}