// Logging Wrapper for spdlog

#pragma once
#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"

namespace NeuroEvolution {

	class Logger {
	public:
		static void Init();
		static inline std::shared_ptr<spdlog::logger>& GetEngineLogger() { return s_EngineLogger; };

	private:
		static std::shared_ptr<spdlog::logger> s_EngineLogger;
	};
}

#define ENGINE_RESULTS_LOGGER(...) NeuroEvolution::Logger::GetEngineLogger()->warn(__VA_ARGS__)
