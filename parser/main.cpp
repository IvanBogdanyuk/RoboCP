#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;
///finds first name from string from poistion=pos
void writetohfile(string classname, vector<pair<string, string>> classdata);
void writetocppfile(string classname, vector<pair<string, string>> classdata);
pair<string,int> getName(string s, int pos)
{
	int namestart = s.find('"', pos);

	if ((namestart > s.find('}',pos)) || (namestart==-1))
		return make_pair("",-1);
	int nameend = s.find('"', namestart + 1);
	return make_pair(s.substr(namestart + 1, nameend - 1 - namestart),nameend);
}
pair<pair<string,string>, int> getKeyValue(string s, int pos)
{
	if (s.find('"', pos) > s.find('}') || (s.find('"', pos) == -1))
		return make_pair(make_pair("", ""),-1);
	pair<string, int> key = getName(s, pos);
	if (key.second==-1)
		return make_pair(make_pair("", ""), -1);
	pos = s.find(':', pos)+1;
	pair<string, int> value = getName(s, pos); 
	if (value.second == -1)
		return make_pair(make_pair("", ""), -1);
	return make_pair(make_pair(key.first, value.first), pos + value.first.size() + 2);
}
void parse(string &s)
{
	vector<pair<string, vector<pair<string,string>>>> classmas;
	char c = '1';
	int pos = 0;
	string classname;
	string classstr;
	while (1)
	{
		pair<string,int> classname = getName(s,pos);
		if (classname.second == -1)
			break;
		pos +=classname.first.size() + 2;
		int begin = s.find("{", pos);
		int end = s.find("}", pos);
		classstr = s.substr(begin, end-begin);
		vector<pair<string,string>> classdata;
		int i = 0;
		while (1)
		{
			pair<pair<string, string>, int> kv = getKeyValue(classstr, i);
			if (kv.second == -1)
				break;
			classdata.push_back(kv.first);
			i = kv.second+1;
		}
		pos = end+1;
		classmas.push_back(make_pair(classname.first, classdata));
	}
	for (int i = 0; i < classmas.size(); ++i)
	{
		writetohfile(classmas[i].first, classmas[i].second);
		writetocppfile(classmas[i].first, classmas[i].second);
	}
	return;
}
void writetohfile(string classname, vector<pair<string, string>> classdata)
{
	string filename =classname + "Controller.h";
	ofstream out(filename, ofstream::out);
	out << "#pragma once"<<endl;
	out << "#include \"Controller.h\"" << endl;
	out << "class " << classname << ": public Controller{" << endl;
	out << "public:" << endl;
	//constructor & destructor	
	out << "  "<<classname<<"(): ";
	for (int i = 0; i < classdata.size(); ++i)
	{
		out << classdata[i].first << "(";
		if (classdata[i].second == "int")
			out << "0";
		else
		if (classdata[i].second == "double")
			out << "0.0";
		else
		if (classdata[i].second == "string")
			out << "\"\"";
		else
		if (classdata[i].second == "bool")
			out << "false";
		if (i != classdata.size() - 1)
		{
			out << "), ";
		}
		else
			out << ") {};" << endl;
	}
	out << "  ~" << classname << "();"<<endl;
	//constructor & destructor
	
	//get,set
	out << "  void set(string key, string value);"<<endl;
	out << "  template<class T>"<<endl;
	out << "  T get(string key);" << endl;
	//get,set

	//data members
	out << "private:" << endl;
	for (int i = 0; i < classdata.size(); ++i)
	{
		out << "  " << classdata[i].second << " " << classdata[i].first << ";"<<endl;
	}
	//data members
	
	out << "};";
	out.close();
	
}
void writetocppfile(string classname, vector<pair<string, string>> classdata)
{
	string filename = classname + "Controller.cpp";
	ofstream fout(filename, ofstream::out);
	fout << "#include \"" << classname << "Controller.h\"" << endl;
	fout << "template<class T>" << endl;
	fout << "T parse(string value, string type){ " << endl;
	fout << "  if( type == \"int\" )" << endl;
	fout << "    try{" << endl;
	fout << "      int t=stoi(value);" << endl;
	fout << "      return t;" << endl;
	fout << "    }" << endl;
	fout << "    catch{" << endl;
	fout << "      return -1;" << endl;
	fout << "    }" << endl;
	fout << "  else" << endl;
	fout << "  if( type == \"double\" )" << endl;
	fout << "    return strtod(value, nullptr);" << endl;
	fout << "  else" << endl;
	fout << "  if( type == \"bool\" )" << endl;
	fout << "    return value == \"true\";" << endl;
	fout << "  else" << endl;
	fout << "  if( type ==\ \"string\" )" << endl;
	fout << "    return value;" << endl;
	fout << "}" << endl;
	for (int i = 0; i < classdata.size(); ++i)
	{

	}
	//set
	fout << "void "<<classname<<"::set(string key, string value) {" << endl;
	for (int i = 0; i < classdata.size(); ++i)
	{
		fout << "  if ( key == " << "\"" << classdata[i].first << "\" ) " << endl;
		fout << "    " << classdata[i].first << " = parse<" << classdata[i].second << ">(value, \""<<classdata[i].second <<"\");" << endl;
		if (i != classdata.size() - 1)
			fout << "  else" << endl;
	}
	fout << "}" << endl;
	//set

	//get
	fout << "template<class T>" << endl;
	fout << "T " << classname << "::get(string key){" << endl;
	for (int i = 0; i < classdata.size(); ++i)
	{
		fout << "  if ( key == " << "\"" << classdata[i].first << "\" ) " << endl;
		fout << "    return " << classdata[i].first << ";" << endl;
	}
	fout << "}";
	fout.close();
}
int main()
{
    int n;
	ifstream in("classconfig.json", ifstream::in);
	char *text=new char[10000];
	char c = '1';
	int len = 0;
	while (in.good())
	{
		c=in.get();
		text[len++] = c;
	}
	text[len] = '\0';
	string s(text);
	parse(s);
	
	cin >> n;
}