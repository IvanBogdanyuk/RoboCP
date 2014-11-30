#include "JSONConfig.h"

JSONConfig::JSONConfig ()
{
	
}

void JSONConfig::Parser(QString fileName)
{
  QFile json(fileName);
  if(json.open(QIODevice::ReadOnly))
  {
    QJsonParseError  parseError;
	QJsonObject jsonDoc = QJsonDocument::fromJson(json.readAll(), &parseError).object();
	for(auto it = jsonDoc.begin(); it != jsonDoc.end(); it++)
	{
		MapOfConfigs[it.key()] = DetermineConfigObject(it.value().toObject());
	}
  }
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
