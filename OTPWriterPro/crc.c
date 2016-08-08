#include "crc.h"


// crc8(x) = x8 + x5 + x4 + 1
unsigned char calcrc_1byte(unsigned char abyte)
{
	unsigned char i, crc_1byte;
	crc_1byte = 0; //设定crc_1byte初值为0
	for (i = 0; i < 8; i++) {
		if (((crc_1byte ^ abyte) & 0x01)) {
			crc_1byte ^= 0x18;
			crc_1byte >>= 1;
			crc_1byte |= 0x80;
		}
		else
			crc_1byte >>= 1;
		abyte >>= 1;
	}
	return crc_1byte;
}

unsigned char crc8(unsigned char *p, unsigned char len)
{
	unsigned char crc = 0;
	while(len--) //len为总共要校验的字节数
	{
		crc = calcrc_1byte(crc ^ *p++);
	}
	return crc;  //若最终返回的crc为0，则数据传输正确
}


