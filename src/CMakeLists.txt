cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(RoboCP)

find_package(PCL 1.2 REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

FIND_PACKAGE(Boost REQUIRED COMPONENTS serialization)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ADD_DEFINITIONS( "-DHAS_BOOST" )

find_package( OpenCV REQUIRED )

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(RoboCPRobot)
INCLUDE_DIRECTORIES(RoboCPServer)
INCLUDE_DIRECTORIES(RoboCPCommon)

find_package(Qt5Core)

project(Robot)

add_executable (Robot
	RoboCPCommon/Controller.h RoboCPCommon/Controller.cpp
	RoboCPCommon/SenderBase.h RoboCPCommon/SenderBase.cpp
	RoboCPCommon/ReceivedBuffer.h RoboCPCommon/ReceivedBuffer.cpp
	RoboCPCommon/Command.h RoboCPCommon/Command.cpp

	RoboCPCommon/Send.h RoboCPCommon/Send.cpp
	RoboCPCommon/SendBuffer.h 
	
	RoboCPCommon/KinectData.h RoboCPCommon/KinectData.cpp
	RoboCPCommon/KinectBuffer.h 
	
	RoboCPCommon/JSONConfig.h RoboCPCommon/JSONConfig.cpp
	RoboCPCommon/XMLConfig.h RoboCPCommon/XMLConfig.cpp

	RoboCPCommon/Point3d.h

	RoboCPRobot/ArduCopterConfig.h RoboCPRobot/ArduCopterConfig.cpp
	RoboCPRobot/Config.h RoboCPRobot/Config.cpp
	
	RoboCPRobot/CommandProcessing.h RoboCPRobot/CommandProcessing.cpp
	RoboCPRobot/CommandBuffer.h

	RoboCPRobot/Received.h RoboCPRobot/Received.cpp 
	RoboCPRobot/IReceived.h RoboCPRobot/IReceived.cpp
	RoboCPRobot/ClientReceiver.h RoboCPRobot/ClientReceiver.cpp
	
	RoboCPRobot/ArduCopterController.h RoboCPRobot/ArduCopterController.cpp
	RoboCPRobot/ArduCopterReceived.h RoboCPRobot/ArduCopterReceived.cpp
	RoboCPRobot/ArduCopterBuffer.h 
	
	RoboCPRobot/NanoController.h RoboCPRobot/NanoController.cpp
	RoboCPRobot/NanoReceived.h RoboCPRobot/NanoReceived.cpp
	RoboCPRobot/NanoReceivedBuffer.h 

	RoboCPRobot/CameraController.h RoboCPRobot/CameraController.cpp
	RoboCPRobot/CameraReceived.h RoboCPRobot/CameraReceived.cpp
	RoboCPRobot/CameraReceivedBuffer.h
	
	RoboCPRobot/DisplacementImages.h RoboCPRobot/DisplacementImages.cpp
	RoboCPRobot/ImageFlowProcessing.h RoboCPRobot/ImageFlowProcessing.cpp

	RoboCPRobot/SendSender.h RoboCPRobot/SendSender.cpp
	RoboCPRobot/SendProcessing.h RoboCPRobot/SendProcessing.cpp

	RoboCPRobot/KinectController.h RoboCPRobot/KinectController.cpp
	RoboCPRobot/KinectSender.h RoboCPRobot/KinectSender.cpp
	RoboCPRobot/KinectDownsampler.h RoboCPRobot/KinectDownsampler.cpp

	RoboCPRobot/SerialCom.h RoboCPRobot/SerialCom.cpp 
	
	RoboCPRobot/main.cpp

)
target_link_libraries (Robot ${PCL_LIBRARIES} ${Boost_LIBRARIES} ${OpenCV_LIBS} ${Qt5Core_LIBRARIES})



project(Server)

add_executable (Server
	RoboCPCommon/Controller.h RoboCPCommon/Controller.cpp
	RoboCPCommon/SenderBase.h RoboCPCommon/SenderBase.cpp
	RoboCPCommon/ReceivedBuffer.h RoboCPCommon/ReceivedBuffer.cpp
	RoboCPCommon/Command.h RoboCPCommon/Command.cpp

	RoboCPCommon/Send.h RoboCPCommon/Send.cpp
	RoboCPCommon/SendBuffer.h 
	
	RoboCPCommon/KinectData.h RoboCPCommon/KinectData.cpp
	RoboCPCommon/KinectBuffer.h 

	RoboCPCommon/JSONConfig.h RoboCPCommon/JSONConfig.cpp

	RoboCPCommon/Point3d.h


	RoboCPRobot/DisplacementImages.h


	RoboCPServer/KinectViewer.h RoboCPServer/KinectViewer.cpp
	RoboCPServer/KinectReceiver.h RoboCPServer/KinectReceiver.cpp
	RoboCPServer/KinectManager.h RoboCPServer/KinectManager.cpp
	RoboCPServer/SendReceiver.h RoboCPServer/SendReceiver.cpp
	RoboCPServer/SendManager.h RoboCPServer/SendManager.cpp
	RoboCPServer/CommandMaker.h RoboCPServer/CommandMaker.cpp

	RoboCPServer/main.cpp

)
target_link_libraries (Server ${PCL_LIBRARIES} ${Boost_LIBRARIES} ${OpenCV_LIBS})



project(RoboCP)



