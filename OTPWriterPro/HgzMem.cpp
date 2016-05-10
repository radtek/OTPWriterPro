#include "StdAfx.h"
#include "HgzMem.h"
#include "OTPWriterPro.h"
#include "hgz\hgz.h"


CHgzMem::CHgzMem( INT nSize /*= 16*1024*/ )
{
    m_nId = 0;
    m_nSizeUsed = 0;
    m_buf.SetSize(nSize);
    m_bufFlag.SetSize((nSize));
    memset(m_buf.GetData(), 0, nSize);
    memset(m_bufFlag.GetData(), 0, nSize);
}

CHgzMem::~CHgzMem(void)
{
}

BOOL CHgzMem::IsEmpty()
{
    return (m_nSizeUsed == 0);
}

BOOL CHgzMem::IsUsed(UINT32 addr)
{
    return (m_bufFlag[addr] & 0x01);
}

INT32 CHgzMem::WriteBuf( UINT32 addr, UINT8 *pData, UINT32 dataLen )
{
    UINT32 nSize = addr + dataLen;
    if (nSize > m_buf.GetSize())
    {
        m_buf.SetSize(nSize + 1024);
        m_bufFlag.SetSize(nSize + 1024);
    }
    
    if (m_nSizeUsed < nSize)
    {
        m_nSizeUsed = nSize;
    }
    
    memcpy_s(m_buf.GetData()+addr, m_buf.GetSize()-addr, pData, dataLen);
    memset(m_bufFlag.GetData()+addr, 1, dataLen);

    return dataLen;
}

INT32 CHgzMem::ReadBuf( UINT32 addr, UINT8 *pData, UINT32 dataLen )
{
    if (m_nSizeUsed <= addr)
        return 0;

    UINT32 nSize = min(m_nSizeUsed-addr, dataLen);
    memcpy_s(pData, dataLen, m_buf.GetData()+addr, nSize);
    return nSize;
}

BOOL CHgzMem::ClearBuf( UINT32 addr, UINT32 len )
{
    if (-1 == len)
    {
        addr = 0;
        len = m_nSizeUsed;
    }

    UINT32 nSize = 0;

    if (m_nSizeUsed <= addr)
        return FALSE;
    else if (m_nSizeUsed <= addr+len)
    {
        nSize = m_nSizeUsed - addr;
        m_nSizeUsed = addr;
    }
    else
    {
        nSize = len;
    }

    memset(m_buf.GetData()+addr, 0, nSize);
    memset(m_bufFlag.GetData()+addr, 0, nSize);
    return nSize;
}

BOOL CHgzMem::ClearBufAll( void )
{
    return ClearBuf(0, -1);
}

BOOL CHgzMem::FillBuf( UINT32 addr, UINT8 val, UINT32 dataLen )
{
    UINT32 nSize = addr + dataLen;
    if (nSize > m_buf.GetSize())
    {
        m_buf.SetSize(nSize + 1024);
        m_bufFlag.SetSize(nSize + 1024);
    }

    if (m_nSizeUsed < nSize)
    {
        m_nSizeUsed = nSize;
    }

    memset(m_buf.GetData()+addr, val, dataLen);
    memset(m_bufFlag.GetData()+addr, 1, dataLen);

    return dataLen;
}

#define OTPWriter_BUFFER_SIZE ((1024*2))
INT32 CHgzMem::Write( UINT32 addr, UINT32 length )
{
    if (length == 0)
    {
        AfxMessageBox(_T("The lenth of data to write can't be 0!"));
        return 0;
    }

    if (m_nSizeUsed == 0) {
        AfxMessageBox(_T("No data to write."));
        return 0;
    }

    // Open the device using the VID, PID, and optionally the Serial number.
    hid_device *handle = hid_open(HS_VENDOR_ID, HS_PRODUCT_ID_OTPWRITER, NULL);
    if (!handle) {
        _tprintf(_T("unable to open device\n"));
        return 0;
    }

    UINT packet_data_length = 52;
    CHidReport r;
    INT num = 0;
    while (num != length) 
    {
        // build the MemWrite packet.
        INT32 num1 = r.MemWriteReport(addr+num, min(packet_data_length, length-num), m_buf.GetData()+addr+num);
        if (r.SendAndWaitToReceiveReport(handle) < 0) 
            break;
        num += num1;
        g_pctrlProgress->SetPos(num*100/length);
    }
    // Send the terminating data-empty packet.
    r.MemWriteReport(addr+num, 0, m_buf.GetData()+addr+num);
    if (r.SendAndWaitToReceiveReport(handle) < 0) //r.SendReport(handle);
    {
        if (num % OTPWriter_BUFFER_SIZE)
            num = (num/OTPWriter_BUFFER_SIZE)*OTPWriter_BUFFER_SIZE;
        else if (num == 0)
            num = 0;
        else
            num = num-OTPWriter_BUFFER_SIZE;
    }
    hid_close(handle); /* Free handle objects. */
    hid_exit(); /* Free static HIDAPI objects. */

    return num;
}

INT32 CHgzMem::Read( UINT32 addr, UINT32 length )
{
    // Open the device using the VID, PID, and optionally the Serial number.
    hid_device *handle = hid_open(HS_VENDOR_ID, HS_PRODUCT_ID_OTPWRITER, NULL);
    if (!handle) {
        _tprintf(_T("unable to open device\n"));
        return 0;
    }

    INT num = 0;
    CHidReport r;
    
    if (!r.MemReadReport(addr, length))
    {
        num = 0;
        goto Label_Return;
    }

    if (r.SendReport(handle) < 0)
    {
        num = 0;
        goto Label_Return;
    }

    //ClearBufAll();
    while (1) 
    {
        if (r.ReceiveReport(handle) < 0)
            break;

        unsigned int dataLen = hgzRevertByteOrder32(r.m_pkt.memPkt.dataLen);
        unsigned int address = hgzRevertByteOrder32(r.m_pkt.memPkt.addr);
        if (dataLen == 0) {
            g_pctrlProgress->SetPos(100);
            break;
        }
        else {
            num += dataLen;
            memcpy(m_buf.GetData()+address, r.m_pkt.memPkt.data, dataLen);
            memset(m_bufFlag.GetData()+address, 1, dataLen);
            if (m_nSizeUsed < (address + dataLen))
                m_nSizeUsed = address + dataLen;
        }
        g_pctrlProgress->SetPos(num*100/length);
    }

Label_Return:
    hid_close(handle); /* Free handle objects. */
    hid_exit(); /* Free static HIDAPI objects. */

    return num;
}

BOOL CHgzMem::Verify( UINT32 addr, UINT32 length )
{
    if (IsEmpty()) {
        AfxMessageBox(_T("There's no data in the buffer to verify."));
        return FALSE;
    }

    // Open the device using the VID, PID, and optionally the Serial number.
    hid_device *handle = hid_open(HS_VENDOR_ID, HS_PRODUCT_ID_OTPWRITER, NULL);
    if (!handle) {
        _tprintf(_T("unable to open device\n"));
        return FALSE;
    }

    BOOL res = TRUE; // return value;
    CHidReport r;

    INT reportLen = 52;
    for (UINT32 num = 0; num < length; num += reportLen)
    {
        if (!r.MemReadReport(addr+num, min(reportLen, length-num)))
        {
            res = FALSE;
            break;
        }

        if (r.SendReport(handle) < 0)
        {
            res = FALSE;
            break;
        }

        if (r.ReceiveReport(handle) < 0)
        {
            res = FALSE;
            break;
        }

        unsigned int dataLen = hgzRevertByteOrder32(r.m_pkt.memPkt.dataLen);
        unsigned int address = hgzRevertByteOrder32(r.m_pkt.memPkt.addr);
        if (dataLen == 0) 
        {
            res = TRUE;
            break;
        }
            
        CHidReport r1;
        if (!r1.MemReadReport(addr+num, reportLen))
        {
            res = FALSE;
            break;
        }

        if (r1.ReceiveReport(handle) < 0) // 数据长度为 0 的终止包，每个读一个。
        {
            res = FALSE;
            break;
        }

        // compare readout report data with the buf
        int err = -1;
        if ((err = CompareMemData(r, m_buf.GetData())) != -1)
        {
            res = FALSE;
            UINT errAddr = addr + num + err;
            CString s;
            s.Format(_T("Verification fails!\r\n (addr | mem_data | buffer_data): %08X | %02X | %02X\r\n"), errAddr, r.m_pkt.memPkt.data[err], errAddr < g_mem.SizeUsed() ? m_buf[errAddr] : 0x00);
            tcout << s.GetString();
            AfxMessageBox(s);
            break;
        }

        g_pctrlProgress->SetPos(num*100/length);

        Sleep(20);
    }

    hid_close(handle); /* Free handle objects. */
    hid_exit(); /* Free static HIDAPI objects. */

    if (res)
    {
        g_pctrlProgress->SetPos(100);
        _tprintf(_T("Verification succeeds!"));
        //AfxMessageBox(_T("Verification succeeds!"));
    }
    else
    {

    }
    return res;
}


BOOL CHgzMem::VerifyEx( UINT32 addr, UINT32 length, UINT32 addrIgnoreBegin, UINT32 addrIgnoreEnd )
{
    if (IsEmpty()) {
        AfxMessageBox(_T("There's no data in the buffer to verify."));
        return FALSE;
    }

    // Open the device using the VID, PID, and optionally the Serial number.
    hid_device *handle = hid_open(HS_VENDOR_ID, HS_PRODUCT_ID_OTPWRITER, NULL);
    if (!handle) {
        _tprintf(_T("unable to open device\n"));
        return FALSE;
    }

    BOOL res = TRUE; // return value;
    CHidReport r;

    INT reportLen = 52;
    for (UINT32 num = 0; num < length; num += reportLen)
    {
        if (!r.MemReadReport(addr+num, min(reportLen, length-num)))
        {
            res = FALSE;
            break;
        }

        if (r.SendReport(handle) < 0)
        {
            res = FALSE;
            break;
        }

        if (r.ReceiveReport(handle) < 0)
        {
            res = FALSE;
            break;
        }

        unsigned int dataLen = hgzRevertByteOrder32(r.m_pkt.memPkt.dataLen);
        unsigned int address = hgzRevertByteOrder32(r.m_pkt.memPkt.addr);
        if (dataLen == 0) 
        {
            res = TRUE;
            break;
        }

        CHidReport r1;
        if (!r1.MemReadReport(addr+num, reportLen))
        {
            res = FALSE;
            break;
        }

        if (r1.ReceiveReport(handle) < 0) // 数据长度为 0 的终止包，每个读一个。
        {
            res = FALSE;
            break;
        }

        // compare readout report data with the buf
        int err = -1;
        if ((err = CompareMemDataEx(r, m_buf.GetData(), addrIgnoreBegin, addrIgnoreEnd)) != -1)
        {
            res = FALSE;
            UINT errAddr = addr + num + err;
            CString s;
            s.Format(_T("Verification fails!\r\n (addr | mem_data | buffer_data): %08X | %02X | %02X\r\n"), errAddr, r.m_pkt.memPkt.data[err], errAddr < g_mem.SizeUsed() ? m_buf[errAddr] : 0x00);
            tcout << s.GetString();
            AfxMessageBox(s);
            break;
        }

        g_pctrlProgress->SetPos(num*100/length);
        g_pctrlProgress->UpdateWindow();

        Sleep(20);
    }

    hid_close(handle); /* Free handle objects. */
    hid_exit(); /* Free static HIDAPI objects. */

    if (res)
    {
        g_pctrlProgress->SetPos(100);
        _tprintf(_T("Verification succeeds!"));
        //AfxMessageBox(_T("Verification succeeds!"));
    }
    else
    {

    }
    return res;
}


BOOL CHgzMem::Encryt( UINT32 &res )
{
    return ExecuteMemCmd_Cmd1Data0(HS__MEM__ACCESS_DISABLE, &res);
}

BOOL CHgzMem::IsEncyption( void )
{
    return FALSE;
}

BOOL CHgzMem::Erease( UINT32 &startSectorNum, UINT32 &endSectorNum )
{
    CHidReport r;
    if (!r.MemCmdReport_Cmd1Data2(HS__MEM__ERASE_PAGE, startSectorNum, endSectorNum))
    {
        _tprintf(_T("Command fails.\n"));
        return FALSE;
    }
    BOOL bRes = ExecuteMemCmd(r);
    
    if (bRes)
    {
        startSectorNum = r.GetStartSector();
        endSectorNum = r.GetEndSector();
    }

    return bRes;
}

BOOL CHgzMem::EreaseAll( UINT32 &startSectorNum, UINT32 &endSectorNum )
{
    CHidReport r;
    if (!r.MemCmdReport(HS__MEM__ERASE_ALL))
    {
        _tprintf(_T("Command error.\n"));
        return FALSE;
    }
    BOOL bRes = ExecuteMemCmd(r);
    
    if (bRes)
    {
        startSectorNum = r.GetStartSector();
        endSectorNum = r.GetEndSector();
    }

    return bRes;
}

BOOL CHgzMem::IsBlank( UINT32 &res )
{
    return ExecuteMemCmd_Cmd1Data0(HS__MEM__BLKCHK, &res);
}

/////////////////////////
// Private functions
INT CHgzMem::CompareMemData( CHidReport &r, UINT8 *pBuf )
{
    UINT dataLen = hgzRevertByteOrder32(r.m_pkt.memPkt.dataLen);
    UINT address = hgzRevertByteOrder32(r.m_pkt.memPkt.addr);

    for (int i = 0; i < dataLen ; i++)
    {
        UINT addr = address + i;

        if (r.m_pkt.memPkt.data[i] != (addr < g_mem.SizeUsed() ? pBuf[addr] : 0)) {
            return i; // index of the first error byte in the report data field.
        }
    }

    return -1; // successful
}

INT CHgzMem::CompareMemDataEx( CHidReport &r, UINT8 *pBuf, UINT32 addrIgnoreBegin, UINT32 addrIgnoreEnd )
{
    UINT dataLen = hgzRevertByteOrder32(r.m_pkt.memPkt.dataLen);
    UINT address = hgzRevertByteOrder32(r.m_pkt.memPkt.addr);

    for (int i = 0; i < dataLen ; i++)
    {
        UINT addr = address + i;
        
        if (addr>=addrIgnoreBegin && addr<=addrIgnoreEnd)
            continue;

        if (r.m_pkt.memPkt.data[i] != (addr < g_mem.SizeUsed() ? pBuf[addr] : 0)) {
            return i; // index of the first error byte in the report data field.
        }
    }

    return -1; // successful
}

BOOL CHgzMem::ExecuteMemCmd( CHidReport &r, UINT32 *res /*= NULL */ )
{
    if (res) *res = 0;
    // Open the device using the VID, PID, and optionally the Serial number.
    hid_device *handle = hid_open(HS_VENDOR_ID, HS_PRODUCT_ID_OTPWRITER, NULL);
    if (!handle) {
        _tprintf(_T("Unable to open device\n"));
        return FALSE;
    }

    BOOL bRes = TRUE;

    if (r.SendReport(handle) < 0)
    {
        bRes = FALSE;
        goto Label_Return;
    }

    if (r.ReceiveReport(handle) <= 0)
    {
        bRes = FALSE;
        goto Label_Return;
    }

    bRes = TRUE;

Label_Return:
    hid_close(handle); /* Free handle objects. */
    hid_exit(); /* Free static HIDAPI objects. */

    if (bRes)
    {
        if (r.IsDenied())
        {
            bRes = FALSE;
            _tprintf(_T("Command is Denied!\n"));
            AfxMessageBox(_T("拒绝执行！"));
        }
        else
        {
            if (res) *res = r.GetResult();
            _tprintf(_T("Command succeeds.\n"));
        }
    }
    else
    {
        _tprintf(_T("Command fails.\n"));
        AfxMessageBox(_T("执行失败！"));
    }
    return bRes;
}


BOOL CHgzMem::ExecuteMemCmd_Cmd1Data0( UINT8 cmd, UINT32 *res /*= NULL*/ )
{
    CHidReport r;

    if (!r.MemCmdReport(cmd))
    {
        _tprintf(_T("Command error.\n"));
        return FALSE;
    }

    return ExecuteMemCmd(r, res);
}


BOOL CHgzMem::ExecuteCmd_Cmd1Datax( UINT8 cmd, UINT8 dataLen, UINT8 *pData, UINT32 *res /*= NULL*/ )
{
    CHidReport r;

    if (!r.CmdReport(cmd, dataLen, pData))
    {
        _tprintf(_T("Command error.\n"));
        return FALSE;
    }

    return ExecuteMemCmd(r, res);
}

/////////////////////////////////////////////////////
// public

BOOL CHgzMem::EnterOTPSPIMode( UINT32 &res )
{
    return ExecuteMemCmd_Cmd1Data0(HS__MEM__ENTER, &res);
}

BOOL CHgzMem::ExitOTPSPIMode( UINT32 &res )
{
    return ExecuteMemCmd_Cmd1Data0(HS__MEM__EXIT, &res);
}

BOOL CHgzMem::OTPTest_BlankCheckTest( UINT32 &res )
{
    return ExecuteMemCmd_Cmd1Data0(HS__MEM__TEST_BLKCHK, &res);
}

BOOL CHgzMem::OTPTest_WordLineAndBitLineIntegrityTest( UINT32 &res )
{
    return ExecuteMemCmd_Cmd1Data0(HS__MEM__TEST_DEC, &res);
}

BOOL CHgzMem::OTPTest_PreprogramTest( UINT32 &res )
{
    return ExecuteMemCmd_Cmd1Data0(HS__MEM__TEST_WR, &res);
}

BOOL CHgzMem::OTPSpareBitOn( UINT32 &res )
{
    return ExecuteMemCmd_Cmd1Data0(HS__MEM__SPRON, &res);
}

BOOL CHgzMem::OTPSpareBitOff( UINT32 &res )
{
    return ExecuteMemCmd_Cmd1Data0(HS__MEM__SPROFF, &res);
}

BOOL CHgzMem::OTPSPIReset( UINT32 &res )
{
    return ExecuteMemCmd_Cmd1Data0(HS__MEM__RESET, &res);
}

BOOL CHgzMem::DetectChipType( HS_CHIP_TYPE_t *ct )
{
    return ExecuteCmd_Cmd1Datax(HS__CMD__GET__CHIP_TYPE, 0, NULL, (u32*)ct);
}

BOOL CHgzMem::SetChipType( HS_CHIP_TYPE_t *ct )
{
    return ExecuteCmd_Cmd1Datax(HS__CMD__SET__CHIP_TYPE, 1, (UINT8 *)ct);
}

BOOL CHgzMem::GetChipID(UINT32 *chip_id)
{
	return ExecuteCmd_Cmd1Datax(HS__CMD__GET__CHIP_ID, 0, NULL, chip_id);
}

BOOL CHgzMem::GetFirmwareVersionInfo(UINT8 *info)
{
    CHidReport r;

    if (!r.CmdReport(HS__CMD__GET__FIRMWARE_VERSION, 0, NULL))
    {
        _tprintf(_T("Command error.\n"));
        return FALSE;
    }

    if (ExecuteMemCmd(r, NULL))
    {
        memcpy_s(info, 64, r.GetData()+5, r.GetPacketLength()-4);
        info[r.GetPacketLength()-4] = 0;
        return TRUE;
    }

    return FALSE;
}




