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
	static POINT magStartPoint, magEndPoint;
	static int drawMode;
	static bool isDrag;
	static bool isMag;

	switch (uMsg) {
	case WM_CREATE:
		hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		drawMode = SRCCOPY;

		isDrag = false;
		isMag = false;
		break;

	case WM_PAINT: {
		GetClientRect(hWnd, &rt);
		hDC = BeginPaint(hWnd, &ps);
		mDC = CreateCompatibleDC(hDC);
		SelectObject(mDC, hBitmap);
		GetObject(hBitmap, sizeof(BITMAP), &bmp);

		StretchBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, bmp.bmWidth, bmp.bmHeight, drawMode);

		if (isMag) {
			hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(hDC, hPen);
			hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			Rectangle(hDC, magStartPoint.x, magStartPoint.y, magEndPoint.x, magEndPoint.y);

			SelectObject(hDC, oldPen);
			DeleteObject(hPen);
			SelectObject(hDC, oldBrush);
		}

		DeleteDC(mDC);
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_TIMER:
		hDC = GetDC(hWnd);

		ReleaseDC(hWnd, hDC);
		break;

	case WM_KEYDOWN:
		GetClientRect(hWnd, &rt);

		break;

	case WM_LBUTTONDOWN:
		if (isMag) {
			break;
		}
		else if (!isMag) {
			magStartPoint.x = LOWORD(lParam);
			magStartPoint.y = HIWORD(lParam);
			magEndPoint.x = LOWORD(lParam);
			magEndPoint.y = HIWORD(lParam);
			isMag = true;
		}
		isDrag = true;

		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_LBUTTONUP: {
		isDrag = false;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}

	case WM_MOUSEMOVE:
		if (isDrag) {
			magEndPoint.x = LOWORD(lParam);
			magEndPoint.y = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;

	case WM_CHAR:
		if (wParam == '1') {
			hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
			InvalidateRect(hWnd, NULL, FALSE);
		}
		else if (wParam == '2') {
			hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
			InvalidateRect(hWnd, NULL, FALSE);
		}

		else if (wParam == 'e') {

		}
		else if (wParam == 's') {

		}
		else if (wParam == '0') {

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

void CALLBACK MovePiece(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {

	InvalidateRect(hWnd, NULL, FALSE);
}