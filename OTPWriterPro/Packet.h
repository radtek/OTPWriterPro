#pragma once


#include "HSOTPWriterProtocol.h"
#include <iostream>

using namespace std;




class CPacket;
typedef CArray <CPacket, CPacket&> CPacketArray;

class CPacket
{
public:
	CPacket(void) 
	{ 
		//memset(&m_pkt, 0, sizeof(m_pkt)); 
		m_pkt.memPkt.len = 0;

	};
	~CPacket(void) {};

public:
#pragma pack(1) // change local byte alignment
	/*typedef struct {
		unsigned char len;
		unsigned char cmdL1;
		unsigned char cmdL2;
		union {
			unsigned int whole;
			unsigned char bytearr[4];
		} addr;
		union {
			unsigned int whole;
			unsigned char bytearr[4];
		} dataLen;
		//unsigned char addr[4];
		//unsigned char dataLen[4];
		unsigned char data[HS_OTPWRITER_HID_REPORT_SIZE-(1+1+1+4+4)];
	} HS_RW_MEM_PACKET_t;*/

	typedef struct {
		unsigned char len;  // ength of packet
        unsigned char csb; // Control Status Byte
		unsigned char cmdL1; // Command Level 1
		unsigned char cmdL2; // Command Level 2
		/*union {
			unsigned int whole;
			unsigned char bytearr[4];
		} addr;
		union {
			unsigned int whole;
			unsigned char bytearr[4];
		} dataLen;*/
		unsigned int addr;
		unsigned int dataLen;
		unsigned char data[HS_OTPWRITER_HID_REPORT_SIZE-(1+1+1+1+4+4)];
	} HS_RW_MEM_PACKET_t;

	typedef struct {
		unsigned char len;
        unsigned char csb;
		unsigned char cmdL1;
		unsigned char cmdL2;
	} HS_CMD_PACKET_t;

	typedef struct {
		unsigned char len;
        unsigned char csb;
		unsigned char cmdL1;
		unsigned char cmdL2;
		unsigned char val;
	} HS_RW_VALUE_PACKET_t;

	typedef union {
		HS_RW_MEM_PACKET_t memPkt;
		HS_CMD_PACKET_t cmdPkt;
		HS_RW_VALUE_PACKET_t valPkt;
	} HS_COMMON_PACKET_t;

	HS_COMMON_PACKET_t m_pkt;
#pragma pack()

public:
	void print(BOOL printOn = TRUE);
	//friend ostream& operator << (ostream& os, const CPacket& packet);

	void setup(unsigned char pid);


public:
	const CPacket& operator=(const CPacket& packet)
    {
        if(this != &packet)
        {
			m_pkt = packet.m_pkt;
        }
 
        return *this;
    }

};
