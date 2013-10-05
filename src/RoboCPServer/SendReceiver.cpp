#pragma once
#include "SendReceiver.h"


SendReceiver::SendReceiver (XMLConfig * x, SendBuffer * b)
{
  ip = x->IP; // Reading IP from config
  port = x->SendPort; // Reading port from config
  sendBuffer = b;
}

SendReceiver::~SendReceiver ()
{
}

void SendReceiver::Start ()
{
  try {
    tcp::iostream socketStream (ip.c_str(), port.c_str() ); // Trying to connect

    if (!socketStream.fail() ) {
      cout << "SendReceiver: Connected!" << endl; // TODO: write in log
      #ifdef ENABLE_LOGGING
	    RAW_LOG (INFO, "SendReceiver: Connected!");
      #endif
	    Sleep(6000);

	    while ( true ) {
		    boost::archive::xml_iarchive ia(socketStream); // We will receive Send objects in XML
		    boost::shared_ptr<Send> sendData (new Send);  // Creating new Send object
		    ia >> BOOST_SERIALIZATION_NVP(sendData); // Receiving
	    	sendBuffer->Enqueue (sendData); // Adding Send in buffer
	    }
	  }
  }
  catch (exception& e) {
    cout << "SendReceiver: Exception: " << e.what () << endl; // TODO: write in log
    #ifdef ENABLE_LOGGING
	  RAW_LOG (INFO, "SendReceiver: Exception: %s", e.what());
    #endif
  }
}
