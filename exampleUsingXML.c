#pragma once

#include "XML/irrXML.h"

using namespace irr;
using namespace io;

bool ParseConfigCommand(vector<BaseTest*>& testList, int argc, _TCHAR* argv[]);

int _tmain(int argc, _TCHAR* argv[])
{
	vector<BaseTest*> testList;

	if(argc > 1)
	{
		bool isValid = true;
		char* command = argv[1];
		while(true)
		{
			if(!strcmp(command, "-config"))
			{
				isValid &= ParseConfigCommand(testList, argc, argv);
			}
			else
			{
				printf("Invalid execution format\n");
				isValid = false;
			}

			break;
		}
		if(!isValid)
		{
			PrintHelp();
			return -1;
		}
	}
}

bool ParseConfigCommand(vector<BaseTest*>& testList, int argc, _TCHAR* argv[])
{
	if(argc != 3)
		return false;

	char* configFile = argv[2];
	IrrXMLReader* xml = createIrrXMLReader(configFile);
	bool isError = false;
	// Getting root tag
	if(!GetNextXmlElement(xml, isError))
		return false;
	if(strcmp(xml->getNodeName(), "TestConfig") != 0)
	{
		printf("Unexpected root tag '%s'", xml->getNodeName());
		return false;
	}

	if(!ParseXmlTests(xml, testList))
		return false;

	delete xml;
	return true;
}