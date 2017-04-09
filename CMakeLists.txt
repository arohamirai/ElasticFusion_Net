cmake_minimum_required(VERSION 2.4.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")

project(main)

find_package(OpenNI2 REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(
	${OPENNI2_INCLUDE_DIRS}
)


add_executable(main main.cpp)
target_link_libraries(main 
	${OPENNI2_LIBRARIES}
	${OpenCV_LIBS}
)
