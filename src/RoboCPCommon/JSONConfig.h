#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "pcl/compression/octree_pointcloud_compression.h"
#include <boost/property_tree/ptree.hpp>
#include "ArduCopterConfig.h"
#include <unordered_map>

//#define ENABLE_LOGGING

using namespace pcl;
using namespace pcl::octree;
using namespace std;


// Config class
class JSONConfig {
private:

	boost::property_tree::ptree JSONTree; //Tree with "config.json"
	void CreateDefaultConfig(std::string fileName);
    unordered_map<string, Config*> MapOfConfigs;
	Config* DetermineConfigObject(boost::property_tree::ptree treeOfObject);

public:

  JSONConfig();
  void Parser(std::string fileName);
  Config* ConfigByName(std::string configName);
};