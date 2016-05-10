/*************************************************************************
Project: HS6200 - 2.4GHz wireless communication system
Date: 2014.07.12
Author: minzutao

Model name: ble_addr_gen
Function  : According to the BLE address generation rules to generate
the required ble ble address

******************************************************************/
#include "stdafx.h"
#include "ble_addr_gen.h"

/*
Fsync_mode
1 - syncbit number=32, without Preamble
2 - syncbit number=32, with 8-bit Preamble
3 - syncbit number=24, without Preamble
*/

#define     FSYNC_MODE    1

#if defined(BLE_ADDR_32BIT) 
const uint32_t GenPoly = 0xE0000200U;
#endif

#if defined(BLE_ADDR_24BIT) 
code const uint32_t GenPoly = 0x800043U;
#endif

static  uint8_t bitcount(uint32_t n)
{
	uint8_t count = 0;
	while (n) {
		count++;
		n &= (n - 1);
	}
	return count;
}

/*
check the addr , must be  meet ble address generate protocol
*/
static bool addr_check(uint32_t va_addr) {
	uint32_t trans_num = 0, xor_val;
	uint32_t addr = va_addr;
	uint8_t zeroBits, oneBits, i;

	/* 1. It shall have no more than six consecutive zeros or ones. */
	for (i = 0; i<(ADDR_LENGTH - 5); i++) {
		if (!(((addr >> i) & 0x3f) ^ 0x3f) || !((addr >> i) & 0x3f))
			return false;
	}

	/* 2. It shall have no more than 24 transitions*/
	trans_num = addr >> 1;
	trans_num ^= addr;
	if (bitcount(trans_num << 1) >(ADDR_LENGTH * 3 / 4))
		return false;

#if 0
	/* 3. It shall not have all four octets equal*/
#if defined(BLE_ADDR_32BIT)
	if ((addr >> 16) & 0xffff == (addr & 0xffff)) {
		if (((addr >> 8) & 0xff) == (addr & 0xff))
			return false;
	}
#endif
#if defined(BLE_ADDR_24BIT)
	if ((addr >> 8) & 0xff == (addr & 0xff)) {
		if (((addr >> 16) & 0xff) == ((addr >> 8) & 0xff))
			return false;
	}
#endif
#endif

#if (defined(VERSION) && (VERSION == VERSION_2_0))
	/* 4. DC should  be less than 10 transitions.floor(32/3)=10, floor(24/3)=8 */
	oneBits = bitcount(va_addr);

#if defined(BLE_ADDR_32BIT)
	zeroBits = ADDR_LENGTH - oneBits;
#elif defined(BLE_ADDR_24BIT)
	zeroBits = ADDR_LENGTH - oneBits;
#endif

	if (oneBits >= zeroBits) {
		if ((oneBits - zeroBits) >= ADDR_DC_MAX)
			return false;
	}
	else {
		if ((zeroBits - oneBits) >= ADDR_DC_MAX)
			return false;
	}
	if (va_addr == 0x88879d) {
		uint32_t pp = va_addr;
	}

	/* 5. xcorr-rule check , only calculate 8 ~ 15 */
	for (i = 8; i <= 15; i++) {
		xor_val = (va_addr ^ (va_addr >> i)) & (((uint32_t)1 << (ADDR_LENGTH - i)) - 1);
		oneBits = bitcount(xor_val);
		zeroBits = ADDR_LENGTH - i - oneBits;

		if (zeroBits > oneBits) {
			if ((zeroBits - oneBits) >= ADDR_XCORR_MAX)
				return false;
		}
	}
#endif

	return true;
}


uint32_t gen_ble_addr(uint32_t para) {
	uint32_t gen_addr = para;
	uint32_t msb_addr = 0;
	uint8_t i, j = 0;
	while (1) {
		j++;
		/* Generate the random data in 1 ~ (2^32-1), or 1 ~ (2^24-1) */
		for (i = 0; i<32; i++)
			gen_addr = (gen_addr >> 1) ^ ((-(gen_addr & 1u)) & GenPoly);
		if (addr_check(gen_addr) == true)
			break;
	}
#if 0
	/* change msb->lsb */
	for (i = 0; i<ADDR_LENGTH; i++) {
		msb_addr += ((gen_addr >> (ADDR_LENGTH - 1 - i)) & 0x1) << i;
	}
	return msb_addr;
#endif

	return gen_addr;
}



