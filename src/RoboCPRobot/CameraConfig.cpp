#include "CameraConfig.h"

CameraConfig::CameraConfig():Number(0), FramesPerSecond(0), FrameWidth(0), FrameHeight(0)  {
  
}
CameraConfig::~CameraConfig(){
  
}
int CameraConfig::getNumber() {
  return Number;
}
int CameraConfig::getFramesPerSecond() {
  return FramesPerSecond;
}
int CameraConfig::getFrameWidth() {
  return FrameWidth;
}
int CameraConfig::getFrameHeight() {
  return FrameHeight;
}
