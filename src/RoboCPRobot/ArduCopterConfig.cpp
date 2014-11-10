#pragma once
#include "ArduCopterConfig.h"

ArduCopterConfig::ArduCopterConfig(void)
{
}

ArduCopterConfig::~ArduCopterConfig(void)
{
}

std::string ArduCopterConfig::getPort()
{
	return Port;
}

void ArduCopterConfig::setPort(std::string Port)
{
	this->Port = Port;
}