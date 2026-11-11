cmake_minimum_required(VERSION 3.16)
project(mybench LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# La librairie, header-only (commentaire avec # cette fois !)
add_library(mybench INTERFACE)
target_include_directories(mybench INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Le programme d'exemple
add_executable(demo examples/demo.cpp)
target_link_libraries(demo PRIVATE mybench)
target_compile_options(demo PRIVATE -O2 -Wall -Wextra)

# Programme 2 : la demonstration de DoNotOptimize
add_executable(demonstration examples/demonstration.cpp)
target_link_libraries(demonstration PRIVATE mybench)
target_compile_options(demonstration PRIVATE -O2 -Wall -Wextra)

# Programme 3 : benchmark de commande externe
add_executable(benchcmd examples/benchcmd.cpp)
target_link_libraries(benchcmd PRIVATE mybench)
target_compile_options(benchcmd PRIVATE -O2 -Wall -Wextra)
