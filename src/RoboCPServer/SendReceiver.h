#pragma once
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include "XMLConfig.h"
#include "Send.h"
#include "SendBuffer.h"
#include "KinectViewer.h"

#include <boost/asio.hpp>

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <glog/raw_logging.h>

using boost::asio::ip::tcp;
using namespace std;

// Class purpose: receivig Send objects and updating KinectViewer info
class SendReceiver
{
public:
  void Start ();
  SendReceiver (XMLConfig * x, KinectViewer* v);
  ~SendReceiver (void);
private:
  string ip;
  string port;
  KinectViewer* kinectViewer;
  void updateText (boost::shared_ptr<Send> s);
};