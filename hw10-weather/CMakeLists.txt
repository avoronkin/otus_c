cmake_minimum_required(VERSION 3.15)
project(hw10-weather C)

set(CMAKE_C_STANDARD 11)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror -g)
endif()

add_subdirectory("lib/curl")
add_subdirectory("lib/parson")

include_directories(
    "lib/curl"
    "lib/parson"
)

set(SOURCES
    "src/config.c"
    "src/fetch.c"
    "src/metaweather_locations.c"
    "src/metaweather_forecasts.c"
    "src/main.c"
)

add_executable(weather ${SOURCES})
target_link_libraries(weather libcurl parson)
