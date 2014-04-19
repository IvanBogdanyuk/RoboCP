#pragma once
#include "ClientReceiver.h"


ClientReceiver::ClientReceiver(XMLConfig * x)
{
  port = x->CommandPort; // Reading port from config
}


ClientReceiver::~ClientReceiver(void)
{
}

void ClientReceiver::Start ()
{
  try {
    boost::asio::io_service io_service;
	tcp::endpoint endpoint (tcp::v4 (), atoi(port.c_str()) );
    tcp::acceptor acceptor (io_service, endpoint);

    tcp::iostream socketStream;

    cout << "ClientReceiver: Waiting for connection.." << endl; //TODO: write in log
	RAW_LOG (INFO, "ClientReceiver: Waiting for connection..");
	
    acceptor.accept (*socketStream.rdbuf ()); // waiting from connection from any IP

    cout << "ClientReceiver: Connected!" << endl; //TODO: write in log
	RAW_LOG (INFO, "ClientReceiver: Connected!");
	
	boost::archive::xml_iarchive ia(socketStream); // We will receive objects in XML
	Command com;

	while (!socketStream.fail() ) {
	  ia >> BOOST_SERIALIZATION_NVP(com);
      cout << "New command: " << com.ComType << " " << com.ComCondition << " " << com.Value << endl; // TODO: command buffer
	  RAW_LOG (INFO, "New command: %d %d %f", com.ComType, com.ComCondition, com.Value);
	}
  
  }
  catch (exception& e) {
    cout << "ClientReceiver: Exception: " << e.what () << endl; //TODO: write in log
	RAW_LOG (INFO, "ClientReceiver: Exception: %s", e.what());
  }
}