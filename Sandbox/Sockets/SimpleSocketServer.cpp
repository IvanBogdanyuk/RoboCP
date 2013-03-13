#include <iostream>
#include <string>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;


int main (int argc, char** argv) {
  boost::asio::io_service io_service;
  tcp::endpoint endpoint(tcp::v4(), 6666);
  tcp::acceptor acceptor (io_service, endpoint);
  
  tcp::iostream socketStream;

  cout << "Waiting for connection.." << endl;
  acceptor.accept (*socketStream.rdbuf ());

  cout << "Connected!" << endl;

  while (!socketStream.fail()) {
    string line;
    socketStream >> line;
    cout << line << endl;
  }

  cout << "Disconnected!" << endl;
	
  return (0);
}