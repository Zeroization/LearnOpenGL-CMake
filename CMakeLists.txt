cmake_minimum_required(VERSION 3.27)

# 为 MSVC 编译器启用热重载。
if(POLICY CMP0141)
	cmake_policy(SET CMP0141 NEW)
	set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# 识别utf-8
if (WIN32)
    add_definitions(/utf-8)
endif()

project(
	LearnOpenGL
	LANGUAGES CXX C
	DESCRIPTION "用CMake构建的LearnOpenGL"
	VERSION 0.0.1
)

# 第三方依赖库
add_subdirectory("./Dependencies")
# Core
add_subdirectory("./LearnOpenGL-Core")
# Tests
add_subdirectory("./LearnOpenGL-Tests")