#pragma once


#include "HSOTPWriterProtocol.h"
#include <iostream>
#include "hidapi.h"


using namespace std;




class CHidReport;
typedef CArray <CHidReport, CHidReport&> CHidReporttArray;

class CHidReport
{
public:
	CHidReport(INT reportID = 0) 
	{ 
		//memset(&m_pkt, 0, sizeof(m_pkt)); 
        m_id = reportID;
		m_pkt.memPkt.len = 0;
	};
	~CHidReport(void) {};

public:
#pragma pack(1) // change local byte alignment
    typedef struct {
        unsigned char len;
        unsigned char csb;
        unsigned char data[HS_OTPWRITER_HID_REPORT_SIZE-(1+1)];
    } HS_BASE_PACKET_t;

	typedef struct {
		unsigned char len;  // length of packet
        unsigned char csb; // Control Status Byte
		unsigned char cmdL1; // Command Level 1
		unsigned char cmdL2; // Command Level 2
		unsigned int addr;
		unsigned int dataLen;
		unsigned char data[HS_OTPWRITER_HID_REPORT_SIZE-(1+1+1+1+4+4)];
	} HS_RW_MEM_PACKET_t;

	typedef struct {
		unsigned char len;
        unsigned char csb;
		unsigned char cmdL1;
		unsigned char cmdL2;
        unsigned char data[HS_OTPWRITER_HID_REPORT_SIZE-(1+1+1+1)];
	} HS_CMD_PACKET_t;

	typedef struct {
        unsigned char len;
        unsigned char csb;
		unsigned char cmdL1;
		unsigned char cmdL2;
		unsigned char val;
	} HS_RW_VALUE_PACKET_t;

	typedef union {
        HS_BASE_PACKET_t basePkt;
		HS_RW_MEM_PACKET_t memPkt;
		HS_CMD_PACKET_t cmdPkt;
		HS_RW_VALUE_PACKET_t valPkt;
	} HS_COMMON_PACKET_t;

    unsigned char m_id;
    HS_COMMON_PACKET_t m_pkt;
#pragma pack()


    //friend ostream& operator << (ostream& os, const CHidReport& report);

public:
	const CHidReport& operator=(const CHidReport& report)
    {
        if(this != &report)
        {
			m_id = report.m_id;
            m_pkt = report.m_pkt;
        }
 
        return *this;
    }
    
public:
    void ReportID(INT reportID) {m_id = reportID;}
    INT ReportID(void) {return m_id;}

    void print(BOOL bDirectionDown, BOOL printOn = TRUE);

    INT SendReport(hid_device *handle, BOOL bPrint = TRUE);
    INT ReceiveReport(hid_device *handle, BOOL bPrint = TRUE);
    INT SendAndWaitToReceiveReport( hid_device * handle, BOOL bPrint  = TRUE );

    BOOL IsDenied();
    BOOL IsBlank();
    UINT32 GetResult();
    UINT32 GetStartSector();
    UINT32 GetEndSector();
    UINT8 *GetData();
    UINT GetPacketLength();
    UINT GetReportLength();

    // BaseReport
    BOOL BaseReport( INT dataLen, UINT8 *pData );
    BOOL BaseReport_CxDx(INT cmdLen, UINT8 *pCmd, INT dataLen, UINT8 *pData);

    BOOL BaseReport_C1Dx(UINT8 cmdL1, INT dataLen, UINT8 *pData);
    BOOL BaseReport_C2Dx(UINT8 cmdL1, UINT8 cmdL2, INT dataLen, UINT8 *pData);
    BOOL BaseReport_C3Dx(UINT8 cmdL1, UINT8 cmdL2, UINT8 cmdL3, INT dataLen, UINT8 *pData);

    BOOL BaseReport_C1D0(UINT8 cmdL1);
    BOOL BaseReport_C2D0(UINT8 cmdL1, UINT8 cmdL2);
    BOOL BaseReport_C3D0(UINT8 cmdL1, UINT8 cmdL2, UINT8 cmdL3);

    BOOL BaseReport_C1D1(UINT8 cmdL1, UINT8 data);
    BOOL BaseReport_C2D1(UINT8 cmdL1, UINT8 cmdL2, UINT8 data);
    BOOL BaseReport_C3D1(UINT8 cmdL1, UINT8 cmdL2, UINT8 cmdL3, UINT8 data);

    // MEM report
    BOOL MemReport(UINT8 cmdL1, UINT8); // NA
    BOOL MemReadReport(UINT32 addr, UINT32 dataLen);
    INT32 MemWriteReport(UINT32 addr, UINT32 dataLen, UINT8 *pBuf);
    BOOL MemCmdReport(UINT8 cmd);
    BOOL MemCmdReport_Cmd1Data2(UINT8 cmd, UINT32 data1, UINT32 data2);
    // CMD report
    BOOL CmdReport(UINT8 cmdL1, UINT8 dataLen, UINT8 *pData);

    // ERR report

    // DBG report



};
