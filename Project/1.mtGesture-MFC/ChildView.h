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
protected:
	// The drawing object
	CDrawingObject m_drawingObject;
	
	// Holds gesture configuration
	CGestureConfig m_gestureConfig;

	// Needed for drawing object position and size calculations
	double	m_dblZoomRatioStart;
	double	m_dblZoomRatioTotal;	
	CPoint	m_ptCenter;

// Overrides
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
	// Gesture handlers
	virtual BOOL OnGestureZoom(CPoint ptCenter, long lDelta);
	virtual BOOL OnGesturePan(CPoint ptFrom, CPoint ptTo);
	virtual BOOL OnGestureRotate(CPoint ptCenter, double dblAngle);
	virtual BOOL OnGesturePressAndTap(CPoint ptFirstFinger, long lDelta);
	virtual BOOL OnGestureTwoFingerTap(CPoint ptCenter);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};