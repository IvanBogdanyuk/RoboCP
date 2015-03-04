#pragma once
#include "SenderBase.h"
#include "Send.h"
#include "SendBuffer.h"
#include "XMLConfig.h"
#include <boost/asio.hpp>
#include "QtCore\qsharedpointer.h"
#include "QtNetwork\qtcpserver.h"
#include "QtNetwork\qtcpsocket.h"

#ifdef ENABLE_LOGGING
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <glog/raw_logging.h>
#endif

using boost::asio::ip::tcp;
using namespace std;

//Class purpose: taking Send object from buffer and sending it via TCP-IP
class SendSender :
  public SenderBase
{
public:
  void Start(); 
  SendSender(XMLConfig * x, SendBuffer * buf);
  ~SendSender(void);
private:
  SendBuffer * buffer;
  int port;
};

