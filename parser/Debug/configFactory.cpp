#include "configFactory.h"

configFactory::~configFactory(){

}
configFactory::configFactory(){
  QFile json(config.json);
  if (json.open(QIODevice::ReadOnly))
  {
    QJsonParseError  parseError;
    QJsonObject jsonDoc = QJsonDocument::fromJson(json.readAll(), &parseError).object();
    for (auto it = jsonDoc.begin(); it != jsonDoc.end(); it++)
    {
      MapOfConfigs[it.key()] = DetermineConfigObject(it.value().toObject());
    }
  }
}
Config* configFactory::DetermineConfigObject(QJsonObject treeOfObject)
{
QString type = treeOfObject.value("Type").toString();
  if (type == "Camera"){
    CameraConfig *config= new Camera();
    config->Number = treeOfObject.value("Number").toInt();
    config->FramesPerSecond = treeOfObject.value("FramesPerSecond").toInt();
    config->FrameWidth = treeOfObject.value("FrameWidth").toInt();
    config->FrameHeight = treeOfObject.value("FrameHeight").toInt();
    return config;
  }
  if (type == "Carduino"){
    CarduinoConfig *config= new Carduino();
    config->Port = treeOfObject.value("Port").toString().toStdString());
    return config;
  }
  if (type == "Arducopter"){
    ArducopterConfig *config= new Arducopter();
    config->Port = treeOfObject.value("Port").toString().toStdString());
    config->IsAvailable = treeOfObject.value("IsAvailable").toInt();
    config->DoFakeStart = treeOfObject.value("DoFakeStart").toInt();
    return config;
  }

return nullptr;
}

Config* JSONConfig::ConfigByName(QString configName)
{
	return MapOfConfigs[configName];
}