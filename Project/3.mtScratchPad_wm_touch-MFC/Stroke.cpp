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

#include "StdAfx.h"
#include "Stroke.h"

CStroke::CStroke(int id, COLORREF color):
	m_clr(color),
    m_id(id)
{
	// Initially empty, but allow to grow rapidly
	SetSize(0, 1000);
}

// Draws a complete stroke onto given device context.
// in:
//      pDC     handle to device context
void CStroke::Draw(CDC* pDC) const
{
	if (GetCount() <= 0)
		return;

	CPen pen(PS_SOLID, 3, m_clr);
	CPen* oldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(GetAt(0));

	pDC->Polyline(GetData(), GetCount());
	pDC->SelectObject(oldPen);
}

CStroke::~CStroke(void)
{
}