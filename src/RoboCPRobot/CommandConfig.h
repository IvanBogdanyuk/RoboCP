#pragma once
#include "Config.h"
#include "configFactory.h"
class CommandConfig : public Config{
public:
  CommandConfig();
  friend class configFactory;
  ~CommandConfig();
  int getPort();

private:
  int Port;
};