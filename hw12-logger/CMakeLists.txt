cmake_minimum_required(VERSION 3.16)
project(hw12_logger C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -Werror -rdynamic")

add_library(logger logger.c)

add_executable(example example.c)
target_link_libraries(example logger)
