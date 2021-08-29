cmake_minimum_required(VERSION 3.12)
project(Project VERSION 1.0.0)
enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Simulazioni consts.cpp classes_sir.cpp space_random.cpp functions_random.cpp functions_mains.cpp main.cpp)
target_compile_options(Simulazioni PRIVATE -Wall -Wextra -lasan)

add_executable(Automa consts.cpp classes_sir.cpp space_random.cpp functions_mains.cpp random_evolution.cpp)
target_compile_options(Automa PRIVATE -Wall -Wextra -lasan)
