#pragma once
/*************************************************************************
Project: HS6200 - 2.4GHz wireless communication system
Date: 2014.07.12
Author: minzutao

Model name: ble_addr_gen
Function  : According to the BLE address generation rules to generate
the required ble ble address

******************************************************************/

#ifndef _BLE_ADDR_GEN_H_
#define _BLE_ADDR_GEN_H_

/* addr generata version */
#define  VERSION_1_0     0x100
#define  VERSION_2_0	 0x200

#define  VERSION         VERSION_2_0

#ifndef  VERSION
#define  VERSION         VERSION_1_0
#endif

/* define cpu type  arm or 8051 */
//#define CPU_TYPE_ARM   1
//#define  CPU_TYPE_8051  1

#define  BLE_ADDR_32BIT 1
//#define  BLE_ADDR_24BIT 1

#include "afxwin.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef CPU_TYPE_ARM
#include <stdint.h>
#endif

//#ifdef CPU_TYPE_8051
typedef  unsigned int  uint32_t;
typedef  unsigned char uint8_t;
typedef  signed char sint8_t;
//#endif

#ifndef  bool
#define  bool    uint8_t
#endif

#ifndef true
#define true     1
#endif

#ifndef false
#define false    0
#endif

uint32_t gen_ble_addr(uint32_t para);

#if defined(BLE_ADDR_32BIT)
#define ADDR_LENGTH    32
#define ADDR_DC_MAX    10
#define ADDR_XCORR_MAX 12
#endif

#if defined(BLE_ADDR_24BIT)
#define ADDR_LENGTH    24
#define ADDR_DC_MAX    8
#define ADDR_XCORR_MAX 9
#endif

#endif
