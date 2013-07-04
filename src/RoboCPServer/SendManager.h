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

#ifdef ENABLE_LOGGING
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <glog/raw_logging.h>
#endif

using boost::asio::ip::tcp;
using namespace std;

/*
enum ArduCopterReceivedType{
  AccelerationPacket,
  AnglesPacket,
  AltitudePacket
};
*/

// Class purpose: taking Send objects from buffer and updating KinectViewer info
class SendManager
{
public:
  void Start ();
  SendManager (SendBuffer* b, KinectViewer* v);
  ~SendManager (void);
private:
  SendBuffer* sendBuffer;
  KinectViewer* kinectViewer;
  void updateText (boost::shared_ptr<Send> s);
};