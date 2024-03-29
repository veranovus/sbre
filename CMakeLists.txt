cmake_minimum_required(VERSION 3.20)
project(sbre C)

set(CMAKE_C_STANDART 99)

# OpenGL
include_directories(/opt/homebrew/Cellar/glew/2.2.0_1/include)
include_directories(/opt/homebrew/Cellar/glfw/3.3.6/include)
link_directories(/opt/homebrew/Cellar/glew/2.2.0_1/lib)
link_directories(/opt/homebrew/Cellar/glfw/3.3.6/lib)

# Freetype include directories
include_directories(/opt/homebrew/Cellar/freetype/2.11.1/include/freetype2)
link_directories(/opt/homebrew/Cellar/freetype/2.11.1/lib)

# Vendor include directories
include_directories(src/vendor)

# STB image files
file(GLOB STB_IMAGE_SOURCE src/vendor/stb_image/*.c)

# Library source files
file(GLOB SOURCE_FILES src/*.c src/debug/*c)

# Library include files
include_directories(src)

# We include our source files here
add_executable(sbre ${SOURCE_FILES} ${STB_IMAGE_SOURCE})

# Link the necessary libraries
find_package(OpenGL REQUIRED)
target_link_libraries(sbre OpenGL::GL)
find_package(glfw3 REQUIRED)
target_link_libraries(sbre glfw.3.3)
find_package(GLEW REQUIRED)
target_link_libraries(sbre GLEW)

# Link freetype
find_package(freetype REQUIRED)
target_link_libraries(sbre freetype)
