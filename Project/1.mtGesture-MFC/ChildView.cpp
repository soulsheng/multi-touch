// ----------------------------------------------------------------------------------
// Microsoft Developer & Platform Evangelism
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
// 
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
// EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
// OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// ----------------------------------------------------------------------------------
// The example companies, organizations, products, domain names,
// e-mail addresses, logos, people, places, and events depicted
// herein are fictitious.  No association with any real company,
// organization, product, domain name, email address, logo, person,
// places, or events is intended or should be inferred.
// ----------------------------------------------------------------------------------

// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "TouchGestureDemo.h"
#include "ChildView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Only redraw the box
	m_drawingObject.Paint(&dc);
}



void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_drawingObject.ResetObject(cx, cy);
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetGestureConfig(&m_gestureConfig);

	// Only rotation is not enabled by default
	m_gestureConfig.EnableRotate();

	SetGestureConfig(&m_gestureConfig);

	return 0;
}

BOOL CChildView::OnGestureZoom(CPoint ptCenter, long lDelta)
{
	if ((m_pCurrentGestureInfo->dwFlags & GF_BEGIN) == GF_BEGIN)
	{
		m_dblZoomRatioStart = m_dblZoomRatioTotal = lDelta;
	}
	else if (lDelta != 0)
	{
		m_dblZoomRatioTotal += lDelta;
		double zoomFactor = (double)m_dblZoomRatioTotal / m_dblZoomRatioStart;
		
		m_drawingObject.Zoom(zoomFactor, ptCenter.x, ptCenter.y);
		
		m_dblZoomRatioStart = m_dblZoomRatioTotal;
		RedrawWindow();
	}

	return TRUE;
}

BOOL CChildView::OnGesturePan(CPoint ptFrom, CPoint ptTo)
{
	int dx = ptTo.x - ptFrom.x;
	int dy = ptTo.y - ptFrom.y;

	if (dx != 0 || dy != 0)
	{
		m_drawingObject.Move(dx, dy);
		RedrawWindow();
	}
	return TRUE;
}

BOOL CChildView::OnGestureRotate(CPoint ptCenter, double dblAngle)
{
	if ((m_pCurrentGestureInfo->dwFlags & GF_BEGIN) == GF_BEGIN)
	{
		// Make the first center, the rotating one
		m_ptCenter = ptCenter;
	}
	else if (dblAngle != 0.)
	{
		m_drawingObject.Rotate(dblAngle * PI / 100.0, m_ptCenter.x, m_ptCenter.y);
		RedrawWindow();
	}

	return TRUE;
}

BOOL CChildView::OnGesturePressAndTap(CPoint ptFirstFinger, long lDelta)
{
	if ((m_pCurrentGestureInfo->dwFlags & GF_BEGIN) != 0)
	{
		m_drawingObject.ShiftColor();
		RedrawWindow();
	}

	return TRUE;
}

BOOL CChildView::OnGestureTwoFingerTap(CPoint ptCenter)
{
	m_drawingObject.TogleDrawDiagonals();
	RedrawWindow();

	return TRUE;
}