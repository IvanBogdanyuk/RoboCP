#pragma once
#include <iostream>
#include <QJsonDocument>
#include <QFile>
#include <QString>
#include <QByteArray>
#include <QJsonObject>
#include <QHash>
#include "Config.h"
#include "KinectConfig.h"
#include "CommandConfig.h"
#include "SendConfig.h"
#include "CarduinoConfig.h"
#include "ArducopterConfig.h"
#include "CameraConfig.h"

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