#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include "resource.h"

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
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

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	srand((unsigned int)time(NULL));
	PAINTSTRUCT ps;
	HDC hDC, mDC;
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;
	RECT rt;
	BITMAP bmp;
	static HBITMAP hBitmap;
	static POINT pivot, mousePoint;
	static int selectedNum;
	static bool isStarted;

	switch (uMsg) {
	case WM_CREATE:
		GetClientRect(hWnd, &rt);
		pivot.x = rt.right;
		pivot.y = rt.bottom;
		selectedNum = 0;
		isStarted = false;
		break;

	case WM_PAINT:
		GetClientRect(hWnd, &rt);
		hDC = BeginPaint(hWnd, &ps);
		mDC = CreateCompatibleDC(hDC);
		SelectObject(mDC, hBitmap);
		GetObject(hBitmap, sizeof(BITMAP), &bmp);

		StretchBlt(hDC, 0, 0, pivot.x, pivot.y, mDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		StretchBlt(hDC, pivot.x, 0, rt.right - pivot.x, pivot.y, mDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		StretchBlt(hDC, 0, pivot.y, pivot.x, rt.bottom - pivot.y, mDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		StretchBlt(hDC, pivot.x, pivot.y, rt.right - pivot.x, rt.bottom - pivot.y, mDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

		hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
		oldPen = (HPEN)SelectObject(hDC, hPen);
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

		switch (selectedNum) {
		case 1:
			Rectangle(hDC, 0, 0, pivot.x, pivot.y);
			break;
		case 2:
			Rectangle(hDC, pivot.x, 0, rt.right, pivot.y);
			break;
		case 3:
			Rectangle(hDC, 0, pivot.y, pivot.x, rt.bottom);
			break;
		case 4:
			Rectangle(hDC, pivot.x, pivot.y, rt.right, rt.bottom);
			break;
		}

		SelectObject(hDC, oldPen);
		DeleteObject(hPen);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);

		DeleteDC(mDC);
		EndPaint(hWnd, &ps);
		break;

	case WM_TIMER:
		hDC = GetDC(hWnd);

		ReleaseDC(hWnd, hDC);
		break;

	case WM_KEYDOWN:

		break;

	case WM_LBUTTONDOWN:
		if (!isStarted)
			break;
		mousePoint.x = LOWORD(lParam);
		mousePoint.y = HIWORD(lParam);
		if (mousePoint.x < pivot.x && mousePoint.y < pivot.y) {
			selectedNum = 1;
		}
		else if (mousePoint.x > pivot.x && mousePoint.y < pivot.y) {
			selectedNum = 2;
		}
		else if (mousePoint.x < pivot.x && mousePoint.y > pivot.y) {
			selectedNum = 3;
		}
		else if (mousePoint.x > pivot.x && mousePoint.y > pivot.y) {
			selectedNum = 4;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_CHAR:
		if (wParam == 'a') {
			isStarted = true;
			hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
			InvalidateRect(hWnd, NULL, FALSE);
		}
		else if (wParam == 'r') {

		}
		else if (wParam == '+') {
			GetClientRect(hWnd, &rt);
			if (pivot.x >= rt.right || pivot.y >= rt.bottom)
				break;
			pivot.x += 10;
			pivot.y += 10;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		else if (wParam == '-') {
			GetClientRect(hWnd, &rt);
			if (pivot.x <= 0 || pivot.y <= 0)
				break;
			pivot.x -= 10;
			pivot.y -= 10;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		else if (wParam == 'p') {

		}
		else if (wParam == 's') {
			hBitmap = NULL;
			GetClientRect(hWnd, &rt);
			pivot.x = rt.right;
			pivot.y = rt.bottom;
			selectedNum = 0;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'q') {
			DeleteObject(hBitmap);
			PostQuitMessage(0);
		}
		break;

	case WM_DESTROY:
		DeleteObject(hBitmap);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}