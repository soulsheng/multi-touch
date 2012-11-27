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

// MTScratchpadWMTouch.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MTScratchpadWMTouch.h"
#include "Stroke.h"

#define MAX_LOADSTRING 100

// Global Variables:
CStrokeCollection g_StrkColFinished; // The user finished entering strokes. 
                                    // The user lifted his or her finger.
CStrokeCollection g_StrkColDrawing; // The Strokes collection the user is
                                   // currently drawing. 

HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

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
	LoadString(hInstance, IDC_MTSCRATCHPADWMTOUCH, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

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

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MTSCRATCHPADWMTOUCH));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MTSCRATCHPADWMTOUCH));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MTSCRATCHPADWMTOUCH);
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

    // Register the application window for receiving multi-touch input.
    if(!RegisterTouchWindow(hWnd, 0))
    {
        MessageBox(hWnd, L"Cannot register application window for touch input", L"Error", MB_OK);
        return FALSE;
    }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// Returns color for the newly started stroke.
// in:
//      bPrimaryContact     flag, whether the contact is the primary contact
// returns:
//      COLORREF, color of the stroke
COLORREF GetTouchColor(bool bPrimaryContact)
{
    static int s_iCurrColor = 0;    // Rotating secondary color index
    static COLORREF s_arrColor[] =  // Secondary colors array
    {
        RGB(255, 0, 0),             // Red
        RGB(0, 255, 0),             // Green
        RGB(0, 0, 255),             // Blue
        RGB(0, 255, 255),           // Cyan
        RGB(255, 0, 255),           // Magenta
        RGB(255, 255, 0)            // Yellow
    };

    COLORREF color;
    if (bPrimaryContact)
    {
        // The application renders the primary contact in black.
        color = RGB(0,0,0);         // Black
    }
    else
    {
        // Take the current secondary color.
        color = s_arrColor[s_iCurrColor];

        // Move to the next color in the array.
        s_iCurrColor = (s_iCurrColor + 1) % (sizeof(s_arrColor)/sizeof(s_arrColor[0]));
    }

    return color;
}

// Extracts contact point in client area coordinates (pixels) from a TOUCHINPUT
// structure.
// in:
//      hWnd        window handle
//      ti          TOUCHINPUT structure (info about contact)
// returns:
//      POINT with contact coordinates
POINT GetTouchPoint(HWND hWnd, const TOUCHINPUT& ti)
{
    POINT pt;
    pt.x = TOUCH_COORD_TO_PIXEL(ti.x);
    pt.y = TOUCH_COORD_TO_PIXEL(ti.y);
    ScreenToClient(hWnd, &pt);
    return pt;
}

// Handler for touch-down input.
// in:
//      hWnd        window handle
//      ti          TOUCHINPUT structure (info about contact)

void OnTouchDownHandler(HWND hWnd, const TOUCHINPUT& ti)
{
    // Create a new stroke, add a point, and assign a color to it.
    CStroke strkNew;
	POINT p = GetTouchPoint(hWnd, ti); 
	
    strkNew.AddPoint(p);
    strkNew.SetColor(GetTouchColor((ti.dwFlags & TOUCHEVENTF_PRIMARY) != 0));
	strkNew.SetId(ti.dwID);

    // Add the new stroke to the collection of strokes being drawn.
    g_StrkColDrawing.AddStroke(strkNew);
}

// Handler for touch-move input.
// in:
//      hWnd        window handle
//      ti          TOUCHINPUT structure (info about contact)
void OnTouchMoveHandler(HWND hWnd, const TOUCHINPUT& ti)
{
    // Find the stroke in the collection of the strokes being drawn.
	int iStrk = g_StrkColDrawing.FindStrokeById(ti.dwID);

	POINT p = GetTouchPoint(hWnd, ti); 

    // Add the contact point to the stroke.
    g_StrkColDrawing[iStrk].AddPoint(p);

    // Partial redraw: redraw only the last line segment.
    HDC hDC = GetDC(hWnd);
    g_StrkColDrawing[iStrk].DrawLast(hDC);
    ReleaseDC(hWnd, hDC);
}


// Handler for touch-up message.
// in:
//      hWnd        window handle
//      ti          TOUCHINPUT structure (info about contact)
void OnTouchUpHandler(HWND hWnd, const TOUCHINPUT& ti)
{

    // Find the stroke in the collection of the strokes being drawn.
	int iStrk = g_StrkColDrawing.FindStrokeById(ti.dwID);

    // Add the finished stroke to the collection of finished strokes.
    g_StrkColFinished.AddStroke(g_StrkColDrawing[iStrk]);

    // Remove finished stroke from the collection of strokes being drawn.
    g_StrkColDrawing.RemoveStroke(iStrk);

    // Redraw the window.
    InvalidateRect(hWnd, NULL, FALSE);
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
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
        // Full redraw: draw complete collection of finished strokes and
        // also all the strokes that are currently in drawing.
        g_StrkColFinished.Draw(hdc);
        g_StrkColDrawing.Draw(hdc);
        EndPaint(hWnd, &ps);
        break;

		case WM_TOUCH:
        {
			// A WM_TOUCH message can contain several messages from different contacts
            // packed together.
            unsigned int numInputs = (int) wParam;      // Number of actual per-contact messages
            TOUCHINPUT* ti = new TOUCHINPUT[numInputs]; // Allocate the storage for the parameters of the per-contact messages
            
			// Unpack message parameters into the array of TOUCHINPUT structures, each
            // representing a message for one single contact.
			if (GetTouchInputInfo((HTOUCHINPUT)lParam, numInputs, ti, sizeof(TOUCHINPUT)))
            {
				// For each contact, dispatch the message to the appropriate message
                // handler.
                for (unsigned int i=0; i<numInputs; ++i)
                {
					if (ti[i].dwFlags & TOUCHEVENTF_DOWN)
                    {
						OnTouchDownHandler(hWnd, ti[i]);
                    }
                    else if (ti[i].dwFlags & TOUCHEVENTF_MOVE)
                    {
						OnTouchMoveHandler(hWnd, ti[i]);
                    }
                    else if (ti[i].dwFlags & TOUCHEVENTF_UP)
                    {
						OnTouchUpHandler(hWnd, ti[i]);
					}
				}
			}
			CloseTouchInputHandle((HTOUCHINPUT)lParam);
            delete [] ti;
        }
        break;
	
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