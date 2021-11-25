cmake_minimum_required(VERSION 3.15)
project(hw10-weather C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -Werror")

find_package(CURL REQUIRED COMPONENTS HTTP HTTPS )
include_directories(${CURL_INCLUDE_DIRS})
set(METAWEATHER_DEPS ${METAWEATHER_DEPS} ${CURL_LIBRARIES})

add_subdirectory("contrib/parson")
include_directories("contrib/parson")
set(METAWEATHER_DEPS ${METAWEATHER_DEPS} parson)

add_library(metaweather
    "src/metaweather/forecasts.c"
    "src/metaweather/http.c"
    "src/metaweather/locations.c"
    )
target_link_libraries(metaweather ${METAWEATHER_DEPS})
target_include_directories(metaweather PUBLIC src/metaweather)
set_target_properties(metaweather PROPERTIES PUBLIC_HEADER src/metaweather/metaweather.h)

set(SOURCES
    "src/main.c"
    "src/config.c"
    "src/forecast.c"
)

add_executable(weather_cli ${SOURCES} )
target_link_libraries(weather_cli metaweather)
