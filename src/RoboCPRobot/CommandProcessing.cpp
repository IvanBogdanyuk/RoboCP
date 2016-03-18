#include "CommandProcessing.h"


CommandProcessing::CommandProcessing(CommandBuffer *buf)
{
  buffer = buf;
}

CommandBuffer *CommandProcessing::GetBuffer(void)
{
  return buffer;
}

CommandProcessing::~CommandProcessing(void)
{
}

void CommandProcessing::Start(void)
{

}
