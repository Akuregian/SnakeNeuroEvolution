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

#define ENABLE_DEBUG 0
#define ENABLE_INIT_DEBUG 0

// Step for each process of the code without viewing whats been initialized

#if ENABLE_DEBUG == 1
	#define ENGINE_LOGGER_INFO(...)     NeuroEvolution::Logger::GetEngineLogger()->info(__VA_ARGS__)
	#define ENGINE_LOGGER_ERROR(...)    NeuroEvolution::Logger::GetEngineLogger()->error(__VA_ARGS__)
	#define ENGINE_LOGGER_WARN(...)     NeuroEvolution::Logger::GetEngineLogger()->warn(__VA_ARGS__)
	#define ENGINE_LOGGER_CRITICAL(...) NeuroEvolution::Logger::GetEngineLogger()->critical(__VA_ARGS__)
#else
	#define ENGINE_LOGGER_INFO
	#define ENGINE_LOGGER_ERROR
	#define ENGINE_LOGGER_WARN
	#define ENGINE_LOGGER_CRITICAL
#endif

// Views whats been initliazed or Destryoed
#if ENABLE_INIT_DEBUG == 1
	#define ENGINE_INIT_INFO(...)     NeuroEvolution::Logger::GetEngineLogger()->info(__VA_ARGS__)
	#define ENGINE_INIT_ERROR(...)    NeuroEvolution::Logger::GetEngineLogger()->error(__VA_ARGS__)
	#define ENGINE_INIT_WARN(...)     NeuroEvolution::Logger::GetEngineLogger()->warn(__VA_ARGS__)
	#define ENGINE_INIT_CRITICAL(...) NeuroEvolution::Logger::GetEngineLogger()->critical(__VA_ARGS__)
#else
	#define ENGINE_INIT_INFO
	#define ENGINE_INIT_ERROR
	#define ENGINE_INIT_WARN 
	#define ENGINE_INIT_CRITICAL
#endif

#define ENGINE_RESULTS_LOGGER(...) NeuroEvolution::Logger::GetEngineLogger()->warn(__VA_ARGS__)
