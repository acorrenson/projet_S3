cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tsp C)

# set include path
include_directories(
  ../include/
)

# declare library TSPLIB
add_library(tsplib
  tsplib/tsplib_io.c
  tsplib/tsplib_tools.c)

target_link_libraries(tsplib
  m # math library
)


# declare the executable
add_executable(Tsp tsp.c)

target_link_libraries(Tsp
  tsplib
)