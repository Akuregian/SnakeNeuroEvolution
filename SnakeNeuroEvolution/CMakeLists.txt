# CMakeList.txt : CMake project for NeuroEvolutionEngine, include source and define
# project specific logic here.
#

# Recursive Search: Headers and Source Files in the Engine
file(GLOB_RECURSE ENGINE_SRC_FILES "Engine/Source/*.cpp")
file(GLOB_RECURSE ENGINE_HEADER_FILES "Engine/Headers/*.hpp")

# Recursive Search: Headers and Source Files in the Game
file(GLOB_RECURSE GAME_SRC_FILES "Game/Source/*.cpp")
file(GLOB_RECURSE GAME_HEADER_FILES "Game/Headers/*.hpp")

set(PRE_COMPILED_HEADERS "Engine/Headers/pch.hpp")

# Add source to this project's executable.
add_executable (${PROJECT_NAME} ${ENGINE_SRC_FILES} ${ENGINE_HEADER_FILES} ${GAME_SRC_FILES} ${GAME_HEADER_FILES})

# TODO: Add tests and install targets if needed.

# Pre Compiled Header Target File
target_precompile_headers(${PROJECT_NAME} PUBLIC ${PRE_COMPILED_HEADERS})

# Add Target Eigen library
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)

# Add Target spdlog library
add_subdirectory(third_party_libs/spdlog)
target_include_directories(${PROJECT_NAME} PUBLIC "third_party_libs/spdlog/include")

# Add Target sfml library 
set(BUILD_SHARED_LIBS FALSE)
add_subdirectory("third_party_libs/sfml")
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window)
