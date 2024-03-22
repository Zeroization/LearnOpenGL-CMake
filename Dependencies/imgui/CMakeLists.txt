cmake_minimum_required(VERSION 3.27)

file(GLOB_RECURSE IMGUI_SOURCES *.cpp)
file(GLOB_RECURSE IMGUI_HEADERS *.h *.hpp)

add_library(imgui STATIC
	${IMGUI_SOURCES} ${IMGUI_HEADERS}
)
target_include_directories(imgui PUBLIC ../imgui/)
target_link_libraries(imgui PUBLIC glfw)
target_compile_features(imgui PRIVATE cxx_std_20)
