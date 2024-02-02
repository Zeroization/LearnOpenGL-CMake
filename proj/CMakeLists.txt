# Ö÷ÏîÄ¿ÅäÖÃ
add_executable(LearnOpenGL "./src/main.cpp")
target_include_directories(LearnOpenGL PUBLIC "include")
target_link_libraries(LearnOpenGL PUBLIC glfw glad stb_image)
target_compile_features(LearnOpenGL PUBLIC cxx_std_20)
target_precompile_headers(LearnOpenGL PUBLIC "./include/pch.hpp")