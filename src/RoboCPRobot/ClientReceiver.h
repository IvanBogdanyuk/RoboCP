#pragma once
#include "XMLConfig.h"
#include "Command.h"
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <string>

#include <boost/asio.hpp>


using boost::asio::ip::tcp;

using namespace std;


// Class purpose: Receiving commands from server
class ClientReceiver
{
public:
  void Start ();
  ClientReceiver(XMLConfig * x);
  ~ClientReceiver(void);
private:
  string port;
};

