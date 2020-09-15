cmake_minimum_required(VERSION 3.17)
project(MP0_Distributed_Log_Querier)

set(CMAKE_CXX_STANDARD 14)

add_executable(MP0_Distributed_Log_Querier main.cpp include/client.h include/server.h include/grep.h src/grep.cpp src/run_client.cpp src/run_server.cpp)