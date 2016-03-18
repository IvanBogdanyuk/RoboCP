#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "parser.h"
using namespace std;

Templates temp;
///finds first name from string from poistion,
///returns name and position of last symbol+1
///if nothing is found makes empty pair with "-1" as 2 argument
pair<string, int> getName(string s, int pos)
{
	int namestart = s.find('"', pos);

	if ((namestart > s.find('}', pos)) || (namestart == -1))
		return make_pair("", -1);
	int nameend = s.find('"', namestart + 1);
	return make_pair(s.substr(namestart + 1, nameend - 1 - namestart), nameend);
}

///finds pair (key,value) 
///returns this pair and index of last symbol in value name,
///if nothing is found makes empty pair with "-1" as 2 argument 
pair<propType, int> getKeyValue(string s, int pos)
{
	if (s.find('"', pos) > s.find('}') || (s.find('"', pos) == -1))
		return make_pair(propType(), -1);
	pair<string, int> key = getName(s, pos);
	if (key.second == -1)
		return make_pair(propType(), -1);
	pos = s.find(':', pos) + 1;
	pair<string, int> value = getName(s, pos);
	if (value.second == -1)
		return make_pair(propType(), -1);
	return make_pair(propType(key.first, value.first), pos + value.first.size() + 2);
}

void parsecf(string &s,char **dir)
{
	vector<Class> classArr;
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
		vector<propType> classdata;
		int i = 0;
		while (1)
		{
			pair<propType, int> kv = getKeyValue(classstr, i);
			if (kv.second == -1)
				break;
			classdata.push_back(kv.first);
			i = kv.second + 1;
		}
		pos = end + 1;
		classArr.push_back(Class(classname.first,classdata));
	}
	for (int i = 0; i < classArr.size(); ++i)
	{
		writeToHeader(classArr[i]);
		writeToCpp(classArr[i]);
	}
	writeToConfigFactoryHeader(classArr);
	writeToConfigFactoryCpp(classArr);
	writeToVcxproj(classArr, dir);
	return;
}

void writeToHeader(Class classdata)
{
	string filename =classdata.Name + "Config.h";
	ofstream out(filename, ofstream::out);
	string templatestr = temp.tempMap["header"];

	while (templatestr.find("%classname%") != -1)
	{
		templatestr.replace(templatestr.find("%classname%"), 11, classdata.Name);
	}
	string getmas = "";
	for (int i = 0; i < classdata.Props.size(); ++i)
	{
		getmas += "  " + classdata.Props[i].type + " get" + classdata.Props[i].prop + "();\n";
	}
	templatestr.replace(templatestr.find("[getmas]"), 8, getmas);
	string propstr = "";
	for (int i = 0; i < classdata.Props.size(); ++i)
	{
		propstr += "  " + classdata.Props[i].type + " " + classdata.Props[i].prop + ";\n";
	}
	templatestr.replace(templatestr.find("[prop]"), 6, propstr);

	out << templatestr;
	out.close();

}

void writeToCpp(Class classdata)
{
	string filename = classdata.Name + "Config.cpp";
	ofstream fout(filename, ofstream::out);
	string templatestr = temp.tempMap["cpp"];
	while (templatestr.find("%classname%") != -1)
	{
		templatestr.replace(templatestr.find("%classname%"), 11, classdata.Name);
	}
	string constructorinit = "";
	for (int i = 0; i < classdata.Props.size(); ++i)
	{
		constructorinit += classdata.Props[i].prop + "(";
		if (classdata.Props[i].type == "int")
			constructorinit += "0";
		else
		if (classdata.Props[i].type == "double")
			constructorinit += "0.0";
		else
		if (classdata.Props[i].type == "string")
			constructorinit += "\"\"";
		else
		if (classdata.Props[i].type == "bool")
			constructorinit += "false";
		if (i != classdata.Props.size() - 1)
		{
			constructorinit += "), ";
		}
		else
			constructorinit += ") ";
	}
	templatestr.replace(templatestr.find("[constructorinit]"), 17, constructorinit);

	string getmas = "";
	for (int i = 0; i < classdata.Props.size(); ++i)
	{
		getmas += classdata.Props[i].type + +" " + classdata.Name + "Config::" + "get" + classdata.Props[i].prop + "() {\n";
		getmas += "  return " + classdata.Props[i].prop + ";\n}\n";
	}
	templatestr.replace(templatestr.find("[getmas]"), 8, getmas);
	fout << templatestr;
	fout.close();
}

void writeToConfigFactoryHeader(vector<Class> classArr)
{
	ofstream outh("configFactory.h", ofstream::out);
	string templatestrh = temp.tempMap["configFactoryHeader"];
	string incmas = "";
	for (int i = 0; i < classArr.size(); ++i)
	{
		incmas += "#include \"" + classArr[i].Name + "Config.h\"\n";
	}
	templatestrh.replace(templatestrh.find("[incmas]"), 8, incmas);
	outh << templatestrh;
	outh.close();

}

void writeToConfigFactoryCpp(vector<Class> classArr)
{

	ofstream outcpp("configFactory.cpp", ofstream::out);

	string templatestrcpp = temp.tempMap["configFactoryCpp"];
		

	string ifmas = "";
	for (int i = 0; i < classArr.size(); ++i)
	{
		string propmas = "";
		ifmas += "  if (type == \"" + classArr[i].Name + "\"){\n";
		ifmas += "    " + classArr[i].Name + "Config *config= new " + classArr[i].Name + "Config();\n";
		ifmas += "[propmas]";
		ifmas += "    return config;\n  }\n";

		for (int j = 0; j < classArr[i].Props.size(); ++j)
		{
			if (classArr[i].Props[j].type == "int")
				propmas += "    config->" + classArr[i].Props[j].prop + 
					+" = treeOfObject.value(\"" + classArr[i].Props[j].prop + "\").toInt();\n";
			else
			if (classArr[i].Props[j].type == "bool")
				propmas += "    config->" + classArr[i].Props[j].prop + 
					+" = treeOfObject.value(\"" + classArr[i].Props[j].prop + "\").toInt();\n";
			else
			if (classArr[i].Props[j].type == "double")
				propmas += "    config->" + classArr[i].Props[j].prop + 
					+" = treeOfObject.value(\"" + classArr[i].Props[j].prop + "\").toDouble();\n";
			else
			if (classArr[i].Props[j].type == "string")
				propmas += "    config->" + classArr[i].Props[j].prop + 
					+" = treeOfObject.value(\"" + classArr[i].Props[j].prop + "\").toString().toUtf8().constData();\n";
		}
		ifmas.replace(ifmas.find("[propmas]"), 9, propmas);

	}
	templatestrcpp.replace(templatestrcpp.find("[ifmas]"), 7, ifmas);
	outcpp << templatestrcpp;
	outcpp.close();
}
void writeToVcxproj(vector<Class> classArr, char **dir)
{
	ifstream in(dir[1], ifstream::in);
	string file;
	char c;
	while (in.good())
	{
		c = in.get();
		file += c;
	}
	in.close();
	if (file.size() == 0)
		return;
	string configDir=file.substr(0,file.find("\\RoboCPRobot"));
	configDir = configDir.substr(configDir.rfind("\"")+1, configDir.npos);
	configDir += "\\RoboCPRobot\\";
	int pos=file.rfind("<ClInclude")-1;
	for (int i = 0; i < classArr.size(); ++i)
	{
		file.insert(pos, "    <ClInclude Include=\""+configDir+classArr[i].Name+"Config.h\" />\n");
	}
	pos = file.rfind("<ClCompile") - 1;
	for (int i = 0; i < classArr.size(); ++i)
	{
		file.insert(pos, "     <ClCompile Include=\"" + configDir + classArr[i].Name + "Config.cpp\" />\n");
	}
	ofstream out(dir[1], ofstream::out);
	out << file;
	out.close();
}

int main(int argc, char *argv[])
{
	int n;
	temp.readTemplates();
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
	parsecf(s,argv);
	delete[] text;
	in.close();
}