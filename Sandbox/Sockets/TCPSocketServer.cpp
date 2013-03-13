#include <iostream>
#include <string>
#include <conio.h>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;



class SocketServer {
protected:
  boost::asio::io_service io_service;
  tcp::endpoint endpoint;
  tcp::iostream socketStream;
  tcp::acceptor * acceptor;
  stringstream * buffer;

public:
  SocketServer (string address, unsigned short port, stringstream * buf) {
    buffer = buf;
    endpoint.address (boost::asio::ip::address::from_string(address));
    endpoint.port(port);
    acceptor = new tcp::acceptor (io_service, endpoint);
  }

  ~SocketServer () {
    delete acceptor;
  }

  void start ();

};


class SocketServerListener  : public SocketServer {
public:
  SocketServerListener (string address, unsigned short port, stringstream * buf) : SocketServer (address, port, buf) {}

  void start () {
    try {
      cout << "Waiting for connection.." << endl;

	  acceptor->accept (*socketStream.rdbuf ());
			
	  cout << "Connected!" << endl;

	  //main loop
	  while (!socketStream.fail()) {
		string line;
		socketStream >> line;
		*buffer << line;
		cout << "receive this: " << line << endl;
	  }

      cout << "Disconnected!" << endl;
	}
	catch (std::exception& e) {
	  cerr << e.what () << endl;
	}

  }
};


class SocketServerSender  : public SocketServer {
public:
  SocketServerSender (string address, unsigned short port, stringstream * buf) : SocketServer (address, port, buf) {}

  void start () {
    try {
      cout << "Waiting for connection.." << endl;

	  acceptor->accept (*socketStream.rdbuf ());
			
	  cout << "Connected!" << endl;

	  //main loop
	  while (!socketStream.fail()) {
		string line;
		//don't have buffer yet, so just taking line from console 
		//*buffer >> line;                           
		cout << "what do u wanna send?" << endl;
		cin >> line;

		socketStream << line << endl;
		socketStream.flush();
	  }

      cout << "Disconnected!" << endl;
	}
	catch (std::exception& e) {
	  cerr << e.what () << endl;
	}

  }
};




int main (int argc, char** argv) {	
	stringstream buffer;
	string str = "127.0.0.1";
	unsigned short port = 6666;

	SocketServerListener l (str, port, &buffer);
	//SocketServerSender l (str, port, &buffer);
	l.start();

	return (0);
}