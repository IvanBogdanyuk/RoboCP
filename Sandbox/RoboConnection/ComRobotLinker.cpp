#include "ComRobotLinker.h"
#include "com_checksum.h"

/*
bool com_checksum(uint8_t *data, int32_t length, uint8_t com_ck_a, uint8_t com_ck_b) //Gets an array, length of its payload 
//and checksums on the com-ports
{
    if (length < 5)
        return 0;
    uint16_t res = crc_calculate(data, length + 6);
    crc_accumulate(crcs[data[5]], &res);
    uint8_t ck_a = (uint8_t)(res & 0xFF);          //< High byte
    uint8_t ck_b = (uint8_t)(res >> 8);              //< Low byte
    if ((ck_a == com_ck_a) && (ck_b == com_ck_b)) //If checksums are equal to each other, then
        return true;                              //returns true 
    else
        return false;
}
bool com_checksum(uint8_t *data, int32_t length) //Gets an array, length of its payload 
//and checksums on the com-ports
{
    if (length < 5)
        return 0;
    uint16_t res = crc_calculate(data, length + 6);
    crc_accumulate(crcs[data[5]], &res);
    uint8_t ck_a = (uint8_t)(res & 0xFF);          //< High byte
    uint8_t ck_b = (uint8_t)(res >> 8);              //< Low byte
    if ((ck_a == (uint8_t)data[length + 6]) && (ck_b == (uint8_t)data[length + 7])) //If checksums are equal to each other, then
        return true;                                                        //return true 
    else
        return false;
}
*/
ComRobotLinker::ComRobotLinker(){
    copterCom = new ComConnection();
}

void ComRobotLinker::sendPacket(MavlinkPacket* packet){


    //TODO: deciding on what length of the packet is

    copterCom->Write(packet->data, packet->len);
    QByteArray buffer = copterCom->readPacket();
    if (buffer.length() > 5){
        int idd = (unsigned char)buffer[5];
        if ((idd == 22) && (com_checksum(packet->data, 9)))
            std::cout << "checked:" << std::endl;

        std::cout << "got obj with id: " << idd << std::endl;
    }
}

void ComRobotLinker::openPort(QString name)
{
    copterCom->openPort(name);
    copterCom->getParamList();
}
