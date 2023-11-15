# version control
cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)

# project name
project(Billiards)

# set directories
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)
aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC)

# set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
file(COPY resources DESTINATION ${CMAKE_SOURCE_DIR}/bin)
file(COPY shaders DESTINATION ${CMAKE_SOURCE_DIR}/bin)

# generate executable
add_executable(Billiards ${SRC})
target_link_libraries(Billiards glfw3.dll opengl32.lib) # opengl32.dll may be useless