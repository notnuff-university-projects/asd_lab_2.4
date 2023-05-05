cmake_minimum_required(VERSION 3.25)
project(2_4 C)

set(CMAKE_C_STANDARD 11)

add_executable(2_4 main.c  "src/c files/graph_create.c"  "src/c files/vec_math_lib.c" "src/c files/X_routines.c" "src/c files/graph_draw.c")
target_link_libraries(2_4 X11)
target_link_libraries(2_4 m)
