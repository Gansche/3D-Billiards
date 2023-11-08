cmake_minimum_required(VERSION 3.25)
project(Billiards)

include_directories(include)
link_directories(lib)
set(CMAKE_CXX_STANDARD 17)
aux_source_directory(src SRC)

add_executable(Billiards ${SRC})
target_link_libraries(Billiards glfw3.dll opengl32.lib)