#include "JSONConfig.h"
#include "boost\property_tree\json_parser.hpp"

JSONConfig::JSONConfig ()
{
	
}

void JSONConfig::CreateDefaultConfig(QString fileName)
{
  // default values
  //JSONTree.put("IP", "127.0.0.1");
  //JSONTree.put("KinectPort.Port", "6666");
  //JSONTree.put("CommandPort.Port", "6667");
  //JSONTree.put("SendPort.Port", "6668");
  //JSONTree.put("CarduinoPort.Port", "\\\\.\\COM3");
  //JSONTree.put("ArducopterPort.Port", "\\\\.\\COM4");
  //JSONTree.put("CompressionProfile", pcl::octree::LOW_RES_ONLINE_COMPRESSION_WITHOUT_COLOR); //-?
  //JSONTree.put("ShowStatistics", false);
  //JSONTree.put("PointResolution", 0.001);
  //JSONTree.put("OctreeResolution", 0.01);
  //JSONTree.put("DoVoxelGridDownDownSampling", false);
  //JSONTree.put("IFrameRate", 30);
  //JSONTree.put("DoColorEncoding", false);
  //JSONTree.put("ColorBitResolution", 6);
  //JSONTree.put("CameraNumber", 1);
  //JSONTree.put("CameraFramesPerSecond", 180);
  //JSONTree.put("CameraFrameWidth", 320);
  //JSONTree.put("CameraFrameHeight", 240);
  //std::ofstream file(fileName);
  //boost:property_tree:write_json(file, JSONTree);
  //MapOfConfigs = unordered_map<string, Config*>();
  //file.close();
}

void JSONConfig::Parser(QString fileName)
{
  QFile json(fileName);
  if(json.open(QIODevice::ReadOnly))
  {
    QJsonParseError  parseError;
	QJsonObject jsonDoc = QJsonDocument::fromJson(json.readAll(), &parseError).object();
  /*std::ifstream ifs(fileName);
  if (ifs.is_open())
  {
    boost::property_tree::read_json(ifs, JSONTree);
  }
  else
  {
    CreateDefaultConfig(fileName);
  }*/
	for(auto it = jsonDoc.begin(); it != jsonDoc.end(); it++)
	{
		MapOfConfigs[it.key()] = DetermineConfigObject(it.value().toObject());
	}
  }
  /*IP = JSONTree.get<std::string>("IP.Address", "127.0.0.1");
  KinectPort = JSONTree.get<std::string>("KinectPort.Port", "6666");
  CommandPort = JSONTree.get<std::string>("CommandPort.Port", "6667");
  SendPort = JSONTree.get<std::string>("SendPort.Port", "6668");
  CarduinoPort = JSONTree.get<std::string>("CarduinoPort.Port", "\\\\.\\COM3");
  ArducopterPort = JSONTree.get<std::string>("ArducopterPort.Port", "\\\\.\\COM4");
  CompressionProfile = pcl::octree::compression_Profiles_e(JSONTree.get<int>("CompressionProfile", pcl::octree::LOW_RES_ONLINE_COMPRESSION_WITHOUT_COLOR));
  ShowStatistics = JSONTree.get<bool>("ShowStatistics", false);
  PointResolution = JSONTree.get<double>("PointResolution", 0.001);
  OctreeResolution = JSONTree.get<double>("OctreeResolution", 0.01);
  DoVoxelGridDownDownSampling = JSONTree.get<bool>("DoVoxelGridDownDownSampling", false);
  IFrameRate = JSONTree.get<unsigned int>("IFrameRate", 30);
  DoColorEncoding = JSONTree.get<bool>("DoColorEncoding", false);
  ColorBitResolution = JSONTree.get<unsigned int>("ColorBitResolution", 6);
  CameraNumber = JSONTree.get<int>("CameraNumber", 1);
  CameraFramesPerSecond = JSONTree.get<int>("CameraFramesPerSecond", 180);
  CameraFrameWidth = JSONTree.get<int>("OctreeResolution", 320);
  CameraFrameHeight = JSONTree.get<int>("OctreeResolution", 240);*/
}

Config* JSONConfig::DetermineConfigObject(QJsonObject treeOfObject)
{
	QString type = treeOfObject.value("Type").toString();
	if(type == "ArduCopter")
	{
		ArduCopterConfig* config = new ArduCopterConfig();
			config->DoFakeStart = treeOfObject.value("DoFakeStart").toInt();
		config->IsAvailable = treeOfObject.value("IsAvailable").toInt();
		config->setPort(treeOfObject.value("Port").toString().toStdString());
		return config;
	}
}

Config* JSONConfig::ConfigByName(QString configName)
{
	return MapOfConfigs[configName];
}
