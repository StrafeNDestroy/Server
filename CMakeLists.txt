cmake_minimum_required(VERSION 3.22.1)
project(server)

set(
SOURCES
includes/Sourcefiles/local_server.cpp
includes/Sourcefiles/http_request.cpp


 
)

include_directories(
    includes/Headers
    includes/Sourcefiles   
)
add_executable(${PROJECT_NAME} ${SOURCES})