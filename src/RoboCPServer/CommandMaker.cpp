#pragma once
#include "CommandMaker.h"


CommandMaker::CommandMaker (XMLConfig * x)
{
  ip = x->IP;
  port = x->CommandPort;
}

CommandMaker::~CommandMaker ()
{
}

void CommandMaker::Start ()
{
  try {
    tcp::iostream socketStream (ip.c_str(), port.c_str() );

	if (!socketStream.fail() ) {
      cout << "CommandMaker: Connected!" << endl;
	  
	  while (!socketStream.fail() ) {
		Command com;
		cout << "input sec (int):" << endl;
		cin >> com.sec;
		cout << "input angle (float):" << endl;
		cin >> com.angle;
		boost::archive::xml_oarchive oa(socketStream);
		oa << BOOST_SERIALIZATION_NVP(com);
	  }
	
	}
  }
  catch (exception& e) {
    cout << "CommandMaker: Exception: " << e.what () << endl;
  }
}
