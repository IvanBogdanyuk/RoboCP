/*








//WARNING!!!!!!!! This is a template file. Do not change it without necessity.










*/

#include "ArducopterConfig.h"

ArducopterConfig::ArducopterConfig():Port(""), IsAvailable(false), DoFakeStart(false)  {
  
}
ArducopterConfig::~ArducopterConfig(){
  
}
string ArducopterConfig::getPort() {
  return Port;
}
bool ArducopterConfig::getIsAvailable() {
  return IsAvailable;
}
bool ArducopterConfig::getDoFakeStart() {
  return DoFakeStart;
}