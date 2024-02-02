# stb_image
add_library(stb_image INTERFACE)
add_compile_definitions(STB_IMAGE_IMPLEMENTATION STBI_WINDOWS_UTF8)
target_include_directories(stb_image INTERFACE "./")
target_compile_features(stb_image INTERFACE cxx_std_20)