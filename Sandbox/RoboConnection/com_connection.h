#pragma once

#include <iostream>
#include "mavlink_vis.h"
#include <QtSerialPort\QSerialPort>
#include <QtSerialPort\QSerialPortInfo>

class ComConnection{
public:
	ComConnection();
	void openPort(QString name);
	void Write(unsigned char* message, int length);
	QByteArray readPacket();
	void getParamList();
private:
	QSerialPort serial;
	bool com_checksum(uint8_t *data, int32_t length, uint8_t com_ck_a, uint8_t com_ck_b); //Gets an array, length of it payload 
	
};

