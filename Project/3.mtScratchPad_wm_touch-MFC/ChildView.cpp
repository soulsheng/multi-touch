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
#include "ScratchPad.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView() : m_iCurrColor(0)
{
}

CChildView::~CChildView()
{
    for (int i = 0; i < m_StrkColDrawing.GetCount(); ++i)
    {
        delete m_StrkColDrawing[i];
    }

    for (int i = 0; i < m_StrkColFinished.GetCount(); ++i)
    {
        delete m_StrkColFinished[i];
    }
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
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
	
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
    m_StrkColFinished.Draw(&dc);
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here
    
    if (!RegisterTouchWindow())
    {
        ASSERT(FALSE);
    }

    return 0;
}

BOOL CChildView::OnTouchInput(CPoint pt, int nInputNumber, int nInputsCount, PTOUCHINPUT pInput)
{     
	if ((pInput->dwFlags & TOUCHEVENTF_DOWN) == TOUCHEVENTF_DOWN) // Touch Down event
	{
		return OnTouchInputDown(pt, pInput);
	}
	else if ((pInput->dwFlags & TOUCHEVENTF_MOVE) == TOUCHEVENTF_MOVE) // Touch Move event
	{
		return OnTouchInputMove(pt, pInput);
	}
	else if ((pInput->dwFlags & TOUCHEVENTF_UP) == TOUCHEVENTF_UP) // Touch Move event
	{
		return OnTouchInputUp(pt, pInput);
	}

    return FALSE;
}

BOOL CChildView::OnTouchInputDown(CPoint pt, PTOUCHINPUT pInput)
{
    // Create new stroke and add point to it.
	COLORREF strokeColor = GetTouchColor((pInput->dwFlags & TOUCHEVENTF_PRIMARY) != 0);

	CStroke* pStrkNew = new CStroke(pInput->dwID, strokeColor);
	pStrkNew->Add(pt);

    // Add new stroke to the collection of strokes in drawing.
    m_StrkColDrawing.Add(pStrkNew);	
	
	return TRUE;
}

BOOL CChildView::OnTouchInputMove(CPoint pt, PTOUCHINPUT pInput)
{
    // Find the stroke in the collection of the strokes in drawing.
    int strokeIndex = m_StrkColDrawing.FindStrokeById(pInput->dwID);

	if (strokeIndex >= 0)
	{
		CStroke* pStrk =  m_StrkColDrawing[strokeIndex];

        // Add contact point to the stroke
        pStrk->Add(pt);

        // Draw the last stroke
    	pStrk->Draw(GetDC());
	}

	return TRUE;
}

BOOL CChildView::OnTouchInputUp(CPoint pt, PTOUCHINPUT pInput)
{
    // Find the stroke in the collection of the strokes in drawing.
    int strokeIndex = m_StrkColDrawing.FindStrokeById(pInput->dwID);

	if (strokeIndex >= 0)
	{
		CStroke* pStrkCopy = m_StrkColDrawing[strokeIndex];

    	// Remove this stroke from the collection of strokes in drawing.
		m_StrkColDrawing.RemoveAt(strokeIndex);

		// Add this stroke to the collection of finished strokes.
    	m_StrkColFinished.Add(pStrkCopy);
	}

	return TRUE;
}

COLORREF CChildView::GetTouchColor(bool bPrimaryContact)
{
    static COLORREF c_arrColor[] =  // Secondary colors array
    {
        RGB(255, 0, 0),             // Red
        RGB(0, 255, 0),             // Green
        RGB(0, 0, 255),             // Blue
        RGB(0, 255, 255),           // Cyan
        RGB(255, 0, 255),           // Magenta
        RGB(255, 255, 0)            // Yellow
    };

    COLORREF color;
    if (bPrimaryContact)
    {
        // The primary contact is drawn in black.
        color = RGB(0,0,0);         // Black
    }
    else
    {
        // Take current secondary color.
        color = c_arrColor[m_iCurrColor];

        // Move to the next color in the array.
        m_iCurrColor = (m_iCurrColor + 1) % (sizeof(c_arrColor)/sizeof(c_arrColor[0]));
    }

    return color;
}