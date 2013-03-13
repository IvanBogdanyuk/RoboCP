#include <iostream>
#include <string>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;


int main (int argc, char** argv) {
  tcp::iostream socketStream ("127.0.0.1", "6666");

  if (!socketStream.fail())
    cout << "Connected!" << endl;
  
  while (!socketStream.fail()) {
    string line;
    cout << "line to send?" << endl;
    cin >> line;
    socketStream << line << endl;
    socketStream.flush();
  }

  cout << "Disconnected!" << endl;
	
  return (0);
}