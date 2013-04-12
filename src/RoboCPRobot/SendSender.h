#pragma once
#include "SenderBase.h"
#include "Send.h"
#include "SendBuffer.h"
#include "XMLConfig.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;


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

