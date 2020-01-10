# =======================================
# == INIT ZONE 
# =======================================

cmake_minimum_required(VERSION 3.7)

# set the project name
project(Tsp C)

# set include path
include_directories(
  ../include/
)

# =======================================
# == LIBRARY ZONE 
# =======================================

# declare library TSPLIB
add_library(tsplib
  tsplib/tsplib_io.c
  tsplib/tsplib_tools.c)

target_link_libraries(tsplib
  m # math library
)

# =======================================
# == EXECUTABLE ZONE 
# =======================================

file(GLOB methods "./methods/*.c")

# declare the executable
add_executable(Tsp 
  utils.c
  cli.c
  ${methods}
  tsp.c
)

target_link_libraries(Tsp
  tsplib
)

# =======================================
# == TEST ZONE
# =======================================

# declare library TESTINGLIB
add_library(testinglib 
  ../test/testing.c
)

# find test files
file(GLOB tests "../test/*.test.c")

# data for tests
file(COPY ../test/test.tsp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ../data/att10.tsp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# main module to run every tests
add_executable(Test
  ${tests}
  ./utils.c
  ${methods}
  ../test/all_test.c
)

# link modules needed for tests
target_link_libraries(Test
  tsplib
  testinglib
)
