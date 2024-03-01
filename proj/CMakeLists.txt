cmake_minimum_required(VERSION 3.27)

file(GLOB_RECURSE cpps "src/*.cpp")

# Ö÷ÏîÄ¿ÅäÖÃ
add_executable(LearnOpenGL "${cpps}")

target_include_directories(LearnOpenGL PUBLIC "include")

target_link_libraries(LearnOpenGL PUBLIC 
	glfw 
	glad
	glm
	imgui
	spdlog
	stb_image
)

target_precompile_headers(LearnOpenGL PUBLIC "./include/pch.hpp")
target_compile_features(LearnOpenGL PUBLIC cxx_std_20)
target_compile_definitions(LearnOpenGL 
	PRIVATE PROJ_RES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res"
	PRIVATE SPDLOG_USE_STD_FORMAT
	PRIVATE SPDLOG_FMT_EXTERNAL
	PRIVATE FMT_HEADER_ONLY
)