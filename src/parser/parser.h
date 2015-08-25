#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

const string names[4] = { "header", "cpp", "configFactoryHeader", "configFactoryCpp" };

class propType{
public:
	propType()
	{
		prop = string("");
		type = string("");
	}
	propType(const char *_prop, const char *_type)
	{
		prop = string(_prop);
		type = string(_type);
	}
	propType(string _prop, string _type)
	{
		prop = _prop;
		type = _type;
	}
	string prop;
	string type;
};

class Class{
public:
	Class(string _Name, vector < propType > _Props)
	{
		Name = _Name;
		Props = _Props;
	}
	string Name;
	vector < propType > Props;
};

class Templates{
public:
	void readTemplates();
	map<string,string> tempMap;

};
void writeToHeader(Class classdata);
void writeToCpp(Class classdata);
void writeToConfigFactoryHeader(vector<Class> classArr);
void writeToConfigFactoryCpp(vector<Class> classArr);
void writeToVcxproj(vector<Class> classArr,char **dir);