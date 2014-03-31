#include "StdAfx.h"
#include "Packet.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include ".\hgz\hgz.h"


using namespace std;
//int CPacket::usb_send(void)
//{
//	DWORD dwBytesWriteRequest = 0;
//	if (m_pkt.common.pcl) dwBytesWriteRequest = 3 + m_pkt.common.pcl; // (sizeof(pkt_comm_t)-4);
//	return usb_write(&m_pkt.common, dwBytesWriteRequest);
//}
//
//int CPacket::usb_receive(void)
//{
//	return m_packet_length = usb_read(&m_pkt.ack);
//}

void CPacket::print(BOOL bDirectionDown, BOOL printOn /*= TRUE*/)
{
	if (!printOn) return;

    CString packet_info = bDirectionDown ? _T("H2D: ") : _T("D2H: "); //_T("↓ H2D: ") : _T("↑ D2H: ");

	switch (m_pkt.memPkt.cmdL1)
	{
	case HS__MEM: 
		packet_info.AppendFormat(_T("HS__MEM"));
		break;
	case HS__CMD: 
		packet_info.AppendFormat(_T("HS__CMD"));
		break;
	case HS__ERR: 
		packet_info.AppendFormat(_T("HS__ERR"));
		break;
	case HS__DBG: 
		packet_info.AppendFormat(_T("HS__DBG"));
		break;
	}

	CString cstr; 
	cstr.Format(_T(""));
	//cstr.Format(_T("\r\n"));
	//cstr.AppendFormat(_T("================================================\r\n"));
	cstr.AppendFormat(_T("%s"), packet_info);
	cstr.AppendFormat(_T("\r\n"));
	//cstr.AppendFormat(_T(":  "));

	cstr.AppendFormat(_T("%02X | "), m_pkt.memPkt.len);
    cstr.AppendFormat(_T("%02X | "), m_pkt.memPkt.csb);
    cstr.AppendFormat(_T("%02X | "), m_pkt.memPkt.cmdL1);
	cstr.AppendFormat(_T("%02X | "), m_pkt.memPkt.cmdL2);
	cstr.AppendFormat(_T("%08X | "), hgzRevertByteOrder32(m_pkt.memPkt.addr));
	cstr.AppendFormat(_T("%08X | "), hgzRevertByteOrder32(m_pkt.memPkt.dataLen));
	for (int i = 0; i < m_pkt.memPkt.len-12; i++) 
	{
		cstr.AppendFormat(_T("%02X"), m_pkt.memPkt.data[i]);
	}
	cstr.AppendFormat(_T("\r\n"));
		
	tcout << cstr.GetString();

	// logFile
    // save log file
    if (0)
    {
        CString pathName = _T("HSOTPWriterPro.log");
        CStdioFile mFile;
        CFileException mExcept;
        mFile.Open(pathName, CFile::modeReadWrite | CFile::typeText, &mExcept);

        mFile.SeekToEnd();
        mFile.WriteString(cstr);
        mFile.Close();

        /*ofstream of(PathName.GetString(), ios_base::app);
        if (!of)
        {
            cerr << "Cannot open the logfile for output!\n";
            exit (-1);
        }
        of << cstr;
        of.close();*/
    }

	//// 要输出汉字，需如下设置：
	//tcout.imbue(std::locale("chs")); // Language and Country/Region Strings
	//tcout << _T("哈哈");
	

}

void CPacket::setup( unsigned char pid)
{
	m_pkt.memPkt.cmdL1 = pid;

	switch (pid)
	{
	case HS__MEM:
		break;
	case HS__CMD:
		break;
	case HS__ERR:
		break;
	case HS__DBG:
		break;
	}
}


// overlay << operator, friend function, not member function
//ostream& operator << (ostream& os, const CPacket& packet)
//{
//	if (packet.m_packet_length)
//	{
//		if (packet.m_pkt.common.cmdL1 == PID_COMM)
//		{
//			const unsigned char *p = &packet.m_pkt.comm.cmdL1;
//			
//			// o -- Out
//			os << 'o' << " "; 
//			// Time
//			if (packet.m_log_on.txp.time)
//				os << setw(8) << setfill('0') << dec << clock() << " "; 
//			// RF DevNum
//			if (packet.m_log_on.txp.pd)
//				os << setw(2) << setfill('0') << hex << (unsigned int)*(&packet.m_pkt.comm.pd) << " "; 
//			// PipeNum
//			if (packet.m_log_on.txp.addr_or_pipe)
//				for (unsigned char i = 0; i < p[4]; i++) 
//				{
//					// 注意：char 和 unsigned char 类型数值的 << 输出，是 ascii 码输出
//					os << setw(2) << setfill('0') << hex << (unsigned int)*(p+5+ i) << " "; 
//				}
//			// Packet type: TXPL, TXPLNAK, ACKPL
//			if (packet.m_log_on.txp.wiack_noack)
//				os << setw(2) << setfill('0') << hex << (unsigned int)*(&packet.m_pkt.comm.cmdL2) << " "; 
//			// Payload
//			if (packet.m_log_on.txp.payload)
//				for (unsigned char i = 0; i < packet.m_packet_length - 5 - p[4]; i++) 
//				{
//					// 注意：char 和 unsigned char 类型数值的 << 输出，是 ascii 码输出
//					os << setw(2) << setfill('0') << hex << (unsigned int)*(p+5+p[4] + i) << " ";
//				}
//			os << endl;
//		}
//		
//		else if ((packet.m_pkt.common.cmdL1 == PID_ACK) && (packet.m_pkt.ack.pt == TYPE_PLD))
//		{
//			// i -- In
//			os << 'i' << " "; 
//			// Time
//			if (packet.m_log_on.rxp.time) 
//				os << setw(8) << setfill('0') << dec << clock() << " "; 
//			// RF DevNum
//			if (packet.m_log_on.rxp.pd) 
//				os << setw(2) << setfill('0') << hex << (unsigned int)*(&packet.m_pkt.ack.pd) << " "; 
//			// PipeNum
//			if (packet.m_log_on.rxp.pipe) 
//				os << setw(2) << setfill('0') << hex << (unsigned int)*(&packet.m_pkt.ack.dat[0]) << " "; 
//			// Payload
//			if (packet.m_log_on.rxp.payload) 
//				for (unsigned char i = 0; i < packet.m_packet_length - 5; i++)
//				{
//					// 注意：char 和 unsigned char 类型数值的 << 输出，是 ascii 码输出
//					os << setw(2) << setfill('0') << hex << (unsigned int)*(&packet.m_pkt.ack.dat[1] + i) << " "; 
//				}
//			os << endl;
//		}
//
//		else
//		{
//			for (unsigned char i = 0; i < packet.m_packet_length; i++)
//			{
//				// 注意：char 和 unsigned char 类型数值的 << 输出，是 ascii 码输出
//				os << setw(2) << setfill('0') << hex << (unsigned int)*(&packet.m_pkt.common.cmdL1 + i) << " ";
//			}
//			os << endl;
//		}
//	}
//
//	return os;
//}


