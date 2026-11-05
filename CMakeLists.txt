//projet, nous allons implementer notre propre benchmark, afin de, comprendre le system, low level, code c++, les api, le reseau, les timers, les processus, les interpretations...

cmake_minimum_required(VERSION 3.16)
project(mybench LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Notre librairie est "header-only" : elle ne vit que dans des .hpp.
# Une cible INTERFACE ne compile rien en soi, elle expose juste le dossier include/
# à ceux qui l'utilisent.
add_library(mybench INTERFACE)
target_include_directories(mybench INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Le programme d'exemple, qui utilise la librairie.
add_executable(demo examples/demo.cpp)
target_link_libraries(demo PRIVATE mybench)
target_compile_options(demo PRIVATE -O2 -Wall -Wextra)
