#pragma once
#include "XMLConfig.cpp"
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <string>

#include <boost/asio.hpp>


using boost::asio::ip::tcp;

using namespace std;


class Command {
private:
  friend class boost::serialization::access;
 
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
	ar & BOOST_SERIALIZATION_NVP(sec);
	ar & BOOST_SERIALIZATION_NVP(angle);
  }

public:
  int sec;
  float angle;
  Command () {
    sec = 0;
	angle = 0;
  }
  Command (int s) {
    sec = s;
	angle = 0;
  }
  Command (float a) {
    sec = 0;
	angle = a;
  }
  Command (int s, float a) {
    sec = s;
	angle = a;
  }
};


class CommandMaker {
private:
  string IP;
  string port;

public:
  void run () {
	try {
	  tcp::iostream socketStream (IP.c_str(), port.c_str() );

	  if (!socketStream.fail() )
        cout << "CommandMaker Connected!" << endl;
	  
	  while (!socketStream.fail() ) {
		Command com;
		cout << "input sec (int):" << endl;
		cin >> com.sec;
		cout << "input angle (float):" << endl;
		cin >> com.angle;
		boost::archive::xml_oarchive oa(socketStream);
		oa << BOOST_SERIALIZATION_NVP(com);
	  }
	}
	catch (exception& e) {
      cout << "CommandMaker Exception: " << e.what () << endl;
    }
  }
  CommandMaker (XMLConfig c) {
	IP = c.IP;
	port = c.CommandPort;
  }
};