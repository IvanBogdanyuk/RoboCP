#include "CarduinoConfig.h"

CarduinoConfig::CarduinoConfig():Port("")  {
  
}
CarduinoConfig::~CarduinoConfig(){
  
}
string CarduinoConfig::getPort() {
  return Port;
}
