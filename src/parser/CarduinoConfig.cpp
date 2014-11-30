//WARNING!!!!!!!! This is a template file. Do not change it without necessity.
#include "CarduinoConfig.h"

CarduinoConfig::CarduinoConfig():Port("")  {
  
}
CarduinoConfig::~CarduinoConfig(){
  
}
string CarduinoConfig::getPort() {
  return Port;
}