#pragma once
#include "ReceivedBuffer.h"
#include "NanoReceived.h"
class NanoReceivedBuffer :
  public ReceivedBuffer
{
public:
  void Enqueue(NanoReceived);
  NanoReceived Dequeue();
  NanoReceivedBuffer(void);
  ~NanoReceivedBuffer(void);
};

