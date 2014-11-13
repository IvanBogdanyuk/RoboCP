#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "pcl/compression/octree_pointcloud_compression.h"
#include <boost/property_tree/ptree.hpp>
#include "ArduCopterConfig.h"
#include <unordered_map>
#include <QJsonDocument.h>
#include <QFile.h>
#include <QString.h>
#include <QByteArray.h>
#include <QJsonObject.h>
#include <QHash>

//#define ENABLE_LOGGING

using namespace pcl;
using namespace pcl::octree;

// Config class
class JSONConfig {
private:

	boost::property_tree::ptree JSONTree; //Tree with "config.json"
	void CreateDefaultConfig(QString fileName);
	QHash<QString, Config*> MapOfConfigs;
	Config* DetermineConfigObject(QJsonObject treeOfObject);

public:

  JSONConfig();
  void Parser(QString fileName);
  Config* ConfigByName(QString configName);
};