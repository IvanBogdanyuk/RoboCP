#pragma once
#include "Controller.h"
#include <time.h>
#include "XMLConfig.h"
#include "CommandBuffer.h"

class CommandProcessing : public Controller
{
private:
  CommandBuffer *buffer;
public:
  void Start(void);
  CommandBuffer *GetBuffer(void);
  CommandProcessing(XMLConfig *x, CommandBuffer *buf);
  ~CommandProcessing(void);
};

