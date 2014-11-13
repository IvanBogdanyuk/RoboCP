#pragma once
#include <iostream>
#include "pcl/compression/octree_pointcloud_compression.h"
#include "ArduCopterConfig.h"
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

	QHash<QString, Config*> MapOfConfigs;
	Config* DetermineConfigObject(QJsonObject treeOfObject);

public:

  JSONConfig();
  void Parser(QString fileName);
  Config* ConfigByName(QString configName);
};