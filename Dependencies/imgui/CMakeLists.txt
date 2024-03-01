cmake_minimum_required(VERSION 3.27)

file(GLOB_RECURSE ALL_SOURCES *.cpp)
file(GLOB_RECURSE ALL_HEADERS *.h)

message(${ALL_SOURCES})

add_library(imgui
	${ALL_SOURCES} ${ALL_HEADERS}
)
target_include_directories(imgui PUBLIC .)
target_link_libraries(imgui PRIVATE glfw)