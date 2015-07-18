#include "com_connection.h"
#include <iostream>

ComConnection::ComConnection()
{
    //initializing connection
}
void ComConnection::openPort(QString name)
{
    serial.setPortName(name);
    bool errorFlag = true;

    errorFlag &= serial.setBaudRate(QSerialPort::Baud57600); //проверка скорости
    
    errorFlag &= serial.open(QIODevice::ReadWrite);  //проверка того, что устройство открыто

    if (!errorFlag) 
        std::cout << "error during com connection initialization \n";
}
uint16_t crc_accumulate(uint8_t b, uint16_t crc)
{
    uint8_t ch = (uint8_t)(b ^ (uint8_t)(crc & 0x00ff));
    ch = (uint8_t)(ch ^ (ch << 4));
    return (uint16_t)((crc >> 8) ^ (ch << 8) ^ (ch << 3) ^ (ch >> 4));
}

uint16_t crc_calculate(uint8_t *pBuffer, int length)
{
    if (length < 1)
    {
        return 0xffff;
    }

    uint16_t crcTmp;
    int i;

    crcTmp = 0xffff;

    for (i = 1; i < length; i++) // skips header
    {
        crcTmp = crc_accumulate(pBuffer[i], crcTmp);
    }

    return (crcTmp);
}

void ComConnection::Write(unsigned char* message, int length)
{
    serial.write((char*)message, length);
}
QByteArray ComConnection::readPacket()
{
    if (!serial.isOpen())
        return "";
    QByteArray buffer;
    int count = 0;
    int length = 0;
    int readcount = 0;
    QString buildplaintxtline, plaintxtline;
    while (serial.isOpen())
    {
        if (readcount > 300)
        {
            std::cout << "MAVLink readpacket No valid mavlink packets";
            break;
        }
        readcount++;
        if (serial.waitForReadyRead(-1))
            buffer.replace(count, 1, serial.read(1));
        // check if looks like a mavlink packet and check for exclusions and write to console
        if ((unsigned char)buffer[0] != 254 && buffer[0] != 'U')
        {
            if ((unsigned char)buffer[0] >= 0x20 && (unsigned char)buffer[0] <= 127 || buffer[0] == '\n' || buffer[0] == '\r')
            {
                // check for line termination
                if (buffer[0] == '\r' || buffer[0] == '\n')
                {
                    // check new line is valid
                    if (buildplaintxtline.length() > 3)
                        plaintxtline = buildplaintxtline;
                    // reset for next line
                    buildplaintxtline = "";
                }
                buildplaintxtline += (char)buffer[0];
            }
            count = 0;
            buffer[1] = 0;
            continue;
        }
        // reset count on valid packet
        readcount = 0;

        //Console.WriteLine(DateTime.Now.Millisecond + " SR2 " + BaseStream.BytesToRead);

        // check for a header
        if ((unsigned char)buffer[0] == 254 || buffer[0] == 'U')
        {
            // if we have the header, and no other chars, get the length and packet identifiers
            if (count == 0)
            {
                if (serial.waitForReadyRead(-1))
                {
                    buffer.append(serial.read(5));
                    count = 5;//?
                }
            }

            // packet length
            length = buffer[1] + 6 + 2 - 2; // data + header + checksum - U - length
            if (count >= 5)
            {
                if (serial.waitForReadyRead(-1))
                {
                    buffer.append(serial.read(length - 4));
                }
                count = length + 2;
            }
            break;
        }
        count++;
        if (count == 299)
            break;
    }
    // packet is now verified

    char sysid = buffer[3];
    char compid = buffer[4];

    if ((buffer[0] == 'U' || (unsigned char)buffer[0] == 254) && buffer.length() >= buffer[1])
    {
        // check if we lost pacakets based on seqno

        // set seens sysid's based on hb packet - this will hide 3dr radio packets
        if ((unsigned char)buffer[5] == 0)
        {
            //its hb
        }

        if ((unsigned char)buffer[5] == 253) // status text
        {
            //its status text
        }

    }

    return buffer;
}