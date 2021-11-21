cmake_minimum_required(VERSION 3.15)
project(hw10-weather C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} Wall -Wextra -pedantic -Werror")


add_subdirectory("lib/curl")
add_subdirectory("lib/parson")

include_directories(
    "lib/curl"
    "lib/parson"
)

set(SOURCES
    "src/config.c"
    "src/fetch.c"
    "src/metaweather.c"
    "src/forecast.c"
    "src/main.c"
)

add_executable(weather ${SOURCES})
target_link_libraries(weather libcurl parson)
