// ResizeDoc.cpp : implementation of the CResizeDoc class
//

#include "stdafx.h"
#include "Resize.h"

#include "ResizeDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizeDoc

IMPLEMENT_DYNCREATE(CResizeDoc, CDocument)

BEGIN_MESSAGE_MAP(CResizeDoc, CDocument)
	//{{AFX_MSG_MAP(CResizeDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizeDoc construction/destruction

CResizeDoc::CResizeDoc()
{
	// TODO: add one-time construction code here

}

CResizeDoc::~CResizeDoc()
{
}

BOOL CResizeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CResizeDoc serialization

void CResizeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CResizeDoc diagnostics

#ifdef _DEBUG
void CResizeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CResizeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CResizeDoc commands
