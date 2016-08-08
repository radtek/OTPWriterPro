#ifndef __CRC_H__
#define __CRC_H__

#ifdef __cplusplus
extern "C" {
#endif
	// crc8(x) = x8 + x5 + x4 + 1
	extern unsigned char crc8(unsigned char *p, unsigned char len);

#ifdef __cplusplus
}
#endif

#endif // #ifndef __CRC_H__

