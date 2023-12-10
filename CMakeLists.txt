# version control
cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)

# project name
project(Billiards)

# set directories
include_directories(include)
link_directories(lib)
#aux_source_directory(src SRC)

# set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
file(COPY resources DESTINATION ${CMAKE_SOURCE_DIR}/bin)
file(COPY shaders DESTINATION ${CMAKE_SOURCE_DIR}/bin)

# generate executable
#add_executable(Billiards ${SRC})
add_executable(Billiards src/canvas.cpp src/game.cpp src/glad.c src/light.cpp
        src/main.cpp src/object.cpp src/scene.cpp src/shader.cpp src/stb_image.cpp
        src/table_manager.cpp src/texture.cpp src/camera.cpp src/program.cpp include/program.h)
target_link_libraries(Billiards glfw3.dll opengl32.lib) # opengl32.dll may be useless