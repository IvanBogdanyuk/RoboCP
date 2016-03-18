#include <robo_mavlink_test\mavlink.h>
extern char crcs[];
bool com_checksum(uint8_t *data, int32_t length); //Gets an array, length of its payload 