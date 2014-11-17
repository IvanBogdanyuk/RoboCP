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

class configFactory{
public:
  Config* ConfigByName(QString configName);
  configFactory();
  ~configFactory();
private:
  QHash<QString, Config*> MapOfConfigs;
  Config* DetermineConfigObject(QJsonObject treeOfObject);
};