/*








//WARNING!!!!!!!! This is a template file. Do not change it without necessity.










*/
#pragma once
#include "Config.h"
#include "configFactory.h"
class SendConfig : public Config{
public:
  SendConfig();
  friend class configFactory;
  ~SendConfig();
  int getPort();
  bool getIsAvailable();

private:
  int Port;
  bool IsAvailable;
};