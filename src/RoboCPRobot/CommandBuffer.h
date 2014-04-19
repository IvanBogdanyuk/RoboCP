#pragma once
#include "ReceivedBuffer.h"
#include "Command.h"

class CommandBuffer :  public ReceivedBuffer <boost::shared_ptr<Command>>
{
public:
  CommandBuffer (int size) : ReceivedBuffer <boost::shared_ptr<Command>> (size) {}
};

