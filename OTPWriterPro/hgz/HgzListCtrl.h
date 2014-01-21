#pragma once
#include "atltypes.h"


#define WM_LVM_ITEMCHANGED (WM_USER + 1)

// CItemEdit

class CItemEdit : public CEdit
{
	DECLARE_DYNAMIC(CItemEdit)

public:
	CItemEdit();
	virtual ~CItemEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	bool m_bInputValid;
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	// ��������ʱʧȥ����ʱ�����ַ�����
	CString m_TempStr;
	int m_nStartChar;
	int m_nEndChar;
	bool m_bEndEdit;
	bool m_bEditingAndOutOfView;
};

// CHgzListCtrl

class CHgzListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CHgzListCtrl)

public:
	CHgzListCtrl();
	virtual ~CHgzListCtrl();

protected:
	DECLARE_MESSAGE_MAP()

private:
	CItemEdit m_edit;
	bool m_bEditing;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	BOOL BeginEdit(void);
	void EndEdit(BOOL bValidate);
	int m_nItem;
	int m_nSubItem;

	UINT m_nFlags;
	CPoint m_point;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void HoldOnEditing();

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


