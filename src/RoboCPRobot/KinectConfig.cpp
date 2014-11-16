#include "KinectConfig.h"

KinectConfig::KinectConfig():Port(0)  {
  
}
KinectConfig::~KinectConfig(){
  
}
int KinectConfig::getPort() {
  return Port;
}
