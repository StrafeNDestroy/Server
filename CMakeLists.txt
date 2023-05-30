cmake_minimum_required(VERSION 3.22.1)
project(server)

set(
SOURCES
local_server.cpp
 
)

include_directories(
    includes/Headers
    includes/Sourcefiles   
)
add_executable(${PROJECT_NAME} ${SOURCES})