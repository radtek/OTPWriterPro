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
		m_pkt.MemPkt.pl = 0;

	};
	~CPacket(void) {};

public:
#pragma pack(1) // change local byte alignment
	/*typedef struct {
		unsigned char pl;
		unsigned char pid;
		unsigned char cmd;
		union {
			unsigned int whole;
			unsigned char bytearr[4];
		} address;
		union {
			unsigned int whole;
			unsigned char bytearr[4];
		} data_length;
		//unsigned char address[4];
		//unsigned char data_length[4];
		unsigned char data[HS_OTPWRITER_HID_REPORT_SIZE-(1+1+1+4+4)];
	} HS_RWMEMPACKET_t;*/

	typedef struct {
		unsigned char pl;
		unsigned char pid;
		unsigned char cmd;
		/*union {
			unsigned int whole;
			unsigned char bytearr[4];
		} address;
		union {
			unsigned int whole;
			unsigned char bytearr[4];
		} data_length;*/
		unsigned int address;
		unsigned int data_length;
		unsigned char data[HS_OTPWRITER_HID_REPORT_SIZE-(1+1+1+4+4)];
	} HS_RWMEMPACKET_t;

	typedef struct {
		unsigned char pl;
		unsigned char pid;
		unsigned char cmd;
	} HS_CMDPACKET_t;

	typedef struct {
		unsigned char pl;
		unsigned char pid;
		unsigned char cmd;
		unsigned char value;
	} HS_RWVALUEPACKET_t;

	typedef union {
		HS_RWMEMPACKET_t MemPkt;
		HS_CMDPACKET_t CmdPkt;
		HS_RWVALUEPACKET_t ValPkt;
	} HS_COMMONPACKET_t;

	HS_COMMONPACKET_t m_pkt;
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
