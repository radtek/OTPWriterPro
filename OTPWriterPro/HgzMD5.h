#pragma once
class CHgzMD5
{
public:
    CHgzMD5(void);
    ~CHgzMD5(void);

    // for buffer
    CString md5(UINT8 * buf, UINT32 byteLen, TCHAR *sDigest = NULL);
    
    // for file
    void md5(CFile &f, TCHAR *sDigest = NULL);
    CString md5(CStdioFile &f, CStatic *progress = NULL, TCHAR *sDigest = NULL);

    // for string
    void md5(CString &s, CString &sDigest);
    void md5(TCHAR *s, TCHAR *sDigest = NULL);
    void md5(char *s, char *sDigest = NULL);

private:
    UINT32 digest[4], a, b, c, d;
    UINT32 _x[16];
    //UINT64 originalBitLen;

    void md5init(void);
    void md5core( const UINT8 *x );
    void md5end(UINT8 *begin, UINT32 len, UINT32 originalByteLen);
    
};

