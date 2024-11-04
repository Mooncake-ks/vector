cmake_minimum_required(VERSION 3.10)
project(Vector)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(Vector ${SRC_FILES})

# target_link_libraries(Vector ...) # Подключение библиотек
# target_include_directories(Vector ...) # Дополнительные пути для include
# target_compile_options(Vector ...) # Опции компиляции