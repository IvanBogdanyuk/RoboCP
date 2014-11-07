#pragma once
#include "CommandMaker.h"


CommandMaker::CommandMaker (XMLConfig * x)
{
  ip = x->IP; // Reading IP from config
  port = x->CommandPort; // Readin port from config
}

CommandMaker::~CommandMaker ()
{
}

void CommandMaker::Start ()
{
  try {
    tcp::iostream socketStream (ip.c_str(), port.c_str() ); // Trying to connect

	if (!socketStream.fail() ) {
      cout << "CommandMaker: Connected!" << endl; // TODO: write in log
      #ifdef ENABLE_LOGGING
	  RAW_LOG (INFO, "CommandMaker: Connected!");
	  #endif
	  boost::archive::xml_oarchive oa(socketStream); // We want to send commands in XML
	  Command com;

	  while (!socketStream.fail() ) {
		// Reading command
		cout << "input command type (int):" << endl;
		cin >> com.ComType;
		cout << "input command condition (int):" << endl;
        cin >> com.ComCondition;
		cout << "input condition value (float):" << endl;
        cin >> com.Value;

		oa << BOOST_SERIALIZATION_NVP(com); // Sending command
	  }
	
	}
  }
  catch (exception& e) {
    cout << "CommandMaker: Exception: " << e.what () << endl; // TODO: write in log
    #ifdef ENABLE_LOGGING
	  RAW_LOG (INFO, "CommandMaker: Exception: %s", e.what());
    #endif
  }
}
