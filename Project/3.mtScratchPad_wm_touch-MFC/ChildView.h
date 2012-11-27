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

// ChildView.h : interface of the CChildView class
//


#pragma once

// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Fields
private:
    int m_iCurrColor;                    // The current stroke color
    CStrokeCollection m_StrkColFinished; // The user finished entering strokes  
                                         // after user lifted his or her finger.
    CStrokeCollection m_StrkColDrawing;  // The Strokes collection the user is
                                         // currently drawing. 
private:
    COLORREF GetTouchColor(bool bPrimaryContact);

// Overrides
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL OnTouchInput(CPoint pt, int nInputNumber, int nInputsCount, PTOUCHINPUT pInput);

protected:
    // Handlers for different touch input events
    BOOL OnTouchInputDown(CPoint pt, PTOUCHINPUT pInput);
    BOOL OnTouchInputMove(CPoint pt, PTOUCHINPUT pInput);
    BOOL OnTouchInputUp(CPoint pt, PTOUCHINPUT pInput);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};