#include "CommandProcessing.h"


CommandProcessing::CommandProcessing(XMLConfig *x, CommandBuffer *buf)
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
