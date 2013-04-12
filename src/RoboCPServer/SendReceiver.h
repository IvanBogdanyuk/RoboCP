#pragma once
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include "XMLConfig.h"
#include "Send.h"
#include "SendBuffer.h"

#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;

class SendReceiver
{
public:
  void Start ();
  SendReceiver (XMLConfig * x, SendBuffer * buf);
  ~SendReceiver (void);
private:
  string ip;
  string port;
  SendBuffer * buffer;
};