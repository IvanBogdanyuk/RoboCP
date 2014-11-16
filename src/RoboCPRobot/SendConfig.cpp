#include "SendConfig.h"

SendConfig::SendConfig():Port(0), IsAvailable(false)  {
  
}
SendConfig::~SendConfig(){
  
}
int SendConfig::getPort() {
  return Port;
}
bool SendConfig::getIsAvailable() {
  return IsAvailable;
}
