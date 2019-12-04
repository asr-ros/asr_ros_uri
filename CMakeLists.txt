cmake_minimum_required(VERSION 2.4.6)
project(asr_ros_uri)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  roslib
)

find_package(Boost REQUIRED COMPONENTS
  system
  filesystem
)

include_directories(include
${catkin_INCLUDE_DIRS}
    ${EIGEN_INCLUDE_DIRS}
    ${PCL_INCLUDE_DIRS})

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp roslib
  DEPENDS Boost
)

add_executable(${PROJECT_NAME} src/test.cpp)

target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
)

install(DIRECTORY include/
	DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION})
