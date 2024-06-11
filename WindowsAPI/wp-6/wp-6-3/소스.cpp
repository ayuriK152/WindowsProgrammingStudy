#include <windows.h> 
#include <TCHAR.H>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ModelessDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 840, 840, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

struct Dot {
	double x;
	double y;
	int heart;
};

struct Shape {
	double x;
	double y;
	int shapenum;
};

struct Dot MainDot[8];
int Button1;
struct Shape MainShape;
int Color[3];
int Button2;
double dx;
double dy;
int Button3;
TCHAR buffer[30];

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	srand((unsigned int)time(NULL));
	HDC mdc;
	HBITMAP hBitmap;
	RECT rt;
	PAINTSTRUCT ps;
	HDC hdc;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	HWND hDlg = NULL;

	static int mx;
	static int my;

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hwnd, 1, 100, NULL);
		SetTimer(hwnd, 2, 10, NULL);
	
		break;

	case WM_RBUTTONDOWN:
		hDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, (DLGPROC)&ModelessDlgProc);
		ShowWindow(hDlg, SW_SHOW);
		break;

	case WM_LBUTTONDOWN:
		if (Button1 == 1)
		{
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			for (int i = 0; i < 7; i++)
			{
				if (MainDot[i].heart == 0)
				{
					MainDot[i].x = mx;
					MainDot[i].y = my;
					MainDot[i].heart = 1;
					break;
				}
			}
			
			if (MainDot[6].heart == 1)
			{
				MainDot[7].x = MainDot[0].x;
				MainDot[7].y = MainDot[0].y;
				MainDot[7].heart = 1;
			}

			dx = 0.0;
			dy = 0.0;
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			if (Button2 == 1)
			{
				MainShape.x += dx;
				MainShape.y += dy;
			}
			break;

		case 2:
			if (Button2 == 1)
			{
				if (Button3 == 0)
				{
					for (int i = 0; i < 7; i++)
					{
						if (MainShape.x == MainDot[i].x && MainShape.y == MainDot[i].y)
						{
							dx = (MainDot[i + 1].x - MainDot[i].x) / 16;
							dy = (MainDot[i + 1].y - MainDot[i].y) / 16;
							break;
						}
					}
				}
				else
				{
					for (int i = 1; i < 8; i++)
					{
						if (MainShape.x == MainDot[i].x && MainShape.y == MainDot[i].y)
						{
							dx = (MainDot[i - 1].x - MainDot[i].x) / 16;
							dy = (MainDot[i - 1].y - MainDot[i].y) / 16;
							break;
						}
					}
				}

			}
			break;
		}

		InvalidateRect(hwnd, NULL, FALSE);
		break;

	case WM_PAINT:
		GetClientRect(hwnd, &rt);
		hdc = BeginPaint(hwnd, &ps);
		mdc = CreateCompatibleDC(hdc); //--- 메모리 DC 만들기
		hBitmap = CreateCompatibleBitmap(hdc, rt.right, rt.bottom); //--- 메모리 DC와 연결할 비트맵 만들기
		SelectObject(mdc, (HBITMAP)hBitmap);
		Rectangle(mdc, 0, 0, rt.right, rt.bottom);

		for (int i = 0; i < 7; i++)
		{
			if (MainDot[i].heart == 1 && MainDot[i + 1].heart == 1)
			{
				MoveToEx(mdc, MainDot[i].x, MainDot[i].y, NULL);
				LineTo(mdc, MainDot[i + 1].x, MainDot[i + 1].y);
			}
		}

		hBrush = CreateSolidBrush(RGB(Color[0], Color[1], Color[2]));
		oldBrush = (HBRUSH)SelectObject(mdc, hBrush);
		if (MainShape.shapenum == 1)
			Ellipse(mdc, (int)(MainShape.x - 10), (int)(MainShape.y - 10), (int)(MainShape.x + 10), (int)(MainShape.y + 10));
		if (MainShape.shapenum == 2)
			Rectangle(mdc, (int)(MainShape.x - 10), (int)(MainShape.y - 10), (int)(MainShape.x + 10), (int)(MainShape.y + 10));
		SelectObject(mdc, oldBrush);
		DeleteObject(hBrush);
	
		TextOut(mdc, (int)(MainShape.x - 50), (int)(MainShape.y - 50), buffer, lstrlen(buffer));

		BitBlt(hdc, 0, 0, rt.right, rt.bottom, mdc, 0, 0, SRCCOPY);

		DeleteDC(mdc);
		DeleteObject(hBitmap);
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		KillTimer(hwnd, 2);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL CALLBACK ModelessDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC mdc;
	HBITMAP hBitmap;
	RECT rt;
	PAINTSTRUCT ps;
	HDC hdc;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	static int Button6Heart = 0;
	static int Button6X = 0;
	static int Button6Y = 380;
	static HWND hList;
	static int selection;

	switch (iMsg) 
	{
	case WM_INITDIALOG:
		SetTimer(hDlg, 1, 10, NULL);
		hList = GetDlgItem(hDlg, IDC_LIST1);
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)L"BlackPink");
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)L"IVE");
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)L"New Jeans");
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RADIO1:
			MainShape.shapenum = 1;
			break;

		case IDC_RADIO2:
			MainShape.shapenum = 2;
			break;

		case IDC_RADIO3:
			Color[0] = 255;
			Color[1] = 0;
			Color[2] = 0;
			break;

		case IDC_RADIO4:
			Color[0] = 0;
			Color[1] = 255;
			Color[2] = 0;
			break;

		case IDC_RADIO5:
			Color[0] = 0;
			Color[1] = 0;
			Color[2] = 255;
			break;

		case IDC_BUTTON1:
			Button1 = 1 - Button1;
			break;

		case IDC_BUTTON2:
			Button2 = 1 - Button2;
			MainShape.x = MainDot[0].x;
			MainShape.y = MainDot[0].y;
			break;

		case IDC_BUTTON3:
			Button3 = 1 - Button3;
			break;

		case IDC_BUTTON4:
			Button2 = 1 - Button2;
			break;

		case IDC_BUTTON5:
			for (int i = 0; i < 7; i++)
			{
					MainDot[i].x = 0;
					MainDot[i].y = 0;
					MainDot[i].heart = 0;
			}
			break;

		case IDC_BUTTON6:
			Button6Heart = 1 - Button6Heart;
			break;

		case IDC_BUTTON7:
			DestroyWindow(hDlg);
			break;
		case IDC_LIST1:
			if (HIWORD(wParam) == LBN_SELCHANGE)
				selection = SendMessage(hList, LB_GETCURSEL, 0, 0);
			if (selection != LB_ERR)
				SendMessage(hList, LB_GETTEXT, selection, (LPARAM)buffer);
		}
		InvalidateRect(hDlg, NULL, FALSE);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			if(Button6Heart == 1 && Button6X < 400)
				Button6X++;
			if (Button6X == 399)
				Button6X = 0;
			break;
		}
		InvalidateRect(hDlg, NULL, FALSE);
		break;

	case WM_PAINT:
		GetClientRect(hDlg, &rt);
		hdc = BeginPaint(hDlg, &ps);
		mdc = CreateCompatibleDC(hdc); //--- 메모리 DC 만들기
		hBitmap = CreateCompatibleBitmap(hdc, rt.right, rt.bottom); //--- 메모리 DC와 연결할 비트맵 만들기
		SelectObject(mdc, (HBITMAP)hBitmap);
		Rectangle(mdc, 0, 0, rt.right, rt.bottom);

		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		oldBrush = (HBRUSH)SelectObject(mdc, hBrush);
		if(Button6Heart == 1)
			Rectangle(mdc, Button6X, Button6Y, Button6X + 20, Button6Y + 20);
		SelectObject(mdc, oldBrush);
		DeleteObject(hBrush);

		BitBlt(hdc, 0, 0, rt.right, rt.bottom, mdc, 0, 0, SRCCOPY);

		DeleteDC(mdc);
		DeleteObject(hBitmap);
		EndPaint(hDlg, &ps);
		break;

	case WM_CLOSE:
		EndDialog(hDlg, 0);
		break;
	}
	return 0;
}