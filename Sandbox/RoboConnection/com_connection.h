#pragma once

#include <iostream>
#include "mavlink_vis.h"
#include <QtSerialPort\QSerialPort>
#include <QtSerialPort\QSerialPortInfo>

class ComConnection
{
public:
    ComConnection();
    void openPort(QString name);
    void Write(unsigned char* message, int length);
    QByteArray readPacket();
private:
    QSerialPort serial;
    bool com_checksum(uint8_t *data, int32_t length); //Gets an array, length of it payload 
};  