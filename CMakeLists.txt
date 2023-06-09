cmake_minimum_required(VERSION 3.22.1)
project(server)
find_package( OpenCV REQUIRED )


set(
SOURCES
includes/Sourcefiles/local_server.cpp
includes/Sourcefiles/http_request.cpp
${OpenCV_INCLUDE_DIRS}
)
set(CMAKE_BUILD_TYPE Debug)


set(
LIBS
)

include_directories(
    includes/Headers
    includes/Sourcefiles
    /usr/include/cppcodec
    
    
)
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} )
