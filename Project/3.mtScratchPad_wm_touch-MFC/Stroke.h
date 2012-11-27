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

#pragma once
#include "afxtempl.h"

#include <vector>
using namespace std;

class CStroke : public CArray<POINT, POINT>
{
public:
	CStroke(int id, COLORREF clr);
	~CStroke(void);

    // Property: stroke color
    COLORREF GetColor() const { return m_clr; }

    // Property: stroke ID
    int GetId() const { return m_id; }

    // Draw the complete stroke.
    void Draw(CDC* pDC) const;

private:
    COLORREF m_clr;       // Stroke color
    int m_id;             // Stroke ID
};