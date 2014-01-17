// HgzListCtrl.cpp : 实现文件
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



// CItemEdit 消息处理程序
BOOL CItemEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//拦截ESC键和RETURN键按下的消息，解释为WM_KILLFOCUS消息，这里也可以根据需要设置其它键作为输入结束或取消输入的标志
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == 13 ) {       //回车键
			m_bEndEdit = TRUE;
			pMsg->message = WM_KILLFOCUS;
		}
		else if( pMsg->wParam == 27 ) {   //ESC键
			m_bEndEdit = TRUE;
			m_bInputValid = FALSE;    //此时的编辑结果无效
			pMsg->message = WM_KILLFOCUS;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

void CItemEdit::OnKillFocus(CWnd* pNewWnd)
{
	
	CEdit::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码

	if( m_bEndEdit ) {
		// 得到父窗口，并通知父窗口结束编辑过程
		CHgzListCtrl *parent = (CHgzListCtrl *)GetParent();
		if( parent )
			parent->EndEdit( m_bInputValid );

		m_bInputValid = TRUE;
		m_bEndEdit = TRUE;
	}

}

// CHgzListCtrl 消息处理程序
void CHgzListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	POSITION pos;
	BOOL bSelected = FALSE;

	m_nFlags = nFlags;
	m_point = point;

	// 检查是否有Item正被编辑
	/*if( m_bEditing == TRUE )
	goto defalt_session;*/

	// 检查是否有Item被选中，没有时不进入编辑
	/*pos = GetFirstSelectedItemPosition();
	if( pos )
	{*/
		// 得到被点击的Item
		LVHITTESTINFO testinfo;
		testinfo.pt = point;                  //点击时的鼠标位置
		testinfo.flags = LVHT_ONITEMLABEL;    //点击的必须是标题
		if( SubItemHitTest(&testinfo)<0 )
			goto defalt_session;            //没有点在有效区域，不进入编辑

		if (m_bEditing == TRUE) { 
			if (m_nItem == testinfo.iItem && m_nSubItem == testinfo.iSubItem)
				goto defalt_session;
			else
				EndEdit(TRUE);
		}
		
		
		m_nItem = testinfo.iItem;            //被点击表项的行号
		m_nSubItem = testinfo.iSubItem;    //被点击表项的列号

		if(m_nSubItem == 0) {
			goto defalt_session;            //选中第一列，不编辑
		}
		// 检查该表项是否被选中，没被选中不进入编辑
		//while( pos )
		//	if( m_nItem==GetNextSelectedItem(pos) )
		//	{
		//		bSelected = TRUE;
		//		break;
		//	}
		//	if( bSelected==FALSE )
		//		goto defalt_session;            //没有点在有效区域，不编辑

			// 开始编辑
			m_bEditing = BeginEdit();
			return;
	//}

defalt_session:
	CListCtrl::OnLButtonDown(nFlags, point);
}


BOOL CHgzListCtrl::BeginEdit(void)
{
	// 得到被编辑表项的区域
	CRect rect;
	if( GetSubItemRect(m_nItem, m_nSubItem, LVIR_LABEL, rect)==FALSE )
		return FALSE;

	// 创建编辑控件
 	if (!m_edit.GetSafeHwnd()) {
		if( m_edit.Create(WS_CHILD | WS_CLIPSIBLINGS | WS_BORDER, rect, this, 1/*ID_MYEDIT*/)==FALSE )
			return FALSE;
	}
	else
		m_edit.MoveWindow(&rect);

	// 取被编辑表项的文字
	CString txtItem = GetItemText( m_nItem, m_nSubItem );

	// 取出的文字填写到编辑控件
	m_edit.SetWindowText( txtItem );
	m_edit.SetFocus();
	m_edit.SetSel( 0, -1 );
	m_edit.ShowWindow( SW_SHOW );

	return TRUE;
}


void CHgzListCtrl::EndEdit(BOOL bValidate)
{
	// 编辑结果是有效的，重设被编辑表项的文字
	if( bValidate )
	{
		CString txtItem;
		m_edit.GetWindowText( txtItem );
		SetItemText(m_nItem, m_nSubItem, txtItem);
	}

	// 销毁编辑窗口
	m_edit.DestroyWindow();
	m_bEditing = FALSE;
	m_edit.m_TempStr.Empty();
}


void CHgzListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);

	HoldOnEditing();
}


BOOL CHgzListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

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
	// TODO: 在此添加专用代码和/或调用基类

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
