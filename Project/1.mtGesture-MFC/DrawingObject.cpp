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
// This class handles manipulation of drawing object (rectangle) as a reaction
// to multi touch gestures
//
// Initially, we define rectangle to be in the middle of the client window. So whenever
// user resize window the rectangle is placed in the middle of client window with
// a width set to half of width of client window and height set to half of the heights of
// client window. Rectangle is placed in the middle of the window. 
//
// Through the gestures user can zoom in, zoom out, move or rotate the rectangle. Also,
// by invoking power tap user can add/remove diagonals from the drawing.
//
#include "stdafx.h"
#include "DrawingObject.h"

// This is initialization of colors that we are going to shift through rectangle object
// whenever user invokes rollover gesture
const COLORREF CDrawingObject::s_colors[] = 
{
	RGB(210,0,0),	 // RED
    RGB(255,139,23), // yellow   
    RGB(146,208,80), // green
    RGB(13,13,13),   // black
    RGB(149,179,215) // blue
};

// Default constructor
CDrawingObject::CDrawingObject(void)
{
    // main window application is responsible to invoke ResetObject function to initialize variables.
    // It shoudl be done whenever main window gets WM_SIZE message.
}

// Destructor
CDrawingObject::~CDrawingObject(void)
{
}

// This function shifts color index by one
void CDrawingObject::ShiftColor(void)
{
    // increase current color index by one
    _iColorIndex++;

    if(MAX_COLORS == _iColorIndex)
    {
        // if index is out of the bound then reset it to zero
        _iColorIndex = 0;
    }
}


// This function resets rectangle object information and it's called by main app
// whenever enduser resize client window
// in:
//      cxClient - new width of client window
//      cyClient - new heights size of client window
void CDrawingObject::ResetObject(const int cxClient,const int cyClient)
{
    // Initial positon of center point is the middle point of client window
    _iCx = cxClient/2;
    _iCy = cyClient/2;

    // Initial width and height are half a size of client window
    _iWidth = cxClient/2;
    _iHeight = cyClient/2;

    // Initial scaling factor is 1.0 (no scaling)
    _dScalingFactor = 1.0;

    // Initial rotation angle is 0.0 (no rotation)
    _dRotationAngle = 0.0; 

    _bDrawDiagonals = false; // no drawing of diagonals


    _iColorIndex = 0; // set initial collor to black
}

// This function will be called by the main app whenever WM_PAINT message is triggered. It's 
// responsible to redraw rectangle. Here we calculate the positon of rectangle angles.
// in:
//      hdc - handle to device context
void CDrawingObject::Paint(CDC* pDC)
{
    double localScale = 1.0;

    localScale = max(_dScalingFactor, 0.05); 

    // create new pen
    CPen pen(PS_SOLID,6,RGB(0,0,0));

    // select new pen for drawing, saving old one
    CPen* oldPen = pDC->SelectObject(&pen);

    // first create a polyline that describes rectangle stratched for scaling factor
    POINT ptRect[5];    

    ptRect[0].x = -(LONG)(localScale * _iWidth/2);
    ptRect[0].y = -(LONG)(localScale * _iHeight/2);

    ptRect[1].x = (LONG)(localScale * _iWidth/2);
    ptRect[1].y = ptRect[0].y;

    ptRect[2].x = ptRect[1].x;
    ptRect[2].y = (LONG)(localScale * _iHeight/2);

    ptRect[3].x = ptRect[0].x;
    ptRect[3].y = ptRect[2].y;
    
    ptRect[4].x = ptRect[0].x;
    ptRect[4].y = ptRect[0].y;

    // now we should rotate rectangle for rotation angle 
    double dCos = cos(_dRotationAngle);
    double dSin = sin(_dRotationAngle);

    for(int i=0; i<5; i++)
    {
        LONG lDX = ptRect[i].x;
        LONG lDY = ptRect[i].y;

        ptRect[i].x = (LONG)(lDX*dCos + lDY*dSin);
        ptRect[i].y = (LONG)(lDY*dCos - lDX*dSin);
    }

    // finally we should translate this rectangle
    for(int i=0; i<5; i++)
    {
        ptRect[i].x += _iCx;
        ptRect[i].y += _iCy;
    }    
	CRgn rgn;
	rgn.CreatePolygonRgn(ptRect,5,WINDING);

	CBrush brush;	
	brush.CreateSolidBrush(s_colors[_iColorIndex]);

	pDC->FillRgn(&rgn, &brush);
    pDC->Polyline(ptRect,5);

    if(_bDrawDiagonals)
    {
        // draw diagonals
        pDC->MoveTo(ptRect[0].x,ptRect[0].y);
        pDC->LineTo(ptRect[2].x,ptRect[2].y);
        pDC->MoveTo(ptRect[1].x,ptRect[1].y);
        pDC->LineTo(ptRect[3].x,ptRect[3].y);
    }

	pDC->SelectObject(oldPen);
}

// This function is responsible to translate center points of the rectangle.
// in:
//      ldx - increment/decrement of the x-coordinate
//      ldy - increment/decrement of the y-coordinate
void CDrawingObject::Move(LONG ldx,LONG ldy)
{
    _iCx += ldx;
    _iCy += ldy;
}

// This function zooms out/in rectangle
// in:
//      dZoomFactor - scaling factor of zoom
//      iZx         - x coordinate of zoom center
//      iZy         - y coordinate of zoom center
void CDrawingObject::Zoom(const double dZoomFactor,const LONG iZx,const LONG iZy)
{
    _dScalingFactor *= dZoomFactor;
}

// This function rotates rectangle
// in:
//      dAngle  - angle of rotation in radians
//      iOx     - x-coordinate of the rotation center
//      iOy     - y-coordinate of the rotation center
//
// Note that during the rotation gesture end user defines center of rotation. So this is going to 
// move the center of rectangle too. That's why we have to recalculate new center point of the
// rectangle too.
void CDrawingObject::Rotate(const double dAngle,const LONG iOx,const LONG iOy)
{   
    _dRotationAngle += dAngle;
}