#include "parser.h"

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
		tempMap[names[i]] = s;
		in.close();
	}
	delete[] text;
}

