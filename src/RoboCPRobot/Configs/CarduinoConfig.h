/*








//WARNING!!!!!!!! This is a template file. Do not change it without necessity.










*/
#pragma once
#include "Config.h"
#include "configFactory.h"
class CarduinoConfig : public Config{
public:
  CarduinoConfig();
  friend class configFactory;
  ~CarduinoConfig();
  string getPort();

private:
  string Port;
};