#pragma once
#include "Controller.h"
#include <time.h>

#include "CommandBuffer.h"

class CommandProcessing : public Controller
{
private:
  CommandBuffer *buffer;
public:
  void Start(void);
  CommandBuffer *GetBuffer(void);

  CommandProcessing(CommandBuffer *buf);
  ~CommandProcessing(void);
};

