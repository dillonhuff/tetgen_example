cmake_minimum_required(VERSION 2.6)

project(tetgen_ex)

SET(EXTRA_CXX_COMPILE_FLAGS "-std=c++11 -I./src -I./test -I/opt/local/include -I/Users/dillon/Downloads/tetgen1.5.1-beta1 -O2 -Werror -Wall")

SET(CXX_OCL_LINK_FLAGS "-ltet -L/Users/dillon/Downloads/tetgen1.5.1-beta1")
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${CXX_OCL_LINK_FLAGS}")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_COMPILE_FLAGS}")

INCLUDE_DIRECTORIES(./)

SET(CPP_FILES main.cpp parse_stl.cpp)

add_executable(main ${CPP_FILES})

# Make command:
# /opt/local/bin/cmake -DVTK_DIR:PATH=/Users/dillon/Downloads/VTK-build/ -DCMAKE_BUILD_TYPE=Debug . 
