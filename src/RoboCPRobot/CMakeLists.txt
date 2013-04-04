cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(Robot)

add_executable (Robot
	Controller.h Controller.cpp
	Received.h Received.cpp IReceived.h IReceived.cpp
	ReceivedBuffer.h ReceivedBuffer.cpp
	SenderBase.h SenderBase.cpp
	ClientReceiver.h ClientReceiver.cpp
	
	ArduCopterController.h ArduCopterController.cpp
	ArduCopterReceived.h ArduCopterReceived.cpp
	ArduCopterBuffer.h ArduCopterBuffer.cpp
	
	NanoController.h NanoController.cpp
	NanoReceived.h NanoReceived.cpp
	NanoReceivedBuffer.h NanoReceivedBuffer.cpp

	Send.h Send.cpp
	SendBuffer.h SendBuffer.cpp
	SendSender.h SendSender.cpp
	
	KinectController.h KinectController.cpp
	KinectData.h KinectData.cpp
	KinectBuffer.h KinectBuffer.cpp
	KinectSender.h KinectSender.cpp

	XMLConfig.h XMLConfig.cpp
	
	main.cpp)

target_link_libraries (Robot ${PCL_LIBRARIES} ${Boost_LIBRARIES})
