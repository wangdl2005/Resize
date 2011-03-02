// InputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Resize.h"
#include "InputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputDlg dialog


CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputDlg)
	m_wid_origin = _T("");
	m_h_origin = _T("");
	m_h_to = 200;
	m_wid_to = 100;
	//}}AFX_DATA_INIT
}


void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputDlg)
	DDX_Text(pDX, IDC_WIDORIGIN, m_wid_origin);
	DDX_Text(pDX, IDC_HORIGIN, m_h_origin);
	DDX_Text(pDX, IDC_HTO, m_h_to);
	DDV_MinMaxInt(pDX, m_h_to, 0, 1000);
	DDX_Text(pDX, IDC_WIDTO, m_wid_to);
	DDV_MinMaxInt(pDX, m_wid_to, 0, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
	//{{AFX_MSG_MAP(CInputDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputDlg message handlers
