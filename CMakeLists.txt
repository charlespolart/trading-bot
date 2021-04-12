cmake_minimum_required(VERSION 3.2)

project(ema)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(EMA_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Params.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Coin.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Database.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/EMACross.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Server.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Tools.hpp
)

set(EMA_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Coin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Database.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/EMACross.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Server.cpp
)

add_subdirectory(binapi build)
