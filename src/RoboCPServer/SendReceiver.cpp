#pragma once
#include "SendReceiver.h"


SendReceiver::SendReceiver (XMLConfig * x, SendBuffer * buf)
{
  ip = x->IP;
  port = x->SendPort;
  buffer = buf;
}

SendReceiver::~SendReceiver ()
{
}

void SendReceiver::Start ()
{
  try {
    tcp::iostream socketStream (ip.c_str(), port.c_str() );

    if (!socketStream.fail() ) {
      cout << "SendReceiver: Connected!" << endl;

	boost::archive::xml_iarchive ia(socketStream);

	  while ( true ) {

		boost::shared_ptr<Send> sendData (new Send);
		ia >> BOOST_SERIALIZATION_NVP(sendData);
		buffer->Enqueue (sendData);

	  }
	
	}
  }
  catch (exception& e) {
    cout << "KinectViewer: Exception: " << e.what () << endl;
  }
}