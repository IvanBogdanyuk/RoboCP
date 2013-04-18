#include "SendSender.h"


SendSender::SendSender (XMLConfig * x, SendBuffer * buf)
{
  port = atoi(x->SendPort.c_str() );
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

    cout << "SendSender: Waiting for connection.." << endl;

    acceptor.accept (*socketStream.rdbuf ());

    cout << "SendSender: Connected!" << endl;


	boost::archive::xml_oarchive oa (socketStream);

	while (!socketStream.fail() ) {
	  boost::shared_ptr<Send> sendData;
	  sendData = buffer->Dequeue();
	  oa << BOOST_SERIALIZATION_NVP (sendData);
	}

  }
  catch (exception& e) {
    cout << "SendSender: Exception: " << e.what () << endl;
  }

}