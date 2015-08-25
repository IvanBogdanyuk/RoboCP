/*








//WARNING!!!!!!!! This is a template file. Do not change it without necessity.










*/
#pragma once
#include "Config.h"
#include "configFactory.h"
class ArducopterConfig : public Config{
public:
  ArducopterConfig();
  friend class configFactory;
  ~ArducopterConfig();
  string getPort();
  bool getIsAvailable();
  bool getDoFakeStart();

private:
  string Port;
  bool IsAvailable;
  bool DoFakeStart;
};