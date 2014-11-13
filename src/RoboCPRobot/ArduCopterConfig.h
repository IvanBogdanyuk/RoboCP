#pragma once
#include "Config.h"

class ArduCopterConfig : public Config
{
  public:
    bool IsAvailable;
    bool DoFakeStart;
    std::string getPort(void);
    ArduCopterConfig(void);
    ~ArduCopterConfig(void);
	void setPort(std::string Port);

private:
	std::string Port;
};