#pragma once
#include "XMLConfig.h"
#include "Command.h"
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <string>

#include <boost/asio.hpp>

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <glog/raw_logging.h>


using boost::asio::ip::tcp;

using namespace std;

// Class purpose: reading commands from console and sending them to robot
class CommandMaker
{
public:
  void Start ();
  CommandMaker(XMLConfig * x);
  ~CommandMaker(void);
private:
  string ip;
  string port;
};