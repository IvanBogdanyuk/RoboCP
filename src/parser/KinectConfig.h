//WARNING!!!!!!!! This is a template file. Do not change it without necessity.
#pragma once
#include "Config.h"
#include "configFactory.h"
class KinectConfig : public Config{
public:
  KinectConfig();
  friend class configFactory;
  ~KinectConfig();
  int getPort();

private:
  int Port;
};