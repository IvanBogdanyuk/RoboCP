#include "parser.h"
string tempNotExist(int i)
{

	if (i == 0)
	{
		ofstream out("header.template", ofstream::out);
		string s =
			"//WARNING!!!!!!!! This is a template file. Do not change it without necessity.\n"
			"#pragma once\n"
			"#include \"Config.h\"\n"
			"#include \"configFactory.h\"\n"
			"class %classname%Config : public Config{\n"
			"public:\n"
			"  %classname%Config();\n"
			"  friend class configFactory;\n"
			"  ~%classname%Config();\n"
			"[getmas]\n"
			"private:\n"
			"[prop]"
			"};";
		out << s;
		out.close();
		return s;
	}
	else
	if (i == 1)
	{
		ofstream out("cpp.template", ofstream::out);
		string s =
			"//WARNING!!!!!!!! This is a template file. Do not change it without necessity.\n"
			"#include \"%classname%Config.h\"\n\n"
			"%classname%Config::%classname%Config():[constructorinit] {\n"
			"  \n"
			"}\n"
			"%classname%Config::~%classname%Config(){\n"
			"  \n"
			"}\n"
			"[getmas]";
		out << s;
		out.close();
		return s;
	}
	else
	if (i == 2)
	{
		ofstream out("configFactoryHeader.template", ofstream::out);
		string s =
			"//WARNING!!!!!!!! This is a template file. Do not change it without necessity.\n"
			"#pragma once\n"
			"#include <iostream>\n"
			"#include <QJsonDocument>\n"
			"#include <QFile>\n"
			"#include <QString>\n"
			"#include <QByteArray>\n"
			"#include <QJsonObject>\n"
			"#include <QHash>\n"
			"#include \"Config.h\"\n"
			"[incmas]"
			"\n"
			"//Main class of processing config\n"
			"class configFactory{\n"
			"public:\n"
			"  //configName - name of device\n"
			"  Config* ConfigByName(QString configName);\n"
			"  configFactory();\n"
			"  ~configFactory();\n"
			"  void Parse();\n"
			"private:\n"
			"  QHash<QString, Config*> MapOfConfigs;\n"
			"  Config* DetermineConfigObject(QJsonObject treeOfObject);\n"
			"};"
			"";
		out << s;
		out.close();
		return s;
	}
	else
	if (i == 3)
	{
		ofstream out("configFactoryCpp.template", ofstream::out);
		string s =
			"//WARNING!!!!!!!! This is a template file. Do not change it without necessity.\n"
			"#include \"configFactory.h\"\n\n"
			"configFactory::~configFactory(){\n"
			"\n"
			"}\n"
			"configFactory::configFactory(){\n"
			"\n"
			"}\n"
			"void configFactory::Parse(){\n"
			"  QFile json(\"config.json\");\n"
			"  if (json.open(QIODevice::ReadOnly))\n"
			"  {\n"
			"    QJsonParseError  parseError;\n"
			"    QJsonObject jsonDoc = QJsonDocument::fromJson(json.readAll(), &parseError).object();\n"
			"    for (auto it = jsonDoc.begin(); it != jsonDoc.end(); it++)\n"
			"    {\n"
			"      MapOfConfigs[it.key()] = DetermineConfigObject(it.value().toObject());\n"
			"    }\n"
			"  }\n"
			"Config* configFactory::DetermineConfigObject(QJsonObject treeOfObject)\n"
			"{\n"
			"QString type = treeOfObject.value(\"Type\").toString();\n"
			"[ifmas]"
			"\n"
			"return nullptr;\n"
			"}\n"
			"\n"
			"Config* configFactory::ConfigByName(QString configName)\n"
			"{\n"
			"	return MapOfConfigs[configName];\n"
			"}"
			"";
		out << s;
		out.close();
		return s;
	}

}
void Templates::readTemplates()
{

	char *text = new char[10000];
	for (int i = 0; i < 4; ++i)
	{

		ifstream in(names[i] + ".template", ifstream::in);
		char c = '1';
		int len = 0;
		while (in.good())
		{
			c = in.get();
			text[len++] = c;
		}
		text[len] = '\0';
		string s(text);
		if (s.size() < 5)
		{
			tempNotExist(i);
			i--;
			continue;
		}
		tempMap[names[i]] = s;
		in.close();
	}
	delete[] text;
}

