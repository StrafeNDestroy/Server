cmake_minimum_required(VERSION 3.22.1)
project(server)
find_package( OpenCV REQUIRED )


set(
SOURCES
includes/Sourcefiles/local_server.cpp
includes/Sourcefiles/http_request.cpp
${OpenCV_INCLUDE_DIRS}
)


set(
LIBS
/usr/include/opencv/build/x64/vc16/lib
)

include_directories(
    includes/Headers
    includes/Sourcefiles
    usr/include/opencv/build/include   
)
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} )
