#include "StdAfx.h"
#include "Packet.h"
#include <iostream>
#include <iomanip>
#include ".\hgz\hgz.h"



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

void CPacket::print(BOOL printOn)
{
	if (!printOn) return;


	CString packet_info;
	int packet_direction = 1; // D2H


	switch (m_pkt.MemPkt.pid)
	{
	case HS_PID_MEM: 
		packet_direction = 0;
		packet_info.Format(_T("H2D: HS_PID_MEM"));
		break;
	case HS_PID_CMD: 
		packet_direction = 0;
		packet_info.Format(_T("H2D: HS_PID_CMD"));
		break;
	case HS_PID_ERR: 
		packet_direction = 0;
		packet_info.Format(_T("H2D: HS_PID_ERR"));
		break;
	case HS_PID_DBG: 
		packet_direction = 0;
		packet_info.Format(_T("H2D: HS_PID_DBG"));
		break;
	}


	if (packet_direction == 0)
	{
		CString cstr; 
		cstr.Format(_T(""));
		//cstr.Format(_T("\r\n"));
		//cstr.AppendFormat(_T("================================================\r\n"));
		cstr.AppendFormat(_T("%s"), packet_info);
		cstr.AppendFormat(_T("\r\n"));
		//cstr.AppendFormat(_T(":  "));

		cstr.AppendFormat(_T("%02X | "), m_pkt.MemPkt.pl);
		cstr.AppendFormat(_T("%02X | "), m_pkt.MemPkt.pid);
		cstr.AppendFormat(_T("%02X | "), m_pkt.MemPkt.cmd);
		cstr.AppendFormat(_T("%08X | "), m_pkt.MemPkt.address.whole);
		cstr.AppendFormat(_T("%08X | "), m_pkt.MemPkt.data_length.whole);
		for (int i = 0; i < m_pkt.MemPkt.pl-11; i++) 
		{
			cstr.AppendFormat(_T("%02X"), m_pkt.MemPkt.data[i]);
		}
		cstr.AppendFormat(_T("\r\n"));
		
		tcout << cstr.GetString();

		//// 要输出汉字，需如下设置：
		//tcout.imbue(std::locale("chs")); // Language and Country/Region Strings
		//tcout << _T("哈哈");
	}

}

void CPacket::setup( unsigned char pid)
{
	m_pkt.MemPkt.pid = pid;

	switch (pid)
	{
	case HS_PID_MEM:
		break;
	case HS_PID_CMD:
		break;
	case HS_PID_ERR:
		break;
	case HS_PID_DBG:
		break;
	}
}


// overlay << operator, friend function, not member function
//ostream& operator << (ostream& os, const CPacket& packet)
//{
//	if (packet.m_packet_length)
//	{
//		if (packet.m_pkt.common.pid == PID_COMM)
//		{
//			const unsigned char *p = &packet.m_pkt.comm.pid;
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
//				os << setw(2) << setfill('0') << hex << (unsigned int)*(&packet.m_pkt.comm.cmd) << " "; 
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
//		else if ((packet.m_pkt.common.pid == PID_ACK) && (packet.m_pkt.ack.pt == TYPE_PLD))
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
//				os << setw(2) << setfill('0') << hex << (unsigned int)*(&packet.m_pkt.common.pid + i) << " ";
//			}
//			os << endl;
//		}
//	}
//
//	return os;
//}


