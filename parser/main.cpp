#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;
void writetohfile(string classname, vector<pair<string, string>> classdata);
void writetocppfile(string classname, vector<pair<string, string>> classdata);
void wrtitetocf(vector<pair<string, vector<pair<string, string>>>> classmas);
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
	wrtitetocf(classmas);
	return;
}

void writetohfile(string classname, vector<pair<string, string>> classdata)
{
	string filename = classname + "Controller.h";
	ofstream out(filename, ofstream::out);
	string templatestr =
		"#pragma once\n"
		"#include \"Controller.h\"\n"
		"class %classname% : public Controller{\n"
		"public:\n"
		"  %classname%();\n"
		"  ~%classname%();\n"
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
	string filename = classname + "Controller.cpp";
	ofstream fout(filename, ofstream::out);
	string templatestr =
		"#include \"%classname%Controller.h\"\n\n"
		"%classname%():[constructorinit] {\n"
		"  \n"
		"}\n"
		"~%classname%(){\n"
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
		getmas += "" + classdata[i].second + " get" + classdata[i].first + "() {\n";
		getmas += "  return " + classdata[i].first + ";\n}\n";
	}
	templatestr.replace(templatestr.find("[getmas]"), 8, getmas);
	fout << templatestr;
	fout.close();
}
void wrtitetocf(vector<pair<string, vector<pair<string, string>>>> classmas)
{
	ofstream out("configFactory.cpp", ofstream::out);
	string templatestr =
		"[incmas]\n"
		"enum device[enummas]\n"
		"controller* create(device d) {\n"
		"  switch (d){\n"
		"[casemas]"
		"  default:\n"
		"    return nullptr;\n"
		"  break;\n"
		"  }\n"
		"}";
	string incmas = "";
	for (int i = 0; i < classmas.size(); ++i)
	{
		incmas += "#include \"" + classmas[i].first + "\".h\n";
	}
	templatestr.replace(templatestr.find("[incmas]"), 8, incmas);
	string enummas = "{";
	for (int i = 0; i < classmas.size(); ++i)
	{

		if (i != classmas.size() - 1)
			enummas += " " + classmas[i].first + ",";
		else
			enummas += " " + classmas[i].first + " };";
	}
	templatestr.replace(templatestr.find("[enummas]"), 9, enummas);
	string casemas = "";
	for (int i = 0; i < classmas.size(); ++i)
	{
		casemas += "    case " + classmas[i].first + ":\n";
		casemas += "      " + classmas[i].first + " *dev=new " + classmas[i].first + "();\n";
		for (int j = 0; j < classmas[i].second.size(); ++j)
		{
			casemas += "      " + classmas[i].first + "." + classmas[i].second[j].first + " =getjson(" + classmas[i].second[j].first + "); \n";
		}
		casemas += "      return dev;\n";
		casemas += "      break;\n";
	}
	templatestr.replace(templatestr.find("[casemas]"), 9, casemas);
	out << templatestr;
	out.close();
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
	
	
	
	
	cin >> n;
}