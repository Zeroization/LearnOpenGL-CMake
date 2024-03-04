cmake_minimum_required(VERSION 3.27)

project(
	LearnOpenGL-Core
	LANGUAGES CXX C
	DESCRIPTION "封装项目核心功能"
	VERSION 0.0.1
)

file(GLOB_RECURSE cpps "src/*.cpp")
# TODO: 这里可能要去掉add_executable
add_executable(LearnOpenGL-Core "${cpps}")

target_include_directories(LearnOpenGL-Core PUBLIC "include")
target_link_libraries(LearnOpenGL-Core PUBLIC 
	glfw 
	glad
	glm
	imgui
	spdlog::spdlog_header_only
	stb_image
)
target_precompile_headers(LearnOpenGL-Core PUBLIC "./include/pch.hpp")
target_compile_features(LearnOpenGL-Core PUBLIC cxx_std_20)
target_compile_definitions(LearnOpenGL-Core 
	PRIVATE PROJ_RES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res"
	PRIVATE SPDLOG_USE_STD_FORMAT
	PRIVATE SPDLOG_FMT_EXTERNAL
	PRIVATE FMT_HEADER_ONLY
)