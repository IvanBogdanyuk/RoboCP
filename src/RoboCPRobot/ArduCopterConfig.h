#pragma once
#include "Config.h"

class ArduCopterConfig : public Config
{
  public:
    bool IsAvailable;
    bool DoFakeStart;
    void setPort(std::string Port);
    std::string getPort(void);
    ArduCopterConfig(void);
    ~ArduCopterConfig(void);

private:
	std::string Port;
};