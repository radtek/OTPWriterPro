#pragma once

#include "hidapi.h"
#include "HidReport.h"


class CHgzMem
{
public:
    CHgzMem(INT nSize = 16*1024);
    ~CHgzMem(void);

    INT Size() const { return m_buf.GetSize(); }
    void Size(INT val) { m_buf.SetSize(val); m_bufFlag.SetSize(val); }
    INT SizeUsed() const { return m_nSizeUsed; }
    void SizeUsed(INT val) { m_nSizeUsed = val; }
    INT Id() const { return m_nId; }
    void Id(INT val) { m_nId = val; }

    HS_CHIP_TYPE_t ChipType() const { return m_eChipType; }
    void ChipType(HS_CHIP_TYPE_t val) { m_eChipType = val; }

    UINT8 *GetBuf(void){ return m_buf.GetData(); }
    UINT8 *GetBufFlag(void){ return m_bufFlag.GetData(); }
    BOOL IsEmpty();
    BOOL IsUsed(UINT32 addr);
    
    
    INT32 WriteBuf(UINT32 addr, UINT8 *pData, UINT32 dataLen);
    INT32 ReadBuf(UINT32 addr, UINT8 *pData, UINT32 dataLen);
    BOOL ClearBuf(UINT32 addr, UINT32 len);
    BOOL ClearBufAll( void );
    BOOL FillBuf(UINT32 addr, UINT8 val, UINT32 dataLen);
    BOOL SetBufFlag(UINT32 addr, UINT8 val, UINT32 dataLen);

    BOOL DetectChipType( HS_CHIP_TYPE_t *ct );
    BOOL SetChipType(HS_CHIP_TYPE_t *ct);
    INT32 Write(UINT32 addr, UINT32 length);
    INT32 Read(UINT32 addr, UINT32 length);
    BOOL Verify(UINT32 addr, UINT32 length);
    BOOL VerifyEx(UINT32 addr, UINT32 length, UINT32 addrIgnoreBegin, UINT32 addrIgnoreEnd);
    BOOL Encryt( UINT32 &res );
    BOOL IsEncyption(void);
    BOOL Erease(UINT32 &startSectorNum, UINT32 &endSectorNum);
    BOOL EreaseAll( UINT32 &startSectorNum, UINT32 &endSectorNum );
    BOOL IsBlank(UINT32 &res);

    BOOL EnterOTPSPIMode(UINT32 &res);
    BOOL ExitOTPSPIMode(UINT32 &res);
    BOOL OTPTest_BlankCheckTest(UINT32 &res);
    BOOL OTPTest_WordLineAndBitLineIntegrityTest(UINT32 &res);
    BOOL OTPTest_PreprogramTest(UINT32 &res);
    BOOL OTPSpareBitOn(UINT32 &res);
    BOOL OTPSpareBitOff(UINT32 &res);
    BOOL OTPSPIReset(UINT32 &res);

    BOOL GetFirmwareVersionInfo(UINT8 *info);

    typedef BOOL (CHgzMem::*pMemFunc_Cmd1Data0)(UINT32 &);

private:
    INT CompareMemData( CHidReport &r, UINT8 *pBuf );
    INT CompareMemDataEx( CHidReport &r, UINT8 *pBuf, UINT32 addrIgnoreBegin, UINT32 addrIgnoreEnd );
    BOOL ExecuteMemCmd( CHidReport &r, UINT32 *res = NULL );
    BOOL ExecuteMemCmd_Cmd1Data0(UINT8 cmd, UINT32 *res = NULL);
    BOOL ExecuteCmd_Cmd1Datax(UINT8 cmd, UINT8 dataLen, UINT8 *pData, UINT32 *res = NULL);

private:
    CByteArray m_buf;
    CByteArray m_bufFlag;
    INT m_nId;
    INT m_nSizeUsed;
    HS_CHIP_TYPE_t m_eChipType;
    
};

