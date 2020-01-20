cmake_minimum_required (VERSION 3.16)

project (uring_example C)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_C_STANDARD 99)
link_libraries(uring)
set_property(SOURCE acosw.S PROPERTY LANGUAGE C)
add_executable(main main.c lib.c aco.c acosw.S)
