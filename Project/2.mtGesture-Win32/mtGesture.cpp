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

// mtGesture.cpp : Defines the entry point for the application.
//


#include "stdafx.h"


#include "mtGesture.h"
#include "DrawingObject.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
CDrawingObject g_drawingObject;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;


	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MTGESTURE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	//Task2: Check for touch hardware:
	//Check for MultiTouch
	BYTE digitizerStatus = (BYTE)GetSystemMetrics(SM_DIGITIZER);
	if ((digitizerStatus & (0x80 + 0x40)) == 0) //Stack Ready + MultiTouch
	{
		MessageBox(0, L"No touch support is currently availible", L"Error", MB_OK);
		return 1;
	}

	BYTE nInputs = (BYTE)GetSystemMetrics(SM_MAXIMUMTOUCHES);
	wsprintf(szTitle, L"%s - %d touch inputs", szTitle, nInputs);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MTGESTURE));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MTGESTURE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MTGESTURE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   //Enable all gesture types
   GESTURECONFIG gestureConfig;
   gestureConfig.dwID = 0;
   gestureConfig.dwBlock = 0;
   gestureConfig.dwWant = GC_ALLGESTURES;


   BOOL result = SetGestureConfig(hWnd, 0, 1, &gestureConfig, sizeof(gestureConfig));

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT ProcessGestureMessage(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	static POINT lastPoint;
	static ULONGLONG lastArguments;

	GESTUREINFO gi;  

    gi.cbSize = sizeof(GESTUREINFO);
    gi.dwFlags       = 0;
    gi.ptsLocation.x = 0;
    gi.ptsLocation.y = 0;
    gi.dwID          = 0;
    gi.dwInstanceID  = 0;
    gi.dwSequenceID  = 0;

    BOOL bResult = GetGestureInfo((HGESTUREINFO)lParam, &gi);

	switch(gi.dwID)
	{
		case GID_PAN:
			if ((gi.dwFlags & GF_BEGIN) == 0) //not the first message
			{
				g_drawingObject.Move(gi.ptsLocation.x - lastPoint.x,
									 gi.ptsLocation.y - lastPoint.y);
				InvalidateRect(hWnd, NULL, TRUE);
				
			}
			break;

		case GID_ZOOM:
			if ((gi.dwFlags & GF_BEGIN) == 0 && lastArguments != 0) //not the first message
			{
                double zoomFactor = (double)gi.ullArguments/lastArguments;

                // Now we process zooming in/out of object
				g_drawingObject.Zoom(zoomFactor, gi.ptsLocation.x, gi.ptsLocation.y);

				InvalidateRect(hWnd,NULL,TRUE);
			}
			break;

		case GID_ROTATE:
			{
				//The angle is the rotation delta from the last message, 
				//or from 0 if it is a new message
				ULONGLONG lastAngle = ((gi.dwFlags & GF_BEGIN) != 0) ? 0 : lastArguments;
				int angle = (int)(gi.ullArguments - lastAngle);
					
				g_drawingObject.Rotate(GID_ROTATE_ANGLE_FROM_ARGUMENT(angle), 
					gi.ptsLocation.x, gi.ptsLocation.y);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;

		case GID_PRESSANDTAP:
			if ((gi.dwFlags & GF_BEGIN) != 0) //the first message
			{
				g_drawingObject.ShiftColor();
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;

		case GID_TWOFINGERTAP:
			g_drawingObject.TogleDrawDiagonals();
			InvalidateRect(hWnd, NULL, TRUE);
			break;

	}

	//Remember last values for delta calculations
	lastPoint.x = gi.ptsLocation.x;
	lastPoint.y = gi.ptsLocation.y;
	lastArguments = gi.ullArguments;

	CloseGestureInfoHandle((HGESTUREINFO)lParam);
	return 0;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_SIZE:
		g_drawingObject.ResetObject(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		g_drawingObject.Paint(hdc);
		EndPaint(hWnd, &ps);
		break;

	case WM_GESTURE:
		return ProcessGestureMessage(hWnd, wParam, lParam);

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}