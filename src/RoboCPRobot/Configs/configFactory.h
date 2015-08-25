/*








//WARNING!!!!!!!! This is a template file. Do not change it without necessity.










*/
#pragma once
#include <iostream>
#include <QtCore/QJsonDocument>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtCore/QJsonObject>
#include <QtCore/QHash>
#include "Config.h"
#include "KinectConfig.h"
#include "CommandConfig.h"
#include "SendConfig.h"
#include "CarduinoConfig.h"
#include "ArducopterConfig.h"
#include "CameraConfig.h"
#include "OctreeEncoderConfig.h"

//Main class of processing config
class configFactory{
public:
  //configName - name of device
  Config* ConfigByName(QString configName);
  configFactory();
  ~configFactory();
  void Parse();
private:
  QHash<QString, Config*> MapOfConfigs;
  Config* DetermineConfigObject(QJsonObject treeOfObject);
};