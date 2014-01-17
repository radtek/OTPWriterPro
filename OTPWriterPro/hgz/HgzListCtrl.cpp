// HgzListCtrl.cpp : ʵ���ļ�
//

#include "StdAfx.h"
#include "HgzListCtrl.h"



// CItemEdit

IMPLEMENT_DYNAMIC(CItemEdit, CEdit)

	CItemEdit::CItemEdit()
	: m_TempStr(_T(""))
	, m_nStartChar(0)
	, m_nEndChar(0)
	, m_bEndEdit(false)
	, m_bEditingAndOutOfView(false)
{

}

CItemEdit::~CItemEdit()
{
}


BEGIN_MESSAGE_MAP(CItemEdit, CEdit)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CHgzListCtrl

IMPLEMENT_DYNAMIC(CHgzListCtrl, CListCtrl)

CHgzListCtrl::CHgzListCtrl()
{

	m_nItem = 0;
	m_nSubItem = 0;
	m_bEditing = FALSE;

}

CHgzListCtrl::~CHgzListCtrl()
{
	m_edit.DestroyWindow();
}


BEGIN_MESSAGE_MAP(CHgzListCtrl, CListCtrl)
//	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
//	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_KILLFOCUS()
//	ON_WM_CHAR()
//ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CItemEdit ��Ϣ�������
BOOL CItemEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	//����ESC����RETURN�����µ���Ϣ������ΪWM_KILLFOCUS��Ϣ������Ҳ���Ը�����Ҫ������������Ϊ���������ȡ������ı�־
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == 13 ) {       //�س���
			m_bEndEdit = TRUE;
			pMsg->message = WM_KILLFOCUS;
		}
		else if( pMsg->wParam == 27 ) {   //ESC��
			m_bEndEdit = TRUE;
			m_bInputValid = FALSE;    //��ʱ�ı༭�����Ч
			pMsg->message = WM_KILLFOCUS;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

void CItemEdit::OnKillFocus(CWnd* pNewWnd)
{
	
	CEdit::OnKillFocus(pNewWnd);

	// TODO: �ڴ˴������Ϣ����������

	if( m_bEndEdit ) {
		// �õ������ڣ���֪ͨ�����ڽ����༭����
		CHgzListCtrl *parent = (CHgzListCtrl *)GetParent();
		if( parent )
			parent->EndEdit( m_bInputValid );

		m_bInputValid = TRUE;
		m_bEndEdit = TRUE;
	}

}

// CHgzListCtrl ��Ϣ�������
void CHgzListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	POSITION pos;
	BOOL bSelected = FALSE;

	m_nFlags = nFlags;
	m_point = point;

	// ����Ƿ���Item�����༭
	/*if( m_bEditing == TRUE )
	goto defalt_session;*/

	// ����Ƿ���Item��ѡ�У�û��ʱ������༭
	/*pos = GetFirstSelectedItemPosition();
	if( pos )
	{*/
		// �õ��������Item
		LVHITTESTINFO testinfo;
		testinfo.pt = point;                  //���ʱ�����λ��
		testinfo.flags = LVHT_ONITEMLABEL;    //����ı����Ǳ���
		if( SubItemHitTest(&testinfo)<0 )
			goto defalt_session;            //û�е�����Ч���򣬲�����༭

		if (m_bEditing == TRUE) { 
			if (m_nItem == testinfo.iItem && m_nSubItem == testinfo.iSubItem)
				goto defalt_session;
			else
				EndEdit(TRUE);
		}
		
		
		m_nItem = testinfo.iItem;            //�����������к�
		m_nSubItem = testinfo.iSubItem;    //�����������к�

		if(m_nSubItem == 0) {
			goto defalt_session;            //ѡ�е�һ�У����༭
		}
		// ���ñ����Ƿ�ѡ�У�û��ѡ�в�����༭
		//while( pos )
		//	if( m_nItem==GetNextSelectedItem(pos) )
		//	{
		//		bSelected = TRUE;
		//		break;
		//	}
		//	if( bSelected==FALSE )
		//		goto defalt_session;            //û�е�����Ч���򣬲��༭

			// ��ʼ�༭
			m_bEditing = BeginEdit();
			return;
	//}

defalt_session:
	CListCtrl::OnLButtonDown(nFlags, point);
}


BOOL CHgzListCtrl::BeginEdit(void)
{
	// �õ����༭���������
	CRect rect;
	if( GetSubItemRect(m_nItem, m_nSubItem, LVIR_LABEL, rect)==FALSE )
		return FALSE;

	// �����༭�ؼ�
 	if (!m_edit.GetSafeHwnd()) {
		if( m_edit.Create(WS_CHILD | WS_CLIPSIBLINGS | WS_BORDER, rect, this, 1/*ID_MYEDIT*/)==FALSE )
			return FALSE;
	}
	else
		m_edit.MoveWindow(&rect);

	// ȡ���༭���������
	CString txtItem = GetItemText( m_nItem, m_nSubItem );

	// ȡ����������д���༭�ؼ�
	m_edit.SetWindowText( txtItem );
	m_edit.SetFocus();
	m_edit.SetSel( 0, -1 );
	m_edit.ShowWindow( SW_SHOW );

	return TRUE;
}


void CHgzListCtrl::EndEdit(BOOL bValidate)
{
	// �༭�������Ч�ģ����豻�༭���������
	if( bValidate )
	{
		CString txtItem;
		m_edit.GetWindowText( txtItem );
		SetItemText(m_nItem, m_nSubItem, txtItem);
	}

	// ���ٱ༭����
	m_edit.DestroyWindow();
	m_bEditing = FALSE;
	m_edit.m_TempStr.Empty();
}


void CHgzListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);

	HoldOnEditing();
}


BOOL CHgzListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	BOOL b = CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
	HoldOnEditing();
	return b;
}

void CHgzListCtrl::HoldOnEditing()
{
	m_edit.m_bEndEdit = FALSE;
	if (m_bEditing) {
		if (IsItemVisible(m_nItem)) {
			CRect rect;
			if( GetSubItemRect(m_nItem, m_nSubItem, LVIR_LABEL, rect)==FALSE )
				return;
			m_edit.MoveWindow(&rect);
			m_edit.RedrawWindow();
			if (m_edit.m_bEditingAndOutOfView) {
				m_edit.m_bEditingAndOutOfView = FALSE;
				m_edit.SetWindowText(m_edit.m_TempStr);
				m_edit.m_TempStr.Empty();
				m_edit.ShowWindow(SW_SHOW);
				m_edit.SetFocus();
				m_edit.SetActiveWindow();
				m_edit.CreateSolidCaret(1, rect.Height());
				m_edit.SetSel(m_edit.m_nStartChar, m_edit.m_nEndChar);
				m_edit.ShowCaret();
			}
			m_edit.m_bEndEdit = TRUE;
		}
		else {
			m_edit.GetWindowText(m_edit.m_TempStr);
			m_edit.GetSel(m_edit.m_nStartChar, m_edit.m_nEndChar);
			m_edit.m_bEditingAndOutOfView = TRUE;
			m_edit.ShowWindow(SW_HIDE);
		}
	}
}

BOOL CHgzListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if( pMsg->message == WM_KEYDOWN ) {
		if (m_bEditing) {
			EnsureVisible(m_nItem, FALSE);
			HoldOnEditing();
			HWND hWnd = m_edit.GetSafeHwnd();
			pMsg->hwnd = hWnd;
			return m_edit.PreTranslateMessage(pMsg);
		}
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}
