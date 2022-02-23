#include "../../Engine/Headers/Logger.hpp"

namespace NeuroEvolution {

	std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;

	void Logger::Init() 
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_EngineLogger = spdlog::stdout_color_mt("NeuroEvolution");
		s_EngineLogger->set_level(spdlog::level::trace);
	}
}