cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(Server)

find_package(PCL 1.2 REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

FIND_PACKAGE(Boost REQUIRED COMPONENTS serialization)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ADD_DEFINITIONS( "-DHAS_BOOST" )


add_executable (Server config.xml Main.cpp XMLConfig.cpp KinectViewer.cpp CommandMaker.cpp)
target_link_libraries (Server ${PCL_LIBRARIES} ${QT_LIBRARIES} ${Boost_LIBRARIES})
