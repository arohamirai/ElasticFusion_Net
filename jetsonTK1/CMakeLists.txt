cmake_minimum_required(VERSION 2.4.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")

project(jetsonTK1)

find_package(OpenNI2 REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(
	${OPENNI2_INCLUDE_DIRS}
)


add_executable(jetsonTK1 main.cpp)
target_link_libraries(jetsonTK1 
	${OPENNI2_LIBRARIES}
	${OpenCV_LIBS}
)
