#pragma once
#include "ClientReceiver.h"


ClientReceiver::ClientReceiver(XMLConfig * x)
{
  port = x->CommandPort;
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

    std::cout << "ClientReceiver: Waiting for connection.." << std::endl;

    acceptor.accept (*socketStream.rdbuf ());

    std::cout << "ClientReceiver: Connected!" << std::endl;
	  
	while (!socketStream.fail() ) {
	  Command com;
	  boost::archive::xml_iarchive ia(socketStream);
	  ia >> BOOST_SERIALIZATION_NVP(com);
    cout << "New command: " << com.ComType << " " << com.ComCondition << " " << com.Value << endl; //облака в буфер оттуда отправлять
	 }
  
  }
  catch (exception& e) {
    cout << "ClientReceiver: Exception: " << e.what () << endl;
  }
}