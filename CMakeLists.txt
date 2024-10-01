cmake_minimum_required(VERSION 3.16)
project(MySimpleGE)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization
set(SDL_STATIC ON)

#! ! ! ! ! ! !
#set this to true to ship the game!
#basically this will change the RESOURCES_PATH to be the local path
#! ! ! ! ! ! !
#DELETE THE OUT FOLDER AFTER CHANGING THIS BECAUSE VISUAL STUDIO DOESN'T SEEM TO RECOGNIZE THIS CHANGE AND REBUILD!
option(PRODUCTION_BUILD "Make this a production build" OFF)
#DELETE THE OUT FOLDER AFTER CHANGING THIS BECAUSE VISUAL STUDIO DOESN'T SEEM TO RECOGNIZE THIS CHANGE AND REBUILD!

add_subdirectory(Thirdparty/SDL-release-2.30.5)		#window opener
add_subdirectory(Thirdparty/glad)			#opengl loader
add_subdirectory(Thirdparty/stb_image)			#loading images
add_subdirectory(Thirdparty/stb_truetype)		#loading ttf files
add_subdirectory(Thirdparty/glm)			#math
add_subdirectory(Thirdparty/imgui-docking)		#ui
add_subdirectory(Thirdparty/nlohmann_json-3.11.3)	

if(NOT DEFINED PRODUCTION_RES_PATH)
    set(PRODUCTION_RES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources/")
endif(NOT DEFINED PRODUCTION_RES_PATH)

if(NOT DEFINED RES_PATH)
    set(RES_PATH "./resources/")
endif(NOT DEFINED RES_PATH)

if(PRODUCTION_BUILD)
	# setup the ASSETS_PATH macro to be in the root folder of your exe
	add_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${PRODUCTION_RES_PATH}") 
	add_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1) 

else()
	# This is useful to get an ASSETS_PATH in your IDE during development
	add_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${RES_PATH}")
	add_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0) 

endif()


file(GLOB_RECURSE MY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")

add_library(MySimpleGE)
target_sources(MySimpleGE PRIVATE "${MY_SOURCES}")
target_include_directories(MySimpleGE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source/")
target_link_libraries(MySimpleGE PUBLIC glm glad stb_image stb_truetype imgui SDL2-static nlohmann_json)

