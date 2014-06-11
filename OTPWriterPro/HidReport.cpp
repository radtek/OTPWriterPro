#include "StdAfx.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include ".\hgz\hgz.h"
#include "HidReport.h"


using namespace std;

void CHidReport::print(BOOL bDirectionDown, BOOL printOn /*= TRUE*/)
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
		
    //tcout << cstr.GetString(); // 速度慢
    _tprintf(_T("%s"), cstr); // 速度快

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


// overlay << operator, friend function, not member function
//ostream& operator << (ostream& os, const CHidReport& packet)
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




BOOL CHidReport::BaseReport( INT dataLen, UINT8 *pData )
{
    m_pkt.basePkt.len = 0;
    m_pkt.basePkt.csb = 0;
    
    if (dataLen > sizeof(HS_BASE_PACKET_t)-1 || dataLen == 0 || pData == NULL)
        return FALSE;

    memcpy_s(m_pkt.basePkt.data, sizeof(HS_BASE_PACKET_t)-2, pData, dataLen);

    m_pkt.basePkt.len = 2 + dataLen;

    return TRUE;
}

BOOL CHidReport::BaseReport_CxDx( INT cmdLen, UINT8 *pCmd, INT dataLen, UINT8 *pData )
{
    m_pkt.basePkt.len = 0;
    m_pkt.basePkt.csb = 0;

    if (pCmd == NULL)
        return FALSE;

    if ((cmdLen+dataLen) > sizeof(HS_BASE_PACKET_t)-2 || (cmdLen+dataLen) == 0)
        return FALSE;

    memcpy_s(m_pkt.basePkt.data, sizeof(HS_BASE_PACKET_t)-2, pCmd, cmdLen);
    memcpy_s(m_pkt.basePkt.data+cmdLen, sizeof(HS_BASE_PACKET_t)-2-cmdLen, pData, dataLen);

    m_pkt.basePkt.len = 2 + cmdLen + dataLen;

    return TRUE;
}


BOOL CHidReport::BaseReport_C1Dx( UINT8 cmdL1, INT dataLen, UINT8 *pData )
{
    return BaseReport_CxDx(1, &cmdL1, dataLen, pData);
}

BOOL CHidReport::BaseReport_C2Dx( UINT8 cmdL1, UINT8 cmdL2, INT dataLen, UINT8 *pData )
{
    UINT8 cmd[2];
    cmd[0] = cmdL1;
    cmd[1] = cmdL2;

    return BaseReport_CxDx(sizeof(cmd), cmd, dataLen, pData);
}

BOOL CHidReport::BaseReport_C3Dx( UINT8 cmdL1, UINT8 cmdL2, UINT8 cmdL3, INT dataLen, UINT8 *pData )
{
    UINT8 cmd[3];
    cmd[0] = cmdL1;
    cmd[1] = cmdL2;
    cmd[2] = cmdL3;

    return BaseReport_CxDx(sizeof(cmd), cmd, dataLen, pData);
}


BOOL CHidReport::BaseReport_C1D0( UINT8 cmdL1 )
{
    return BaseReport(1, &cmdL1);
}

BOOL CHidReport::BaseReport_C2D0( UINT8 cmdL1, UINT8 cmdL2 )
{
    return BaseReport_C2Dx(cmdL1, cmdL2, 0, NULL);
}

BOOL CHidReport::BaseReport_C3D0( UINT8 cmdL1, UINT8 cmdL2, UINT8 cmdL3 )
{
    return BaseReport_C3Dx(cmdL1, cmdL2, cmdL3, 0, NULL);
}

BOOL CHidReport::BaseReport_C1D1( UINT8 cmdL1, UINT8 data )
{
    return BaseReport_C1Dx(cmdL1, 1, &data);
}

BOOL CHidReport::BaseReport_C2D1( UINT8 cmdL1, UINT8 cmdL2, UINT8 data )
{
    return BaseReport_C2Dx(cmdL1, cmdL2, 1, &data);
}

BOOL CHidReport::BaseReport_C3D1( UINT8 cmdL1, UINT8 cmdL2, UINT8 cmdL3, UINT8 data )
{
    return BaseReport_C3Dx(cmdL1, cmdL2, cmdL3, 1, &data);
}

BOOL CHidReport::MemReadReport( UINT32 addr, UINT32 dataLen )
{
    UINT8 cmd[2+4*2];
    *(cmd + 0) = HS__MEM;
    *(cmd + 1) = HS__MEM__READ;
    memcpy_s(cmd+2, 4, &addr, 4);
    memcpy_s(cmd+2+4, 4, &dataLen, 4);
    hgzRevertByteOrder(cmd+2, 4);
    hgzRevertByteOrder(cmd+2+4, 4);

    return BaseReport_CxDx(sizeof(cmd), cmd, 0, NULL);
}

INT32 CHidReport::MemWriteReport( UINT32 addr, UINT32 dataLen, UINT8 *pBuf )
{
    UINT8 cmd[2+4*2];
    *(cmd + 0) = HS__MEM;
    *(cmd + 1) = HS__MEM__WRITE;
    memcpy_s(cmd+2, 4, &addr, 4);
    memcpy_s(cmd+2+4, 4, &dataLen, 4);
    hgzRevertByteOrder(cmd+2, 4);
    hgzRevertByteOrder(cmd+2+4, 4);

    return BaseReport_CxDx(sizeof(cmd), cmd, dataLen, pBuf) ? dataLen : 0;
}

INT CHidReport::SendReport( hid_device *handle, BOOL bPrint/* = TRUE*/ )
{
    print(TRUE, bPrint);
    // Set the hid_write() function to be blocking.
    hid_set_nonblocking(handle, 1);
    // send HS__MEM__WRITE packet.
    int res = hid_write(handle, &m_id, 1+sizeof(m_pkt));

    CString s;
    switch (m_pkt.memPkt.cmdL2)
    {
    case HS__MEM__WRITE:
        s = _T("HS__MEM__WRITE");
        break;

    case HS__MEM__READ:
        s = _T("HS__MEM__READ");
        break;

    default:
        break;
    }

    if (res < 0) {
        _tprintf(_T("Unable to send command: %s\n"), s);
        _tprintf(_T("Error: %ls\n"), hid_error(handle));
        return -1;
    }

    return 1;
}

INT CHidReport::ReceiveReport( hid_device *handle, BOOL bPrint/* = TRUE*/ )
{
    UINT8 id = m_id;
    HS_COMMON_PACKET_t p = m_pkt;

	// Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(handle, 0);
	// detect if device is ready for receiving another packet of data
	// Read requested state. hid_read() has been set to be
	// non-blocking by the call to hid_set_nonblocking() above.
	// This loop demonstrates the non-blocking nature of hid_read().
	int res = 0;
	_tprintf(_T("Receiving Report ...\n"));
	/*while (res == 0) {
		res = hid_read(handle, (UINT8 *)&m_pkt, sizeof(m_pkt));
// 		if (res == 0)
// 			_tprintf(_T("."));
		if (res < 0)
			_tprintf(_T("Unable to read()\n"));
		//Sleep(500);
	}*/

    res = hid_read_timeout(handle, (UINT8 *)&m_pkt, sizeof(m_pkt), 5000);
    if (res == 0)
        _tprintf(_T("Timeout!\n"));

    _tprintf(_T("Received data: "));
    print(FALSE, bPrint);
	
	if (p.memPkt.cmdL1 == m_pkt.memPkt.cmdL1 && 
		p.memPkt.cmdL2 == m_pkt.memPkt.cmdL2)
	{
		res = 1;
	}
	else {
		tcout << endl << _T("Read error!") << endl;
		res = -1;
	}	
	
	return res;
}

INT CHidReport::SendAndWaitToReceiveReport( hid_device * handle, BOOL bPrint/* = TRUE*/ )
{
    int res = 0;
    res = SendReport(handle, bPrint);
    if (res < 0) 
        return res;

    res = ReceiveReport(handle, bPrint);
    if (res < 0) 
        return res;

    return res;
}

BOOL CHidReport::MemCmdReport( UINT8 cmd )
{
    return BaseReport_C2D0(HS__MEM, cmd);
}

BOOL CHidReport::IsDenied()
{
    return (m_pkt.basePkt.csb & 1)==1; 
}

BOOL CHidReport::IsBlank()
{
    return (m_pkt.memPkt.data[0] != 0);
}

BOOL CHidReport::MemCmdReport_Cmd1Data2( UINT8 cmd, UINT32 data1, UINT32 data2 )
{
    UINT8 data[8];
    memcpy_s(data, 4, &data1, 4);
    memcpy_s(data+4, 4, &data2, 4);
    hgzRevertByteOrder(data, 4);
    hgzRevertByteOrder(data+4, 4);

    return BaseReport_C2Dx(HS__MEM, cmd, 8, data);
}

UINT32 CHidReport::GetResult()
{
    //throw std::exception("The method or operation is not implemented.");

    switch (m_pkt.basePkt.data[0])
    {
    case HS__MEM:
        {
            switch (m_pkt.memPkt.cmdL2)
            {
            case HS__MEM__ENTER:
            case HS__MEM__EXIT:

            case HS__MEM__WREN:
            case HS__MEM__WRDIS:
            case HS__MEM__RDSR:
            case HS__MEM__WRSR:
            case HS__MEM__ACCESS_DISABLE:
            case HS__MEM__READ_DISABLE:
            case HS__MEM__WRITE_DISABLE:
            case HS__MEM__BLKCHK:

            case HS__MEM__RESET:

            case HS__MEM__TEST_BLKCHK:
            case HS__MEM__TEST_DEC:
            case HS__MEM__TEST_WR:
            case HS__MEM__SPRON:
            case HS__MEM__SPROFF:

            case HS__MEM__RDFPCR:

                return m_pkt.cmdPkt.data[0];

            default:
                break;
            }
        }
        break;

    case HS__CMD:
        {
            switch (m_pkt.valPkt.cmdL2)
            {
            case HS__CMD__SET__CHIP_TYPE:
            case HS__CMD__GET__CHIP_TYPE:
                return m_pkt.valPkt.val;

            }
        }
        break;
    default:
        break;
    }

    return 0;
}

UINT32 CHidReport::GetStartSector()
{
    return hgzRevertByteOrder32(m_pkt.memPkt.addr);
}

UINT32 CHidReport::GetEndSector()
{
    return hgzRevertByteOrder32(m_pkt.memPkt.dataLen);
}


UINT8 * CHidReport::GetData()
{
    return &m_id;
}

UINT CHidReport::GetPacketLength()
{
    return m_pkt.basePkt.len;
}

UINT CHidReport::GetReportLength()
{
    return GetPacketLength()+1;
}

BOOL CHidReport::CmdReport( UINT8 cmdL1, UINT8 dataLen, UINT8 *pData )
{
    return BaseReport_C2Dx(HS__CMD, cmdL1, dataLen, pData);
}






