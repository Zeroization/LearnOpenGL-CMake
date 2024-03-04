cmake_minimum_required(VERSION 3.27)

project(
	LearnOpenGL-Tests
	LANGUAGES CXX C
	DESCRIPTION "测试一些功能"
	VERSION 0.0.1
)

file(GLOB_RECURSE TESTS_HEADERS "include/*.h" "include/*.hpp")
file(GLOB_RECURSE TESTS_SOURCES "src/*.cpp")

add_executable(LearnOpenGL-Tests "${TESTS_SOURCES}")

target_include_directories(LearnOpenGL-Tests PUBLIC "./include")
target_link_libraries(LearnOpenGL-Tests  
	PUBLIC LearnOpenGL-Core
)
target_precompile_headers(LearnOpenGL-Tests PUBLIC "../LearnOpenGL-Core/include/pch.hpp")
target_compile_features(LearnOpenGL-Tests PUBLIC cxx_std_20)
target_compile_definitions(LearnOpenGL-Tests 
	PRIVATE PROJ_RES_PATH="${CMAKE_SOURCE_DIR}/Res"
	PRIVATE SPDLOG_USE_STD_FORMAT
	PRIVATE SPDLOG_FMT_EXTERNAL
	PRIVATE FMT_HEADER_ONLY
)