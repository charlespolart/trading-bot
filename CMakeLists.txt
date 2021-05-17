cmake_minimum_required(VERSION 3.2)

project(trading_bot)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(TRADING_BOT_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Params.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ATR.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Coin.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Database.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/EMA.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/EMACross.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/FetchUsers.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Indicators.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/RSI.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Server.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Tools.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/User.hpp
)

set(TRADING_BOT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ATR.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Coin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Database.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/EMA.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/EMACross.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Indicators.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RSI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/User.cpp
)

add_subdirectory(binapi build)
