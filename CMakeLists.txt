cmake_minimum_required(VERSION 3.10)
project(ConnectFourAI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(connect_four
    src/main.cpp
    src/Board.cpp
    src/AI.cpp
)