#include "StdAfx.h"
#include "HgzMD5.h"
#include ".\hgz\hgz.h"


CHgzMD5::CHgzMD5(void)
{
    md5init();
}


CHgzMD5::~CHgzMD5(void)
{
}


CString CHgzMD5::md5( UINT8 * buf, UINT32 byteLen, TCHAR *sDigest )
{
    UINT8 *begin = buf;
    UINT8 *end = begin + byteLen;
    
    md5init();

    for (; begin+(512/8) <= end; begin+=(512/8))
    {
        md5core(begin);
    }

    md5end(begin, end-begin, byteLen);

    return to_CString();
}

void CHgzMD5::md5( CFile &f, TCHAR *sDigest )
{

}

CString CHgzMD5::md5( CStdioFile &f, CStatic *progress, TCHAR *sDigest )
{
    UINT8 *begin = new UINT8[1024*2];
    UINT8 *begin1 = begin;
    UINT8 *end   = begin + 1024*2;
    UINT32 byteLen = f.GetLength();
    UINT32 byteRead = 0;
    int prog = 0;
    CString sProg;
    if (progress)
        progress->SetWindowText(_T("0%"));
    md5init();

    int i = 0;
    f.SeekToBegin();
    while(byteRead = f.Read(begin, 1024*2))
    {
        end = begin + byteRead;
        for (; begin+(512/8) <= end; begin+=(512/8))
        {
            md5core(begin);
        }

        if (f.GetPosition() == f.GetLength())
        {
            md5end(begin, end-begin, byteLen);
            break;
        }

        begin = begin1;
        
        if ((++i%10 == 0) && prog < f.GetPosition()*100/f.GetLength())
        {
            prog = f.GetPosition()*100/f.GetLength();
            sProg.Format(_T("%d%%"), prog);
            if (progress)
                progress->SetWindowText(sProg);
        }
    }

    CString s;
    for(int i = 0; i < 16; i++)
    {
        s.AppendFormat(_T("%02x"), ((UINT8 *)&digest)[i]);
        //_stprintf(sDigest[i], _T("%02x"), x[i]);
    }
    //AfxMessageBox(_T("md5(")+f.GetFileName()+_T(") = ")+s);

    if (progress)
        progress->SetWindowText(_T("100%"));
    
    delete [] begin1;

    return s;
}

void CHgzMD5::md5( CString &s, CString &sDigest )
{
    UINT8 *buf = (UINT8 *)s.GetBuffer();
    
    sDigest = md5(buf, s.GetLength()*sizeof(TCHAR));
}

//void CHgzMD5::md5( TCHAR *s, TCHAR *sDigest )
//{
//    md5((UINT8 *)s, _tcslen(s)*sizeof(TCHAR));
//}

void CHgzMD5::md5( char *s, char *sDigest )
{
    CString su = md5((UINT8 *)s, strlen(s));
    CStringA sa(su);
    //strcpy(sDigest, sa.GetBuffer());
    strcpy(sDigest, CStringA(su).GetBuffer());
}

void CHgzMD5::md5init( void )
{
    // 初始化链接变量
    digest[0] = 0x67452301;
    digest[1] = 0xefcdab89;
    digest[2] = 0x98badcfe;
    digest[3] = 0x10325476;

}

void CHgzMD5::md5core( const UINT8 *x )
{
    #define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
    #define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
    #define H(x, y, z) ((x) ^ (y) ^ (z))
    #define I(x, y, z) ((y) ^ ((x) | (~z)))

    #define RL(x, y) (((x) << (y)) | ((x) >> (32 - (y)))) //x 循环左移 y 位，注意是循环左移，不是左移。

    /*
       假设Mj表示消息的第j个子分组（从0到15），常数ti是4294967296*abs( sin(i) ）的整数部分，i 取值从1到64，单位是弧度。（4294967296=2**32）
    */
    #define FF(a, b, c, d, Mj, s, ti) a = b + (RL((a + F(b,c,d) + Mj + ti),s))
    #define GG(a, b, c, d, Mj, s, ti) a = b + (RL((a + G(b,c,d) + Mj + ti),s))
    #define HH(a, b, c, d, Mj, s, ti) a = b + (RL((a + H(b,c,d) + Mj + ti),s))
    #define II(a, b, c, d, Mj, s, ti) a = b + (RL((a + I(b,c,d) + Mj + ti),s))
    
    
    /////////////////////////////////////////////
    const UINT32 *X = (const UINT32 *)x;

    // correct byte order
    if(hgzIsBigEndian())
    {
        for (int i = 0; i < 512/8; i++)
        {
            hgzRevertByteOrder((UINT8 *)(_x+i), x+i*4, 4);
        }
        X = _x;
    }

    //MD5核心算法,供64轮
    
    a = digest[0];
    b = digest[1];
    c = digest[2];
    d = digest[3];

    /**//* Round 1 */
    FF (a, b, c, d, X[ 0],  7, 0xd76aa478); /**//*  1 */
    FF (d, a, b, c, X[ 1], 12, 0xe8c7b756); /**//*  2 */
    FF (c, d, a, b, X[ 2], 17, 0x242070db); /**//*  3 */
    FF (b, c, d, a, X[ 3], 22, 0xc1bdceee); /**//*  4 */
    FF (a, b, c, d, X[ 4],  7, 0xf57c0faf); /**//*  5 */
    FF (d, a, b, c, X[ 5], 12, 0x4787c62a); /**//*  6 */
    FF (c, d, a, b, X[ 6], 17, 0xa8304613); /**//*  7 */
    FF (b, c, d, a, X[ 7], 22, 0xfd469501); /**//*  8 */
    FF (a, b, c, d, X[ 8],  7, 0x698098d8); /**//*  9 */
    FF (d, a, b, c, X[ 9], 12, 0x8b44f7af); /**//* 10 */
    FF (c, d, a, b, X[10], 17, 0xffff5bb1); /**//* 11 */
    FF (b, c, d, a, X[11], 22, 0x895cd7be); /**//* 12 */
    FF (a, b, c, d, X[12],  7, 0x6b901122); /**//* 13 */
    FF (d, a, b, c, X[13], 12, 0xfd987193); /**//* 14 */
    FF (c, d, a, b, X[14], 17, 0xa679438e); /**//* 15 */
    FF (b, c, d, a, X[15], 22, 0x49b40821); /**//* 16 */

    /**//* Round 2 */
    GG (a, b, c, d, X[ 1],  5, 0xf61e2562); /**//* 17 */
    GG (d, a, b, c, X[ 6],  9, 0xc040b340); /**//* 18 */
    GG (c, d, a, b, X[11], 14, 0x265e5a51); /**//* 19 */
    GG (b, c, d, a, X[ 0], 20, 0xe9b6c7aa); /**//* 20 */
    GG (a, b, c, d, X[ 5],  5, 0xd62f105d); /**//* 21 */
    GG (d, a, b, c, X[10],  9, 0x02441453); /**//* 22 */
    GG (c, d, a, b, X[15], 14, 0xd8a1e681); /**//* 23 */
    GG (b, c, d, a, X[ 4], 20, 0xe7d3fbc8); /**//* 24 */
    GG (a, b, c, d, X[ 9],  5, 0x21e1cde6); /**//* 25 */
    GG (d, a, b, c, X[14],  9, 0xc33707d6); /**//* 26 */
    GG (c, d, a, b, X[ 3], 14, 0xf4d50d87); /**//* 27 */
    GG (b, c, d, a, X[ 8], 20, 0x455a14ed); /**//* 28 */
    GG (a, b, c, d, X[13],  5, 0xa9e3e905); /**//* 29 */
    GG (d, a, b, c, X[ 2],  9, 0xfcefa3f8); /**//* 30 */
    GG (c, d, a, b, X[ 7], 14, 0x676f02d9); /**//* 31 */
    GG (b, c, d, a, X[12], 20, 0x8d2a4c8a); /**//* 32 */

    /**//* Round 3 */
    HH (a, b, c, d, X[ 5],  4, 0xfffa3942); /**//* 33 */
    HH (d, a, b, c, X[ 8], 11, 0x8771f681); /**//* 34 */
    HH (c, d, a, b, X[11], 16, 0x6d9d6122); /**//* 35 */
    HH (b, c, d, a, X[14], 23, 0xfde5380c); /**//* 36 */
    HH (a, b, c, d, X[ 1],  4, 0xa4beea44); /**//* 37 */
    HH (d, a, b, c, X[ 4], 11, 0x4bdecfa9); /**//* 38 */
    HH (c, d, a, b, X[ 7], 16, 0xf6bb4b60); /**//* 39 */
    HH (b, c, d, a, X[10], 23, 0xbebfbc70); /**//* 40 */
    HH (a, b, c, d, X[13],  4, 0x289b7ec6); /**//* 41 */
    HH (d, a, b, c, X[ 0], 11, 0xeaa127fa); /**//* 42 */
    HH (c, d, a, b, X[ 3], 16, 0xd4ef3085); /**//* 43 */
    HH (b, c, d, a, X[ 6], 23, 0x04881d05); /**//* 44 */
    HH (a, b, c, d, X[ 9],  4, 0xd9d4d039); /**//* 45 */
    HH (d, a, b, c, X[12], 11, 0xe6db99e5); /**//* 46 */
    HH (c, d, a, b, X[15], 16, 0x1fa27cf8); /**//* 47 */
    HH (b, c, d, a, X[ 2], 23, 0xc4ac5665); /**//* 48 */

    /**//* Round 4 */
    II (a, b, c, d, X[ 0],  6, 0xf4292244); /**//* 49 */
    II (d, a, b, c, X[ 7], 10, 0x432aff97); /**//* 50 */
    II (c, d, a, b, X[14], 15, 0xab9423a7); /**//* 51 */
    II (b, c, d, a, X[ 5], 21, 0xfc93a039); /**//* 52 */
    II (a, b, c, d, X[12],  6, 0x655b59c3); /**//* 53 */
    II (d, a, b, c, X[ 3], 10, 0x8f0ccc92); /**//* 54 */
    II (c, d, a, b, X[10], 15, 0xffeff47d); /**//* 55 */
    II (b, c, d, a, X[ 1], 21, 0x85845dd1); /**//* 56 */
    II (a, b, c, d, X[ 8],  6, 0x6fa87e4f); /**//* 57 */
    II (d, a, b, c, X[15], 10, 0xfe2ce6e0); /**//* 58 */
    II (c, d, a, b, X[ 6], 15, 0xa3014314); /**//* 59 */
    II (b, c, d, a, X[13], 21, 0x4e0811a1); /**//* 60 */
    II (a, b, c, d, X[ 4],  6, 0xf7537e82); /**//* 61 */
    II (d, a, b, c, X[11], 10, 0xbd3af235); /**//* 62 */
    II (c, d, a, b, X[ 2], 15, 0x2ad7d2bb); /**//* 63 */
    II (b, c, d, a, X[ 9], 21, 0xeb86d391); /**//* 64 */

    digest[0] += a;
    digest[1] += b;
    digest[2] += c;
    digest[3] += d;
}

void CHgzMD5::md5end( UINT8 *begin, UINT32 len, UINT32 originalByteLen )
{
    UINT8 x[512/8]; // 64 bytes
    UINT64 originalBitLen = originalByteLen*8;

    memcpy(x, begin, len);

    // Padding： 高位补1，后续补0
    x[len] = 0x80;
    memset(x+len+1, 0, (512/8)-len-1);

    if(len >= ((512-64)/8))
    {
        md5core(x);
        memset(x, 0, (512/8));
    }

    memcpy(x+(512-64)/8, (const UINT8*)&originalBitLen, (64/8));    //文件末尾加入原文件的bit长度
    md5core(x);
}
