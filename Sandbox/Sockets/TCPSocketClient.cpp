#include <iostream>
#include <string>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;


class SocketClient {
protected:
  tcp::iostream socketStream;
  stringstream * buffer;
  string address;
  unsigned short port;

public:
  SocketClient (string IP, unsigned short prt, stringstream * buf) {
	  address = IP;
	  port = prt;
	  buffer = buf;
  }

  void start ();
};


class SocketClientListener : public SocketClient {
public:
  SocketClientListener (string IP, unsigned short prt, stringstream * buf) : SocketClient (IP, prt, buf) {}

  void start () {
    try {
	  char itoabuf[8];
	  itoa(port,itoabuf,10);
	  socketStream.connect (address.c_str(), itoabuf);

	  if (!socketStream.fail())
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
	  cout << "Exception: " << e.what () << endl;
	}
  }
};


class SocketClientSender : public SocketClient {
public:
  SocketClientSender (string IP, unsigned short prt, stringstream * buf) : SocketClient (IP, prt, buf) {}

  void start () {
    try {
	  char itoabuf[8];
	  itoa(port,itoabuf,10);
	  socketStream.connect (address.c_str(), itoabuf);

	  if (!socketStream.fail())
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
	  cout << "Exception: " << e.what () << endl;
	}
  }
};


int main (int argc, char** argv) {
	stringstream buffer;
	string str = "127.0.0.1";
	unsigned short port = 6666;

	//SocketClientListener l (str, port, &buffer);
	SocketClientSender l (str, port, &buffer);
	l.start();
	
	return (0);
}