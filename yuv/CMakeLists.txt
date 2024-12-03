cmake_minimum_required(VERSION 3.10)

FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

#获取代码，在项目中，将所有代码都放在src文件夹中
AUX_SOURCE_DIRECTORY(src DIR_SRCS)
MESSAGE(STATUS "Src file: ${DIR_SRCS}")
#添加链接库
SET(PROJECT_NAME YUV)
#建立项目
PROJECT(${PROJECT_NAME})

add_executable(YUV yuv.cpp)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OpenCV_LIBS})
