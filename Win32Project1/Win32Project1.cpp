// Win32Project1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Win32Project1.h"
#include <cstdio>
#define MAX_LOADSTRING 100



char temp[512];
char buttonInfo[512];

#define LOG(X, ...) {sprintf(temp,X, __VA_ARGS__); OutputDebugString(temp);}

bool initFeeder(UINT iInterface = 1);
void setButtonData(int button, bool state, UINT iInterface = 1);
void setJoyData(long X, long Y, long Z, long XR, long ZR, UINT iInterface = 1);



HWND hWnd;
//#define RDB(X, ...) ROS_DEBUG_NAMED("interop",X, __VA_ARGS__)


// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

osgVisual::SpaceMouse * pSpaceMouse = 0;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
//	char temp[512];

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	


	//sprintf(temp,"returned:%d", ret);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			

			//printf("%f.5 %.5f %.5f\n", x, y, z);
			//OutputDebugString("blah");
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
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN32PROJECT1);
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
   

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   strcpy(temp, "initial");
   strcpy(buttonInfo, "initial");

   initFeeder(JOYSTICK_ID);

   pSpaceMouse = new  osgVisual::SpaceMouse();
   int ret = pSpaceMouse->initialize();

   LOG("return:&d", ret);

   SendMessage(hWnd, WM_COMMAND, IDM_ABOUT, IDM_ABOUT);

   return TRUE;
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


void getReadings(HWND hWnd)
{
	double x, y, z,x2,y2,z2;

	pSpaceMouse->getRotations(x, y, z);
	pSpaceMouse->getTranslations(x2, y2, z2);
	sprintf(temp, "Trans:x:%3f y:%3f z:%3.2f  Rotation:x:%.2f y:%.2f z:%.2f\n",x2,y2,z2, x, y, z);
	InvalidateRect(hWnd, 0, TRUE);

}

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
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			SetTimer(hWnd, // window handle
				0, // id of the timer message, leave 0 in this case
				100, // millis
				0 // callback
				);

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

		//            X   Y    
		TextOut(hdc, 20, 10, TEXT(buttonInfo), strlen(buttonInfo));
		TextOut(hdc, 20, 40, TEXT(temp), strlen(temp));
		//pSpaceMouse->getRotations(x, y, z);
		//sprintf(temp, "x:%.2f y:%.2f z:%.2f\n", x, y, z);
		
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:

		getReadings(hWnd);
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
