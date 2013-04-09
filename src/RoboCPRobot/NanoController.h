#pragma once
#include "Controller.h"
#include "NanoReceivedBuffer.h"
class NanoController :
  public Controller
{
private:
  NanoReceivedBuffer* buffer;
public:
  NanoReceivedBuffer *GetBuffer(void);
  NanoController(void);
  ~NanoController(void);
};

