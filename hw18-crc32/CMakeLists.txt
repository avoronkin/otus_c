cmake_minimum_required(VERSION 3.16)
project(hw18_crc32 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -Werror")

add_executable(crc32 crc32.c)

add_executable(crc32_fgetc crc32_fgetc.c)
