// View_Device.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KEditor.h"
#include "View_Device.h"


// View_Device

IMPLEMENT_DYNCREATE(View_Device, CView)

View_Device::View_Device()
{

}

View_Device::~View_Device()
{
}

BEGIN_MESSAGE_MAP(View_Device, CView)
END_MESSAGE_MAP()


// View_Device �׸����Դϴ�.

void View_Device::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// View_Device �����Դϴ�.

#ifdef _DEBUG
void View_Device::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void View_Device::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// View_Device �޽��� ó�����Դϴ�.