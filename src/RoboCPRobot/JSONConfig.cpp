#include "JSONConfig.h"
#include <boost/property_tree/json_parser.hpp>


JSONConfig::JSONConfig ()
{
	// default values
	ourTree.put("IP", "127.0.0.1");
	ourTree.put("KinectPort.Port", "6666");
	ourTree.put("CommandPort.Port", "6667");
	ourTree.put("Send.Port", "6668");
	ourTree.put("Send.IsAvailable", true);
	ourTree.put("CarduinoPort", "\\\\.\\COM3");
	ourTree.put("Arducopter.Port", "\\\\.\\COM4");
	ourTree.put("Arducopter.IsAvailable", true);
	ourTree.put("Arducopter.DoFakeStart", false);
	ourTree.put("CompressionProfile", pcl::octree::LOW_RES_ONLINE_COMPRESSION_WITHOUT_COLOR);
	ourTree.put("ShowStatistics", false);
	ourTree.put("PointResolution", 0.001);
	ourTree.put("OctreeResolution", 0.01);
	ourTree.put("DoVoxelGridDownDownSampling", false);
	ourTree.put("IFrameRate", 30);
	ourTree.put("DoColorEncoding", false);
	ourTree.put("ColorBitResolution", 6);
	ourTree.put("Camera.Number", 1);
	ourTree.put("Camera.FramesPerSecond", 180);
	ourTree.put("Camera.FrameWidth", 320);
	ourTree.put("Camera.FrameHeight", 240);
}

void JSONConfig::Parser(std::string fileName)
{
	std::ifstream ifs(fileName);
	if (ifs.is_open())
	{
		boost::property_tree::json_parser::read_json(ifs, ourTree);
	}
	else
	{
		std::ostringstream ofs(fileName);
		boost::property_tree::write_json(ofs, ourTree);
	}

	IP = ourTree.get<std::string>("IP", "127.0.0.1");
	KinectPort = ourTree.get<std::string>("KinectPort.Port", "6666");
	ConfigOfCommandMaker.IP = ourTree.get<std::string>("IP", "127.0.0.1");
	ConfigOfCommandMaker.Port = ourTree.get<std::string>("CommandPort.Port", "6667");
	ConfigOfSend.IP = ourTree.get<std::string>("IP", "127.0.0.1");
	ConfigOfSend.Port = ourTree.get<std::string>("SendPort.Port", "6668");
	ConfigOfSend.IsAvailable = ourTree.get<bool>("SendPort.IsAvailable", true);
	CarduinoPort = ourTree.get<std::string>("CarduinoPort.Port", "\\\\.\\COM3");
	ConfigOfArducopter.Port = ourTree.get<std::string>("ArducopterPort.Port", "\\\\.\\COM4");
	ConfigOfArducopter.IsAvailable = ourTree.get<bool>("ArducopterPort.IsAvailable", true);
	ConfigOfArducopter.DoFakeStart = ourTree.get<bool>("ArducopterPort.DoFakeStart", false);
	//CompressionProfile = ourTree.get<int>("CompressionProfile", 0);
	ShowStatistics = ourTree.get<bool>("ShowStatistics", false);
	PointResolution = ourTree.get<double>("PointResolution", 0.001);
	OctreeResolution = ourTree.get<double>("OctreeResolution", 0.01);
	DoVoxelGridDownDownSampling = ourTree.get<bool>("DoVoxelGridDownDownSampling", false);
	IFrameRate = ourTree.get<unsigned int>("IFrameRate", 30);
	DoColorEncoding = ourTree.get<bool>("DoColorEncoding", false);
	ColorBitResolution = ourTree.get<unsigned int>("ColorBitResolution", 6);
	ConfigOfCamera.Number = ourTree.get<int>("CameraNumber", 1);
	ConfigOfCamera.FramesPerSecond = ourTree.get<int>("CameraFramesPerSecond", 180);
	ConfigOfCamera.FrameWidth = ourTree.get<int>("FrameWidth", 320);
	ConfigOfCamera.FrameHeight = ourTree.get<int>("FrameHeight", 240);
}