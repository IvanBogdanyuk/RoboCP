#include "CommandConfig.h"

CommandConfig::CommandConfig():Port(0)  {
  
}
CommandConfig::~CommandConfig(){
  
}
int CommandConfig::getPort() {
  return Port;
}
