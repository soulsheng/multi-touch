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

// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

// Stroke.cpp
//
// Implementation of helper classes for stroke storage, CStroke, and
// CStrokeCollection.
#include "stdafx.h"
// Windows header files
#include <windows.h>

// C RunTime header files
#include <stdlib.h>
#include <assert.h>
#define ASSERT assert

// Application header files
#include "stroke.h"

// CStroke constructor.
CStroke::CStroke()
:   m_clr(RGB(0,0,0)),
    m_id(0),
    m_arrPt(NULL),
    m_nPt(0)
{
}

// CStroke copy constuctor
// in:
//      original        source object
CStroke::CStroke(const CStroke& original)
:   m_clr(RGB(0,0,0)),
    m_id(0),
    m_arrPt(NULL),
    m_nPt(0)
{
    operator = (original);
}

// CStroke assignment operator
// in:
//      original        source object
// returns:
//      const reference to this object
const CStroke& CStroke::operator = (const CStroke& original)
{
    if(&original != this)
    {
        // Reallocate the array, if neccessary.
        if(m_nPt < original.m_nPt)
        {
            // Allocate enough chunks of size s_nAlloc to copy the source array.
            // This is more than it is needed. It helps that reallocation doesn't occur too frequently.
            POINT* arrPtNew = (POINT*) realloc(m_arrPt, (original.m_nPt + s_nAlloc - 1)/s_nAlloc*s_nAlloc * sizeof(POINT));
            if(arrPtNew == NULL)
            {
                ASSERT(FALSE && L"CStroke::operator=: failed to realloc point array");
                if(m_arrPt != NULL)
                {
                    free(m_arrPt);
                    m_arrPt = NULL;
                }

                m_nPt = 0;
                return *this;
            }

            m_arrPt = arrPtNew;
        }

        // Copy the array of points.
        if(original.m_nPt > 0)
        {
            memcpy(m_arrPt, original.m_arrPt, original.m_nPt*sizeof(POINT));
        }
        m_nPt = original.m_nPt;

        // Copy the properties.
        m_id = original.m_id;
        m_clr = original.m_clr;
    }

    return *this;
}

// CStroke destructor
CStroke::~CStroke()
{
    if(m_arrPt != NULL)
    {
        free(m_arrPt);
    }
}

// Returns the last point added in the stroke.
// out:
//      pt      point
// returns:
//      flag, determines if output argument, pt, is valid
bool CStroke::GetLastPoint(POINT& pt)
{
    if(m_nPt < 1)
    {
        pt.x = -1;
        pt.y = -1;

        return false;
    }

    pt = m_arrPt[m_nPt-1];

    return true;
}

// Adds a point to the stroke.
// in:
//      pt      new point
// returns:
//      success status
bool CStroke::AddPoint(const POINT& pt)
{
    // If there are no more unused elements in the array, allocate one more chunk of size s_nAlloc.
    if(m_nPt % s_nAlloc == 0)
    {
        POINT* arrPtNew = (POINT*) realloc(m_arrPt, (m_nPt + s_nAlloc)*sizeof(POINT));
        if(arrPtNew == NULL)
        {
            ASSERT(FALSE && L"CStroke::AddPoint: failed to realloc point array");
            if(m_arrPt != NULL)
            {
                free(m_arrPt);
                m_arrPt = NULL;
            }
            m_nPt = 0;

            return false;
        }

        m_arrPt = arrPtNew;
    }

    // Add the point to the array.
    m_arrPt[m_nPt++] = pt;

    return true;
}

// Draws a complete stroke onto a given device context.
// in:
//      hDC     handle to device context
void CStroke::Draw(HDC hDC) const
{
    if(m_nPt < 2)
    {
        return;
    }

    HPEN hPen = ::CreatePen(PS_SOLID, 3, m_clr);
    HGDIOBJ hOldPen = ::SelectObject(hDC, hPen);
    ::Polyline(hDC, m_arrPt, m_nPt);
    ::SelectObject(hDC, hOldPen);
    ::DeleteObject(hPen);
}

// Draw last segment of the stroke to the device context.
// in:
//      hDC     handle to device context
void CStroke::DrawLast(HDC hDC) const
{
    if(m_nPt < 2)
    {
        return;
    }

    HPEN hPen = ::CreatePen(PS_SOLID, 3, m_clr);
    HGDIOBJ hOldPen = ::SelectObject(hDC, hPen);
    ::MoveToEx(hDC, m_arrPt[m_nPt-2].x, m_arrPt[m_nPt-2].y, NULL);
    ::LineTo(hDC, m_arrPt[m_nPt-1].x, m_arrPt[m_nPt-1].y);
    ::SelectObject(hDC, hOldPen);
    ::DeleteObject(hPen);
}

///////////////////////////////////////////////////////////////////////////////

// CStrokeCollection constructor
CStrokeCollection::CStrokeCollection()
:   m_arrStrk(NULL),
    m_nStrk(0)
{
}

// CStrokeCollection copy constructor
// in:
//      original        source object
CStrokeCollection::CStrokeCollection(const CStrokeCollection& original)
:   m_arrStrk(NULL),
    m_nStrk(0)
{
    operator = (original);
}

// CStrokeCollection assignment operator
// in:
//      original        source object
// returns:
//      const reference to this object
const CStrokeCollection& CStrokeCollection::operator = (const CStrokeCollection& original)
{
    if(&original != this)
    {
        // Reallocate the stroke array, if neccessary.
        if(m_nStrk < original.m_nStrk)
        {
            if(m_arrStrk != NULL)
            {
                delete [] m_arrStrk;
            }
            m_arrStrk = new CStroke [(original.m_nStrk + s_nAlloc - 1)/s_nAlloc*s_nAlloc];
            if(m_arrStrk == NULL)
            {
                m_nStrk = 0;
                return *this;
            }
        }

        // Copy the strokes.
        for(int i=0; i<original.m_nStrk; ++i)
        {
            m_arrStrk[i] = original.m_arrStrk[i];
        }
        m_nStrk = original.m_nStrk;
    }

    return *this;
}

// CStrokeCollection destructor
CStrokeCollection::~CStrokeCollection()
{
    if(m_arrStrk != NULL)
    {
        delete [] m_arrStrk;
    }
}

// Access to stroke i
// in:
//      i       stroke index
// returns:
//      stroke, by reference
CStroke& CStrokeCollection::operator [] (int i)
{
    ASSERT(i>=0 && i<m_nStrk);
    return m_arrStrk[i];
}

// Adds the stroke to the collection.
// in:
//      strk        stroke to be added
// returns:
//      success status
bool CStrokeCollection::AddStroke(const CStroke& strk)
{
    // If there are no more unused strokes in the array, allocate one chunk of size s_nAlloc more.
    if(m_nStrk % s_nAlloc == 0)
    {
        CStroke* arrStrk = new CStroke [m_nStrk + s_nAlloc];
        if(arrStrk == NULL)
        {
            return false;
        }

        for(int i=0; i<m_nStrk; ++i)
        {
            arrStrk[i] = m_arrStrk[i];
        }

        if(m_arrStrk != NULL)
        {
            delete [] m_arrStrk;
        }
        m_arrStrk = arrStrk;
    }

    // Add the new stroke to the array.
    m_arrStrk[m_nStrk++] = strk;

    return true;
}

// Removes the stroke from the collection.
// in:
//      i       stroke index
void CStrokeCollection::RemoveStroke(int i)
{
    ASSERT(i>=0 && i<m_nStrk);

    // Destroy the content of stroke i (invoke its destructor).
    m_arrStrk[i] = CStroke();

    // If the stroke i is not the last element in the array,
    // shift all the elements after i one place backwards.
    if(m_nStrk - i - 1 > 0)
    {
        // Move the memory one element backwards.
        memmove(m_arrStrk + i, m_arrStrk + i + 1, (m_nStrk - i - 1) * sizeof(CStroke));
        // Zero out last position in the array, to turn its destructor into a no-op.
        memset(m_arrStrk + m_nStrk - 1, 0, sizeof(CStroke));
    }
    --m_nStrk;
}

// Searches the collection for the given ID.
// in:
//      id      stroke ID
// returns:
//      stroke index in the array, or -1 if not found
int CStrokeCollection::FindStrokeById(int id) const
{
    for(int i=0; i<m_nStrk; ++i)
    {
        if(m_arrStrk[i].GetId() == id)
        {
            return i;
        }
    }

    return -1;
}

// Draw the complete stroke collection onto given device context.
// in:
//      hDC     handle to device context
void CStrokeCollection::Draw(HDC hDC) const
{
    for(int i=0; i<m_nStrk; ++i)
    {
        m_arrStrk[i].Draw(hDC);
    }
}