#include "SendSender.h"


SendSender::SendSender (XMLConfig * x, SendBuffer * buf)
{
  port = atoi(x->SendPort.c_str() ); //reading port from config
  buffer = buf;
}

SendSender::~SendSender (void)
{
}

void SendSender::Start ()
{
  try {
    boost::asio::io_service io_service;
	tcp::endpoint endpoint (tcp::v4 (), port);
    tcp::acceptor acceptor (io_service, endpoint);

    tcp::iostream socketStream;

    cout << "SendSender: Waiting for connection.." << endl; //TODO: write in log
	RAW_LOG (INFO, "SendSender: Waiting for connection..");

    acceptor.accept (*socketStream.rdbuf ()); // waiting from connection from any IP

    cout << "SendSender: Connected!" << endl; //TODO: write in log
	RAW_LOG (INFO, "SendSender: Connected!");

	boost::archive::xml_oarchive oa (socketStream); // We want to send objects in XML

	while (!socketStream.fail() ) {
	  boost::shared_ptr<Send> sendData;
	  sendData = buffer->Dequeue(); // Reading Send object from buffer
	  oa << BOOST_SERIALIZATION_NVP (sendData); // Serializing and sending it
	}

  }
  catch (exception& e) {
    cout << "SendSender: Exception: " << e.what () << endl; //TODO: write in log
	RAW_LOG (INFO, "SendSender: Exception: %s", e.what());
  }

}