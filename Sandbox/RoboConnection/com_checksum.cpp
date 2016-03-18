#include <robo_mavlink_test\mavlink.h>
#include "com_checksum.h"

char crcs[] =  MAVLINK_MESSAGE_CRCS;

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
