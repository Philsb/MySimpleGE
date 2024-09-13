cmake_minimum_required(VERSION 3.16)
project(MySimpleGE)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization
set(SDL_STATIC ON)

add_subdirectory(Thirdparty/SDL-release-2.30.5)		#window opener
add_subdirectory(Thirdparty/glad)			#opengl loader
add_subdirectory(Thirdparty/stb_image)			#loading images
add_subdirectory(Thirdparty/stb_truetype)		#loading ttf files
add_subdirectory(Thirdparty/glm)			#math
add_subdirectory(Thirdparty/imgui-docking)		#ui

add_library(MySimpleGE)
target_sources(MySimpleGE PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/MySimpleGE/init.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Source/MySimpleGE/openglErrorReporting.cpp")
target_include_directories(MySimpleGE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source/")
target_link_libraries(MySimpleGE PUBLIC glm glad stb_image stb_truetype imgui SDL2-static)
