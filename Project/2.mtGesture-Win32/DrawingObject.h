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
//
// DrawingObject.h
//
// Definition of helper class for drawing and manipulation of rectangle object.

#pragma once
#include "stdafx.h"
#include <windows.h>
#include <math.h>

#define MAX_COLORS 5

// CDrawingObject class holds information about rectangle. Instead of storing
// two oposite angle points we do calculate them dynamically by using other inforamtion
// that we store in this class about rectangle. This way we do not get deformation of
// rectangle due to precission issues.
class CDrawingObject
{
public:
    CDrawingObject(void);

    void ResetObject(const int cxClient,const int cyClient);

    void Paint(HDC hdc);
    void Move(LONG ldx,LONG ldy);
    void TogleDrawDiagonals(void){_bDrawDiagonals = !_bDrawDiagonals;}
    void Zoom(const double dZoomFactor,const LONG iZx,const LONG iZy);
    void Rotate(const double dAngle,const LONG iOx,const LONG iOy);
    void ShiftColor(void);

public:
    ~CDrawingObject(void);

private:
    // This is an array of colors that we will shift through when enduser 
    // generates rollover gesture
    static const COLORREF s_colors[];

    // We do retain center point of rectangle (diagonal intesection)
    LONG    _iCx; 
    LONG    _iCy; 

    // Then we keep inforamtion about width and height of rectangle
    int     _iWidth; 
    int     _iHeight; 

    // Zooming in/out will scale width and height for some factor
    double  _dScalingFactor; 

    // Here we store total rotation angle of rectangle (from x-axis)
    double  _dRotationAngle; 

    // This variable triggers drawing of diagonals if set to true
    bool    _bDrawDiagonals; 

    // This variable defines the color of rectangle (index )
    int     _iColorIndex; 
};