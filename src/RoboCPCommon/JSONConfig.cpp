#include "XMLConfig.h"


XMLConfig::XMLConfig ()
{
	// default values
	ourTree.put("IP.Address", "127.0.0.1");
	ourTree.put("KinectPort.Port", "6666");
	ourTree.put("CommandPort.Port", "6667");
	ourTree.put("SendPort.Port", "6668");
	ourTree.put("CarduinoPort.Port", "\\\\.\\COM3");
	ourTree.put("ArducopterPort.Port", "\\\\.\\COM4");
	ourTree.put("CompressionProfile", pcl::octree::LOW_RES_ONLINE_COMPRESSION_WITHOUT_COLOR); //-?
	CompressionProfile = pcl::octree::LOW_RES_ONLINE_COMPRESSION_WITHOUT_COLOR; // - ?
	ourTree.put("ShowStatistics", false);
	ourTree.put("PointResolution", 0.001);
	ourTree.put("OctreeResolution", 0.01);
	ourTree.put("DoVoxelGridDownDownSampling", false);
	ourTree.put("IFrameRate", 30);
	ourTree.put("DoColorEncoding", false);
	ourTree.put("ColorBitResolution", 6);
	ourTree.put("CameraNumber", 1);
	ourTree.put("CameraFramesPerSecond", 180);
	ourTree.put("CameraFrameWidth", 320);
	ourTree.put("CameraFrameHeight", 240);
}

std::string XMLConfig::get_IP()
{
	if (ourTree.get<bool>("IP.IsAvailable", 1))
	{
		return IP;
	}
	else
	{
		throw invalid_argument("IP is not available");
	}
}

std::string XMLConfig::get_KinectPort()
{
	if (ourTree.get<bool>("KinectPort.IsAvailable", 1))
	{
		return KinectPort;
	}
	else
	{
		throw invalid_argument("Kinect is not available");
	}
}

std::string XMLConfig::get_CommandPort()
{
	if (ourTree.get<bool>("CommandPort.IsAvailable", 1))
	{
		return CommandPort;
	}
	else
	{
		throw invalid_argument("Command's port is not available");
	}
}

std::string XMLConfig::get_SendPort()
{
	if (ourTree.get<bool>("SendPort.IsAvailable", 1))
	{
		return SendPort;
	}
	else
	{
		throw invalid_argument("Send's port is not available");
	}
}

std::string XMLConfig::get_CarduinoPort()
{
	if (ourTree.get<bool>("CarduinoPort.IsAvailable", 1))
	{
		return CarduinoPort;
	}
	else
	{
		throw invalid_argument("Carduino is not available");
	}
}

std::string XMLConfig::get_ArducopterPort()
{
	if (ourTree.get<bool>("SendPort.IsAvailable", 1))
	{
		return ArducopterPort;
	}
	else
	{
		throw invalid_argument("Send's port is not available");
	}
}


void XMLConfig::Parser(std::string fileName)
{
	std::ifstream ifs(fileName);
	if (ifs.is_open())
	{
		boost::property_tree::read_xml(ifs, ourTree);
	}
	else
	{
		std::ofstream ofs(fileName);
		boost::property_tree::write_xml(ofs, ourTree);
	}
	IP = ourTree.get<std::string>("IP.Address", "127.0.0.1");
	KinectPort = ourTree.get<std::string>("KinectPort.Port", "6666");
	CommandPort = ourTree.get<std::string>("CommandPort.Port", "6667");
	SendPort = ourTree.get<std::string>("SendPort.Port", "6668");
	CarduinoPort = ourTree.get<std::string>("CarduinoPort.Port", "\\\\.\\COM3");
	ArducopterPort = ourTree.get<std::string>("ArducopterPort.Port", "\\\\.\\COM4");
	//CompressionProfile = ourTree.get<pcl::octree::compression_Profiles_e>("CompressionProfile", pcl::octree::LOW_RES_ONLINE_COMPRESSION_WITHOUT_COLOR);
	ShowStatistics = ourTree.get<bool>("ShowStatistics", false);
	PointResolution = ourTree.get<double>("PointResolution", 0.001);
	OctreeResolution = ourTree.get<double>("OctreeResolution", 0.01);
	DoVoxelGridDownDownSampling = ourTree.get<bool>("DoVoxelGridDownDownSampling", false);
	IFrameRate = ourTree.get<unsigned int>("IFrameRate", 30);
	DoColorEncoding = ourTree.get<bool>("DoColorEncoding", false);
	ColorBitResolution = ourTree.get<unsigned int>("ColorBitResolution", 6);
	CameraNumber = ourTree.get<int>("CameraNumber", 1);
	CameraFramesPerSecond = ourTree.get<int>("CameraFramesPerSecond", 180);
	CameraFrameWidth = ourTree.get<int>("OctreeResolution", 320);
	CameraFrameHeight = ourTree.get<int>("OctreeResolution", 240);
}