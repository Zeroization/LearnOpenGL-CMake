cmake_minimum_required(VERSION 3.27)

file(GLOB ROOT_SOURCES *.cpp)
file(GLOB ROOT_HEADERS *.h)
file(GLOB_RECURSE BACKENDS_SOURCES ./backends/*.cpp)
file(GLOB_RECURSE BACKENDS_HEADERS ./backends/*.cpp)
file(GLOB_RECURSE MISC_SOURCES ./misc/*.cpp)
file(GLOB_RECURSE MISC_HEADERS ./misc/*.cpp)

add_library(imgui STATIC
	${ROOT_SOURCES} ${ROOT_HEADERS}
	${BACKENDS_SOURCES} ${BACKENDS_HEADERS}
	${MISC_SOURCES} ${MISC_HEADERS}
	./thirdParty/FileBrowser/ImGuiFileDialog.cpp
    ./thirdParty/FileBrowser/ImGuiFileDialog.h
    ./thirdParty/FileBrowser/ImGuiFileDialogConfig.h
)
target_include_directories(imgui PUBLIC .)
target_link_libraries(imgui PUBLIC glfw)
target_compile_features(imgui PRIVATE cxx_std_20)
