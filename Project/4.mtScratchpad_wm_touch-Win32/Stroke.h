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

// Stroke.h
//
// Definition of helper classes for stroke storage, CStroke, and
// CStrokeCollection.

#pragma once
#include "stdafx.h"
// A CStroke object represents a single stroke, and is made of the trajectory of the finger from
// touch-down to touch-up. The CStroke object has two properties: the color of the stroke
// and the ID. We use the ID to distinguish strokes coming from different fingers.
class CStroke
{
public:
    CStroke();
    CStroke(const CStroke& original);
    const CStroke& operator = (const CStroke& original);
    ~CStroke();

    // Returns last point in the stroke. Returns success status.
    bool GetLastPoint(POINT& pt);
    // Adds a point to the stroke.
    bool AddPoint(const POINT& pt);

    // Property: stroke color
    void SetColor(COLORREF clr) { m_clr = clr; }
    COLORREF GetColor() const { return m_clr; }

    // Property: stroke ID
    void SetId(int id) { m_id = id; }
    int GetId() const { return m_id; }

    // Draw the complete stroke.
    void Draw(HDC hDC) const;
    // Draw only last segment of the stroke.
    void DrawLast(HDC hDC) const;

private:
    COLORREF m_clr;     // Stroke color
    int m_id;           // Stroke ID
    POINT* m_arrPt;     // Array of points
    int m_nPt;          // Number of points

    static const int s_nAlloc = 32; // Allocation granularity
};

// The CStrokeCollection object represents a collection of strokes.
// It supports add stroke and remove stroke operations and finding a stroke by ID.
class CStrokeCollection
{
public:
    CStrokeCollection();
    CStrokeCollection(const CStrokeCollection& original);
    const CStrokeCollection& operator = (const CStrokeCollection& original);
    ~CStrokeCollection();

    // Provides access to the strokes.
    int Count() const { return m_nStrk; }
    CStroke& operator [] (int i);

    // Add the stroke to the collection.
    bool AddStroke(const CStroke& strk);
    // Remove the stroke from the collection.
    void RemoveStroke(int i);
    // Search the collection for given ID.
    int FindStrokeById(int id) const;

    // Draw the collection of the strokes.
    void Draw(HDC hDC) const;

private:
    CStroke* m_arrStrk; // Stroke array
    int m_nStrk;        // Number of strokes

    static const int s_nAlloc = 32; // Allocation granularity
};