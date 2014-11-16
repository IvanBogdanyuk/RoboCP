#pragma once
#include <iostream>
#include <QJsonDocument.h>
#include <QFile.h>
#include <QString.h>
#include <QByteArray.h>
#include <QJsonObject.h>
#include <QHash>
#include "CameraConfig".h
#include "CarduinoConfig".h
#include "ArducopterConfig".h

class configFactory{
public:
  Config* ConfigByName(QString configName);  configFactory();
  ~configFactory();
private:
  QHash<QString, Config*> MapOfConfigs;
  Config* DetermineConfigObject(QJsonObject treeOfObject);
};