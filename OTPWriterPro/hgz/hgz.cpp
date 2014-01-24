#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <regex>
#include "hgz.h"

using namespace std;
//using std::tcout;
//using namespace std::tr1;


/////////////////////////////////////////////////////////////////////

// ��ȡ�����ַ���
// ****BUG: ���ָ�����Ϊ�ո񣬶��ַ����ں��пո�ʱ��������ȷ�ֿ���ԭ�������ҵ��ڲ�ʵ�����Ȱ�ָ���ָ���ת���˿ո��������ģ����Ժ����ơ�
BOOL hgzExtractSubString(				// �ɹ����� TRUE
		CString& rString,				// ����ţ�iSubString����ȡ�������ַ���
		const CString& cstrFullString,	// ԭ�ַ���
		int iSubString,					// �Ӵ����
		const CString& regexSep )		// �ָ����ַ���
{
	CString strSrc(cstrFullString);
	// ��ȥ����β�ո�
	strSrc.TrimLeft(regexSep);
	strSrc.TrimRight(regexSep);
	
	// regex_constants::syntax_option_type fl = std::regex_constants::icase; // �����������ʽѡ�� - ���Դ�Сд 
	tregex regExpress(regexSep, std::regex_constants::icase);	// �����������ʽ����

	// ���ָ����滻Ϊ�����ո�
	std::tstring tmp =								// �����滻����ַ���
	regex_replace(
		(std::tstring)strSrc.GetString(),	// ԭ�ַ���
		regExpress,									// �������ʽ
		(std::tstring)_T(" "),						// �滻�ַ���
		std::regex_constants::match_any				// ���Ҽ��滻����
		);
	const TCHAR *tmp1 = new TCHAR[32];
		
	// ����ж� std::tstring Ϊ�գ�
	// NULL �� "" �����ַ�����������
	// str.c_str() == "" ���С�
	// ���Գ����Ƿ�Ϊ 0 ���жϡ�
	tmp1 = tmp.length() == 0 ? NULL : tmp.c_str();
	BOOL x = AfxExtractSubString(rString, tmp1, iSubString, _T(' '));
	return x;
}

// ��ȡ�����ַ���
int hgzExtractSubStrings(				// �ɹ�������ȡ���Ӵ��������ɹ����� 0
	CStringArray& arr,				// ��ȡ�����Ӵ�����
	const CString& cstrFullString,	// ԭ�ַ���
	const CString& regexSep )		// �ָ����ַ���
{
	int size = arr.GetSize();
	CString str(cstrFullString);
	CString subStr;
	int i = 0;
	while (hgzExtractSubString(subStr, str, i,	regexSep))
	{
		arr.Add(subStr);
		i++;
	}

	return arr.GetSize() - size;
}

//-----------------------------------------
//  [11/29/2013 hgz]
// ��ȡ�����ַ���
BOOL hgzExtractSubString1(				// �ɹ����� TRUE
	CString& rString,				// ����ţ�iSubString����ȡ�������ַ���
	const CString& cstrFullString,	// ԭ�ַ���
	int iSubString,					// �Ӵ����
	const CString& regexSep )		// �ָ����ַ���
{
	CString strSrc(cstrFullString);
	// ��ȥ����β�ո�
	strSrc.TrimLeft(regexSep);
	strSrc.TrimRight(regexSep);

	// regex_constants::syntax_option_type fl = std::regex_constants::icase; // �����������ʽѡ�� - ���Դ�Сд 
	tregex e(regexSep, std::regex_constants::icase);	// �����������ʽ����

	tsmatch m;
	std::tstring s((std::tstring)strSrc.GetString());
	int i = 0;
	for (; regex_search( s, m,	e ); i++ ) {
		if (i == iSubString) {
			rString = m.prefix().str().c_str();
			return TRUE;
		}
		s = m.suffix().str();
	}
	if (i == iSubString/* the last suffix is the Sub String */ || iSubString == 0 /* the whole string is the 0th sub string */) {
		rString = s.c_str();
		return TRUE;
	}

	return FALSE;

}
// ��ȡ�����ַ���
int hgzExtractSubStrings1(			// �ɹ�������ȡ���Ӵ��������ɹ����� 0
	CStringArray& arr,				// ��ȡ�����Ӵ�����
	const CString& cstrFullString,	// ԭ�ַ���
	const CString& regexSep )		// �ָ����ַ���
{
	//int size = arr.GetSize();
	arr.RemoveAll();
	// first of all, trim the preceding and ending regexSep strings.
	// use CString since it has trim functions.
	if (cstrFullString.GetLength() == 0) return 0;
	CString strSrc(cstrFullString);
	strSrc.TrimLeft(regexSep);
	strSrc.TrimRight(regexSep);
	if (strSrc.GetLength() == 0) return 0;

	// construct the regex object
	// regex_constants::syntax_option_type fl = std::regex_constants::icase; // �����������ʽѡ�� - ���Դ�Сд 
	tregex e(regexSep, std::regex_constants::icase);	// �����������ʽ����

	// m is the search result
	tsmatch m;
	// s is a std string, because regex_search can use only std string as its operands.
	std::tstring s((std::tstring)strSrc.GetString());
	while ( regex_search( s, m,	e ) ) {
		arr.Add(m.prefix().str().c_str());
		s = m.suffix().str();
	}
	arr.Add(s.c_str());

	//return arr.GetSize() - size;
	return arr.GetSize();
}


int hgzSubStringCount(
	const CString& s,				// ԭ�ַ���
	const CString& regexSep )		// �ָ����ַ���
{
	CStringArray arr;
	return hgzExtractSubStrings1(arr, s,regexSep);
}



//void regex_test(CString str)
//{
//	
//	//
//	// �������ʽ
//	//
//	tcout << _T("-------------------------") << endl;
//
//	// ע�� syntax_option_type �� match_flag_type �Ĳ�ͬ
//	regex_constants::syntax_option_type fl = std::regex_constants::icase; // �����������ʽѡ�� - ���Դ�Сд 
//	regex_constants::match_flag_type fl1   = std::regex_constants::match_default;
//    CString regStr(_T(" +"));       // �����������ʽ�ַ���
//	// tregex regExpress(regStr, fl);   // �����������ʽ����
//	 tregex regExpress(regStr.GetString());    // �����������ʽ����
//    // tregex regExpress(_T(" +"));  // �����������ʽ����
//
//    // ������ҵĽ��   
//    //cmatch mr;  
//	match_results<std::string::const_iterator> mr;
//	/*
//	template<class IOtraits, class IOalloc, class Alloc, class Elem, class RXtraits, class Alloc2>
//	bool regex_search(
//		const basic_string<Elem, IOtraits, IOalloc>& str,
//		match_results<typename basic_string<Elem, IOtraits, IOalloc>::const_iterator, Alloc>& match,
//		const basic_regex<Elem, RXtraits, Alloc2>& re,
//		match_flag_type flags = match_default);
//	*/
//
//
//	// search
//	if(regex_search(tstring(str.GetString()), mr, regExpress, fl1))  
//    {  
//        tcout<<_T("�������ʽ: \"")<<regStr<<_T("\"")<<endl;
//		tcout<<_T("����: \"")<<str<<_T("\"")<<endl;
//		tcout<<_T("-- �ɹ�!")<<std::endl;  
//        for(size_t i= 0; i < mr.size(); ++i)  
//        {  
//            tcout<<_T("��")<<i<<_T("�����:\"")<<mr.str(i)<<_T("\"")<<endl;  
//            tcout<<_T("��ʼλ��:")<<mr.position(i);
//			tcout<<endl<<_T("����")<<mr.length(i)<<std::endl;  
//        }  
//        std::tcout<<std::endl;  
//	}
//	
//	// ע�⣺regex_replace ����ַ�������ת��Ϊ std::tstring ���ͷ���ʹ�ã��� regex_search �򲻱ء�
//	CString fmt = _T(" ");
//	// replace
//
//
//	/*
//	// ��ʽ 2��
//	template<class RXtraits, class Alloc, class Elem>
//      basic_string<Elem> regex_replace(
//			const basic_string<Elem>& str,
//			const basic_regex<Elem, RXtraits, Alloc>& re,
//			const basic_string<Elem>& fmt,
//			match_flag_type flags = match_default
//			);
//	*/
//
//	//regex_replace((std::tstring)str.GetString(), regExpress, (std::tstring)fmt.GetString());
//	std::tstring tmp =					// �����滻����ַ���
//	regex_replace(
//		(std::tstring)str.GetString(),	// ԭ�ַ���
//		regExpress,						// �������ʽ
//		(std::tstring)_T(" "),			// �滻�ַ���
//		std::regex_constants::match_any	// ���Ҽ��滻����
//		);
//
//	tcout<<endl<<tmp<<endl;
//	tcout << _T("-------------------------") << endl;
//
//}


// ExtractString ���ԣ��� BUG�����£����ܰ� 01 00 06 00 00 ab 10 3 6 �� a ���ֿ���
void ExtractString(
		CStringArray& arr, 
		const CString& strSrc, 
		const CString& sep = _T("\r\n") )
{
 // Ԥ����: �ɸ�����Ҫ�����Ƿ���ҪTrim���Լ���Trim���ո�/���Ƿָ���/��������
 CString str(strSrc);
 str.TrimLeft(); 
 str.TrimRight();
  
 if(str.IsEmpty())
  return;
 
 // ��ʼ�ֽ�
 int pos = str.Find(sep);
 while (pos != -1)
 {
  //if(!str.Left(pos).IsEmpty()) // ���б�ҪҲ���ڴ�Trim�����ж��Ƿ�Ϊ�գ�Ϊ��������
  arr.Add(str.Left(pos));
 
  str = str.Mid(pos + sep.GetLength());
  pos = str.Find(sep);
 }
 
 arr.Add(str); // think
}

//
// ����Ͽ�׷���ַ���
//
bool AddStrToComboBox(CString &str, CComboBox &cb)
{
	if (!str.IsEmpty() && (cb.FindStringExact(-1, str)) == CB_ERR) // 1����Сд�����У�2����β�ո���Ч��
	{
		cb.AddString(str);
		return TRUE;
	}

	return FALSE;

}

//
//
//
void splitfloat(float x, int &intpart, float &fracpart)
{  
   intpart=int(x); 
   fracpart=x-intpart; 
}




int CStringSplit(CStringArray& dest, const CString& source, const CString& divKey)
{
     dest.RemoveAll();
     int pos = 0;
     int pre_pos = -1;

     int sl = source.GetLength();
	 while( sl != pos )
	 {
         pos = source.Find(divKey, (pos+1));
		 if (pos == -1) pos = sl;
         if (pos != (pre_pos + 1)) // �Ӵ������Թ�
			dest.Add(source.Mid((pre_pos + 1), (pos - (pre_pos + 1))));
		 
		 pre_pos = pos;
     }

	 //for (int i = 0; i < dest.GetSize(); i++)
		// _tprintf_s("%s\n", dest[i]); 

	 return dest.GetSize();
}


int count_ones(unsigned int x, int length)
{
	int ones = 0;

	for (int i = 0; i < length; i++)
	{
		ones += (x>>i) & 0x01;
	}

	return ones;
}


int RandRange( int range_min, int range_max )
{
	// Generate random numbers in the closed interval
	// [range_min, range_max]. In other words,
	// range_min <= random number <= range_max
	return rand() % (range_max - range_min + 1) + range_min;
}

void DeleteSpaceFromString( CString &s )
{
	CStringArray arr;

	ExtractString(arr, s, _T(" "));
	s.Format(_T(""));
	for (int i = 0; i < arr.GetSize(); i++) {
		s.Append(arr[i]);
	}
}

__int64 BinCString2HexInt( CString &b )
{
	__int64 h = 0;

	int len = b.GetLength();
	for (int i = 0; i < len; i++) {
		h += (b[len-1-i] == _T('1')) ? (1<<i) : 0;
		//_tprintf_s(_T("\n%x"), h);
	}

	return h;
}


BOOL hgzOpenConsole()
{
	AllocConsole();  
	_tfreopen(_T("CONOUT$"), _T("w+t"), stdout);  
	_tfreopen(_T("CONIN$"), _T("r+t"), stdin); 

	return TRUE;
}

BOOL hgzCloseConsole()
{
	fclose(stdout);
	fclose(stdin);
	FreeConsole();

	return TRUE;
}

void hgzRevertByteOrder( unsigned char *addr, unsigned int length )
{
	unsigned char x;

	if (length < 2) return;

	for (int i = 0; i < length/2; i++) {
		x = *(addr + i);
		*(addr + i) = *(addr + length - 1 - i);
		*(addr + length - 1 - i) = x;
	}
}

unsigned __int32 hgzRevertByteOrder32( unsigned __int32 x )
{
	hgzRevertByteOrder((unsigned char *)&x, 4);
	return x;
}

unsigned __int16 hgzRevertByteOrder16( unsigned __int16 x )
{
	hgzRevertByteOrder((unsigned char *)&x, 2);
	return x;
}