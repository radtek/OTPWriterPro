#pragma once

//#include "stdafx.h"
#include <string>

using namespace std;

//////////////////////////////
#ifdef _UNICODE
	#define tstring		wstring
	#define tregex		wregex
	#define tcout		wcout
	#define tcmatch		wcmatch
	#define tsmatch		wsmatch
#else
	#define tstring		string
	#define tregex		regex
	#define tcout		cout
	#define tcmatch		cmatch
	#define tsmatch		smatch
#endif
/////////////////////////////



extern BOOL hgzExtractSubString(	// �ɹ����� TRUE
	CString& rString,				// ����ţ�iSubString����ȡ�������ַ���
    const CString& cstrFullString,	// ԭ�ַ���
    int iSubString,					// �Ӵ����
    const CString& regexSep			// �ָ���
	);

extern int hgzExtractSubStrings(	// �ɹ����� TRUE
	CStringArray& arr,				// ��ȡ�����Ӵ�����
    const CString& cstrFullString,	// ԭ�ַ���
    const CString& regexSep			// �ָ����ַ���
	);

extern BOOL hgzExtractSubString1(	// �ɹ����� TRUE
	CString& rString,				// ����ţ�iSubString����ȡ�������ַ���
	const CString& cstrFullString,	// ԭ�ַ���
	int iSubString,					// �Ӵ����
	const CString& regexSep			// �ָ���
	);

extern int hgzExtractSubStrings1(	// �ɹ����� TRUE
	CStringArray& arr,				// ��ȡ�����Ӵ�����
	const CString& cstrFullString,	// ԭ�ַ���
	const CString& regexSep			// �ָ����ַ���
	);

extern int hgzSubStringCount(
	const CString& s,				// ԭ�ַ���
	const CString& regexSep			// �ָ����ַ���
	);

//extern void regex_test(CString str);
extern void ExtractString(CStringArray& arr, const CString& strSrc, const CString& sep);
//
// ����Ͽ�׷���ַ���
//
extern bool AddStrToComboBox(CString &str, CComboBox &cb);
extern void splitfloat(float x, int &intpart, float &fracpart);
extern int CStringSplit(CStringArray& dest, const CString& source, const CString& divKey);
extern int count_ones(unsigned int x, int length);
extern int RandRange( int range_min, int range_max );

//
extern void DeleteSpaceFromString( CString &s );
extern __int64 BinCString2HexInt( CString &b );
extern BOOL hgzOpenConsole();
extern BOOL hgzCloseConsole();

// BigEnding <-> LittleEnding
extern void hgzRevertByteOrder(unsigned char *addr, unsigned int length);
extern unsigned __int32 hgzRevertByteOrder32( unsigned __int32 x );
extern unsigned __int16 hgzRevertByteOrder16( unsigned __int16 x );