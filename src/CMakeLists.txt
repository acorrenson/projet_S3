cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tsp C)

include_directories(
  ../include/
)

add_library(tsplib_io
  lecture_fichier_tsplib.c)

target_include_directories(tsplib_io
  PUBLIC
  ./
)

# add the executable
add_executable(Tsp tsp.c)  

target_link_libraries(Tsp
  tsplib_io
)