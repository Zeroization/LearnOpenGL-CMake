cmake_minimum_required(VERSION 3.27)

project(
	LearnOpenGL-Core
	LANGUAGES CXX C
	DESCRIPTION "��װ��Ŀ���Ĺ���"
	VERSION 0.0.1
)

file(GLOB_RECURSE CORE_HEADERS "include/*.h" "include/*.hpp")
file(GLOB_RECURSE CORE_SOURCES "src/*.cpp")

add_library(LearnOpenGL-Core
	${CORE_HEADERS} ${CORE_SOURCES}
)

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
	PRIVATE PROJ_RES_PATH="${CMAKE_SOURCE_DIR}/Res"
	PRIVATE SPDLOG_USE_STD_FORMAT
	PRIVATE SPDLOG_FMT_EXTERNAL
	PRIVATE FMT_HEADER_ONLY
)