# CMakeList.txt : CMake project for NeuroEvolutionEngine, include source and define
# project specific logic here.
#

# Recursive Search: Headers and Source Files
file(GLOB_RECURSE SRC_FILES Engine/Src/*.cpp)
file(GLOB_RECURSE HEADER_FILES Engine/Headers/*.hpp)
set(PRE_COMPILED_HEADERS Engine/Headers/pch.hpp)

# Add source to this project's executable.
add_executable (${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

# TODO: Add tests and install targets if needed.

# Pre Compiled Header Target File
target_precompile_headers(${PROJECT_NAME} PUBLIC ${PRE_COMPILED_HEADERS})

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)

# Add Target spdlog lib
add_subdirectory(third_party_libs/spdlog)
target_include_directories(${PROJECT_NAME} PUBLIC third_party_libs/spdlog/include)

