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
#include "StrokeCollection.h"


CStrokeCollection::CStrokeCollection(void)
{
}


CStrokeCollection::~CStrokeCollection(void)
{
}
///////////////////////////////////////////////////////////////////////////////

// Searches the collection for the given ID.
// in:
//      id      stroke ID
// returns:
//      stroke index in the array, or -1 if not found
int CStrokeCollection::FindStrokeById(int id) const
{
	for (int i = 0; i < GetCount(); i++)
    {
        if (GetAt(i)->GetId() == id)
        {
            return i;
        }
    }

    return -1;
}


// Draw the complete stroke collection onto given device context.
// in:
//      hDC     handle to device context
void CStrokeCollection::Draw(CDC* pDC) const
{
	for (int i = 0; i < GetCount(); ++i)
    {
        GetAt(i)->Draw(pDC);
    }
}