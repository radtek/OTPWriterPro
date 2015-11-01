#pragma once

#include <string>
#include "hgz\hgz.h"
using std::string;
using std::wstring;

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
    //void md5(TCHAR *s, TCHAR *sDigest);
    void md5(char *s, char *sDigest);

    void to_string(TCHAR *str)
    {
        for(int i = 0; i < 16; i++)
        {
            _stprintf(str+2*i, _T("%02x"), ((UINT8 *)&digest)[i]);
        }
        str[32] = _T('\0');
    }

    tstring to_string(tstring &s)
    {
        TCHAR x[33];
        to_string(x);
        s = x;
        return s;
    }

	tstring to_string()
	{
		TCHAR x[33];
		to_string(x);
		tstring s = x;
		return s;
	}

	CString to_CString(CString &s)
	{
		TCHAR x[33];
		to_string(x);
		s = x;
		return s;
	}
	CString to_CString()
	{
		TCHAR x[33];
		to_string(x);
		CString s = x;
		return s;
	}

    void GetValue(UINT32 *val)
    {
        val[0] = digest[0];
        val[1] = digest[1];
        val[2] = digest[2];
        val[3] = digest[3];
    }

private:
    UINT32 digest[4], a, b, c, d;
    UINT32 _x[16];
    //UINT64 originalBitLen;

    void md5init(void);
    void md5core( const UINT8 *x );
    void md5end(UINT8 *begin, UINT32 len, UINT32 originalByteLen);
    
};

