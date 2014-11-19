#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;
void writetohfile(string classname, vector<pair<string, string>> classdata);
void writetocppfile(string classname, vector<pair<string, string>> classdata);
void wrtitetocfh(vector<pair<string, vector<pair<string, string>>>> classmas);
void wrtitetocfcpp(vector<pair<string, vector<pair<string, string>>>> classmas);
///finds first name from string from poistion=pos,
//if nothing is found makes empty pair with "-1" as 2 argument
pair<string, int> getName(string s, int pos)
{
	int namestart = s.find('"', pos);

	if ((namestart > s.find('}', pos)) || (namestart == -1))
		return make_pair("", -1);
	int nameend = s.find('"', namestart + 1);
	return make_pair(s.substr(namestart + 1, nameend - 1 - namestart), nameend);
}
///finds pair (key,value) and index of last symbol in value name,
//if nothing is found makes empty pair with "-1" as 2 argument 
pair<pair<string, string>, int> getKeyValue(string s, int pos)
{
	if (s.find('"', pos) > s.find('}') || (s.find('"', pos) == -1))
		return make_pair(make_pair("", ""), -1);
	pair<string, int> key = getName(s, pos);
	if (key.second == -1)
		return make_pair(make_pair("", ""), -1);
	pos = s.find(':', pos) + 1;
	pair<string, int> value = getName(s, pos);
	if (value.second == -1)
		return make_pair(make_pair("", ""), -1);
	return make_pair(make_pair(key.first, value.first), pos + value.first.size() + 2);
}
void parsecf(string &s)
{
	vector<pair<string, vector<pair<string, string>>>> classmas;
	char c = '1';
	int pos = 0;
	string classname;
	string classstr;
	while (1)
	{
		pair<string, int> classname = getName(s, pos);
		if (classname.second == -1)
			break;
		pos += classname.first.size() + 2;
		int begin = s.find("{", pos);
		int end = s.find("}", pos);
		classstr = s.substr(begin, end - begin);
		vector<pair<string, string>> classdata;
		int i = 0;
		while (1)
		{
			pair<pair<string, string>, int> kv = getKeyValue(classstr, i);
			if (kv.second == -1)
				break;
			classdata.push_back(kv.first);
			i = kv.second + 1;
		}
		pos = end + 1;
		classmas.push_back(make_pair(classname.first, classdata));
	}
	for (int i = 0; i < classmas.size(); ++i)
	{
		writetohfile(classmas[i].first, classmas[i].second);
		writetocppfile(classmas[i].first, classmas[i].second);
	}
	wrtitetocfcpp(classmas);
	wrtitetocfh(classmas);
	return;
}

void writetohfile(string classname, vector<pair<string, string>> classdata)
{
	string filename = classname + "Config.h";
	ofstream out(filename, ofstream::out);
	string templatestr =
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
	//

	while (templatestr.find("%classname%") != -1)
	{
		templatestr.replace(templatestr.find("%classname%"), 11, classname);
	}
	string getmas = "";
	for (int i = 0; i < classdata.size(); ++i)
	{
		getmas += "  " + classdata[i].second + " get" + classdata[i].first + "();\n";
	}
	templatestr.replace(templatestr.find("[getmas]"), 8, getmas);
	string propstr = "";
	for (int i = 0; i < classdata.size(); ++i)
	{
		propstr += "  " + classdata[i].second + " " + classdata[i].first + ";\n";
	}
	templatestr.replace(templatestr.find("[prop]"), 6, propstr);

	out << templatestr;
	out.close();

}

void writetocppfile(string classname, vector<pair<string, string>> classdata)
{
	string filename = classname + "Config.cpp";
	ofstream fout(filename, ofstream::out);

	

	string templatestr =
		"#include \"%classname%Config.h\"\n\n"
		"%classname%Config::%classname%Config():[constructorinit] {\n"
		"  \n"
		"}\n"
		"%classname%Config::~%classname%Config(){\n"
		"  \n"
		"}\n"
		"[getmas]";

	while (templatestr.find("%classname%") != -1)
	{
		templatestr.replace(templatestr.find("%classname%"), 11, classname);
	}
	string constructorinit = "";
	for (int i = 0; i < classdata.size(); ++i)
	{
		constructorinit += classdata[i].first + "(";
		if (classdata[i].second == "int")
			constructorinit += "0";
		else
		if (classdata[i].second == "double")
			constructorinit += "0.0";
		else
		if (classdata[i].second == "string")
			constructorinit += "\"\"";
		else
		if (classdata[i].second == "bool")
			constructorinit += "false";
		if (i != classdata.size() - 1)
		{
			constructorinit += "), ";
		}
		else
			constructorinit += ") ";
	}
	templatestr.replace(templatestr.find("[constructorinit]"), 17, constructorinit);
	
	string getmas = "";
	for (int i = 0; i < classdata.size(); ++i)
	{
		getmas += classdata[i].second + +" " + classname + "Config::" + "get" + classdata[i].first + "() {\n";
		getmas += "  return " + classdata[i].first + ";\n}\n";
	}
	templatestr.replace(templatestr.find("[getmas]"), 8, getmas);
	fout << templatestr;
	fout.close();
}
void wrtitetocfh(vector<pair<string, vector<pair<string, string>>>> classmas)
{
	ofstream outh("configFactory.h", ofstream::out);
	string templatestrh = 
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
	string incmas = "";
	for (int i = 0; i < classmas.size(); ++i)
	{
		incmas += "#include \"" + classmas[i].first + "Config.h\"\n";
	}
	templatestrh.replace(templatestrh.find("[incmas]"), 8, incmas);
	outh << templatestrh;
	outh.close();

}
void wrtitetocfcpp(vector<pair<string, vector<pair<string, string>>>> classmas)
{
	
	ofstream outcpp("configFactory.cpp", ofstream::out);
	
	string templatestrcpp =
		"#include \"configFactory.h\"\n\n"
		"configFactory::~configFactory(){\n"
		"\n"
		"}\n"
		"configFactory::configFactory(){\n"
		"  }\n"
		"\n"
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
		"}\n"
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
	
	string ifmas = "";
	for (int i = 0; i < classmas.size(); ++i)
	{
		string propmas = "";
		ifmas += "  if (type == \""+ classmas[i].first+"\"){\n";
		ifmas += "    " + classmas[i].first + "Config *config= new " + classmas[i].first + "Config();\n";
		ifmas += "[propmas]";
		ifmas += "    return config;\n  }\n";
		
		for (int j = 0; j < classmas[i].second.size(); ++j)
		{
			if (classmas[i].second[j].second == "int")
				propmas += "    config->" + classmas[i].second[j].first + " = treeOfObject.value(\"" + classmas[i].second[j].first + "\").toInt();\n";
			else
			if (classmas[i].second[j].second == "bool")
				propmas += "    config->" + classmas[i].second[j].first + " = treeOfObject.value(\"" + classmas[i].second[j].first + "\").toInt();\n";
			else
			if (classmas[i].second[j].second == "double")
				propmas += "    config->" + classmas[i].second[j].first + " = treeOfObject.value(\"" + classmas[i].second[j].first + "\").toDouble();\n";
			else
			if (classmas[i].second[j].second == "string")
				propmas += "    config->" + classmas[i].second[j].first + " = treeOfObject.value(\"" + classmas[i].second[j].first + "\").toString().toStdString();\n";
		}
		ifmas.replace(ifmas.find("[propmas]"), 9, propmas);

	}
	templatestrcpp.replace(templatestrcpp.find("[ifmas]"), 7, ifmas);
	outcpp << templatestrcpp;
	outcpp.close();
}
int main()
{
	int n;
	ifstream in("classconfig.json", ifstream::in);
	char *text = new char[10000];
	char c = '1';
	int len = 0;
	while (in.good())
	{
		c = in.get();
		text[len++] = c;
	}
	text[len] = '\0';
	string s(text);
	parsecf(s);
	s.clear();




}