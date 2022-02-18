#include "../../Engine/Headers/Logger.hpp"

namespace NeuroEvolution {

	std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;
	std::shared_ptr<spdlog::logger> Logger::s_EngineInitLogger;

	void Logger::Init() 
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		
		s_EngineLogger = spdlog::stdout_color_mt("NeuroEvolution");
		s_EngineInitLogger = spdlog::stdout_color_mt("Initialized");

		s_EngineLogger->set_level(spdlog::level::trace);
		s_EngineInitLogger->set_level(spdlog::level::trace);

	}
}