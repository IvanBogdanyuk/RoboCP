#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "pcl/compression/octree_pointcloud_compression.h"
#include <boost/property_tree/ptree.hpp>

//#define ENABLE_LOGGING

using namespace pcl;
using namespace pcl::octree;
using namespace std;


// Config class
class JSONConfig {
private:

	boost::property_tree::ptree ourTree; //Tree with "config.xml"


public:

	class ArducopterConfig {
	private:
		friend JSONConfig;
		ArducopterConfig();

	public:
		string Port;
		bool IsAvailable;
		bool DoFakeStart;
	};

	class SendConfig {
	private:
		friend JSONConfig;
		SendConfig();

	public:
		string Port;
		string IP;
		bool IsAvailable;
	};

	class CameraConfig {
	private:
		friend JSONConfig;
		CameraConfig();

	public:
		int	Number;
		int FramesPerSecond;
		int FrameWidth;
		int FrameHeight;
	};

	class CommandMakerConfig {
	private:
		friend JSONConfig;
		CommandMakerConfig();

	public:
		string	IP;
		string Port;
	};

	pcl::octree::compression_Profiles_e CompressionProfile;

	string IP;
	string CarduinoPort;
	string KinectPort;
	

	bool ShowStatistics;
	double PointResolution;
	double OctreeResolution;
	bool DoVoxelGridDownDownSampling;
	unsigned int IFrameRate;
	bool DoColorEncoding;
	unsigned int ColorBitResolution;

	CommandMakerConfig ConfigOfCommandMaker;
	SendConfig ConfigOfSend;
	ArducopterConfig ConfigOfArducopter;
	CameraConfig ConfigOfCamera;

	JSONConfig();
	void Parser(std::string fileName);
};