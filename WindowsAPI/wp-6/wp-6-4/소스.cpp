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

	switch (uMsg) {
	case WM_CREATE:
		break;

	case WM_RBUTTONDOWN:
		hDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, (DLGPROC)&ModelessDlgProc);
		ShowWindow(hDlg, SW_SHOW);
		break;

	case WM_PAINT:
		GetClientRect(hwnd, &rt);
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
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
	static double startx;
	static double starty;
	static double endx;
	static double endy;
	static int Shape = 0;
	static int Size = 0;
	static int Speed = 0;
	static double Mainx;
	static double Mainy;
	static double dx;
	static double dy;
	static int Draw = 0;
	static int Move = 0;


	switch (iMsg)
	{
	case WM_INITDIALOG:
		SetTimer(hDlg, 1, 100, NULL);
		break;

	case WM_LBUTTONDOWN:
		if (Draw == 1)
		{
			startx = LOWORD(lParam);
			starty = HIWORD(lParam);
		}
		break;

	case WM_LBUTTONUP:
		if (Draw == 1)
		{
			endx = LOWORD(lParam);
			endy = HIWORD(lParam);
			dx = (endx - startx) / 16.0;
			dy = (endy - starty) / 16.0;
			InvalidateRect(hDlg, NULL, FALSE);
		}
		break;


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RADIO1:
			Shape = 1;
			break;
		case IDC_RADIO2:
			Shape = 2;
			break;
		case IDC_RADIO3:
			Size = 1;
			break;
		case IDC_RADIO4:
			Size = 2;
			break;
		case IDC_RADIO5:
			Size = 3;
			break;
		case IDC_RADIO6:
			Speed = 2;
			break;
		case IDC_RADIO7:
			Speed = 1;
			break;
		case IDC_BUTTON1:
			Draw = 1 - Draw;
			break;
		case IDC_BUTTON2:
			Move = 1 - Move;
			Mainx = startx;
			Mainy = starty;
			break;
		case IDC_BUTTON3:
			DestroyWindow(hDlg);
			break;
		}
		InvalidateRect(hDlg, NULL, FALSE);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			if (Move == 1)
			{
				if (Mainx > endx - 5 && Mainx < endx + 5 && Mainy > endy - 5 && Mainy < endy + 5)
				{
					Mainx = startx;
					Mainy = starty;
				}
				Mainx = Mainx + (dx * Speed);
				Mainy = Mainy + (dy * Speed);
			}
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
		hBrush = CreateSolidBrush(RGB(230, 230, 230));
		oldBrush = (HBRUSH)SelectObject(mdc, hBrush);
		Rectangle(mdc, 0, 0, rt.right, rt.bottom);
		SelectObject(mdc, oldBrush);
		DeleteObject(hBrush);

		MoveToEx(mdc, startx, starty, NULL);
		LineTo(mdc, endx, endy);

		if (Move == 1)
		{
			if (Shape == 1)
				Rectangle(mdc, Mainx - (Size * 10), Mainy - (Size * 10), Mainx + (Size * 10), Mainy + (Size * 10));
			else if (Shape == 2)
				Ellipse(mdc, Mainx - (Size * 10), Mainy - (Size * 10), Mainx + (Size * 10), Mainy + (Size * 10));
		}



		BitBlt(hdc, 0, 0, rt.right, rt.bottom, mdc, 0, 0, SRCCOPY);

		DeleteDC(mdc);
		DeleteObject(hBitmap);
		EndPaint(hDlg, &ps);
		break;

	case WM_CLOSE:
		KillTimer(hDlg, 1);
		EndDialog(hDlg, 0);
		break;
	}
	return 0;
}