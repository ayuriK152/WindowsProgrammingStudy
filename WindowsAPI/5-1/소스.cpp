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
	static int paintMethod;
	static int privatePivot[4];

	switch (uMsg) {
	case WM_CREATE:
		GetClientRect(hWnd, &rt);
		pivot.x = rt.right;
		pivot.y = rt.bottom;
		selectedNum = 0;
		isStarted = false;
		paintMethod = SRCCOPY;
		privatePivot[0] = 0;
		privatePivot[1] = 0;
		privatePivot[2] = 0;
		privatePivot[3] = 0;
		hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		break;

	case WM_PAINT:
		GetClientRect(hWnd, &rt);
		hDC = BeginPaint(hWnd, &ps);
		mDC = CreateCompatibleDC(hDC);
		SelectObject(mDC, hBitmap);
		GetObject(hBitmap, sizeof(BITMAP), &bmp);

		if (isStarted) {
			StretchBlt(hDC, privatePivot[0], 0, pivot.x - privatePivot[0], pivot.y, mDC, 0, 0, ((pivot.x - privatePivot[0]) / (float)pivot.x * bmp.bmWidth), bmp.bmHeight, paintMethod);
			StretchBlt(hDC, 0, 0, privatePivot[0], pivot.y, mDC, (pivot.x - privatePivot[0]) / (float)pivot.x * bmp.bmWidth, 0, bmp.bmWidth - ((pivot.x - privatePivot[0]) / (float)pivot.x * bmp.bmWidth), bmp.bmHeight, paintMethod);

			StretchBlt(hDC, pivot.x + privatePivot[1], 0, rt.right - pivot.x - privatePivot[1], pivot.y, mDC, 0, 0, ((rt.right - pivot.x - privatePivot[1]) / (float)(rt.right - pivot.x) * bmp.bmWidth), bmp.bmHeight, paintMethod);
			StretchBlt(hDC, pivot.x, 0, privatePivot[1], pivot.y, mDC, (rt.right - pivot.x - privatePivot[1]) / (float)(rt.right - pivot.x) * bmp.bmWidth, 0, bmp.bmWidth - ((rt.right - pivot.x - privatePivot[1]) / (float)(rt.right - pivot.x) * bmp.bmWidth), bmp.bmHeight, paintMethod);

			StretchBlt(hDC, privatePivot[2], pivot.y, pivot.x - privatePivot[2], rt.bottom - pivot.y, mDC, 0, 0, ((pivot.x - privatePivot[2]) / (float)pivot.x * bmp.bmWidth), bmp.bmHeight, paintMethod);
			StretchBlt(hDC, 0, pivot.y, privatePivot[2], rt.bottom - pivot.y, mDC, (pivot.x - privatePivot[2]) / (float)pivot.x * bmp.bmWidth, 0, bmp.bmWidth - ((pivot.x - privatePivot[2]) / (float)pivot.x * bmp.bmWidth), bmp.bmHeight, paintMethod);

			StretchBlt(hDC, pivot.x + privatePivot[3], pivot.y, rt.right - pivot.x - privatePivot[3], rt.bottom - pivot.y, mDC, 0, 0, ((rt.right - pivot.x - privatePivot[3]) / (float)(rt.right - pivot.x) * bmp.bmWidth), bmp.bmHeight, paintMethod);
			StretchBlt(hDC, pivot.x, pivot.y, privatePivot[3], rt.bottom - pivot.y, mDC, (rt.right - pivot.x - privatePivot[3]) / (float)(rt.right - pivot.x) * bmp.bmWidth, 0, bmp.bmWidth - ((rt.right - pivot.x - privatePivot[3]) / (float)(rt.right - pivot.x) * bmp.bmWidth), bmp.bmHeight, paintMethod);

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
		}
		else {
			StretchBlt(hDC, 0, 0, 400, 300, mDC, 0, 0, bmp.bmWidth, bmp.bmHeight, paintMethod);
		}

		DeleteDC(mDC);
		EndPaint(hWnd, &ps);
		break;

	case WM_TIMER:
		hDC = GetDC(hWnd);

		ReleaseDC(hWnd, hDC);
		break;

	case WM_KEYDOWN:
		GetClientRect(hWnd, &rt);
		if (wParam == VK_LEFT) {
			switch (selectedNum) {
				case 1:
					privatePivot[0] -= 5;
					if (privatePivot[0] < 0) {
						privatePivot[0] += pivot.x;
					}
					break;
				case 2:
					privatePivot[1] -= 5;
					if (privatePivot[1] < 0) {
						privatePivot[1] += rt.right - pivot.x;
					}
					break;
				case 3:
					privatePivot[2] -= 5;
					if (privatePivot[2] < 0) {
						privatePivot[2] += pivot.x;
					}
					break;
				case 4:
					privatePivot[3] -= 5;
					if (privatePivot[3] < 0) {
						privatePivot[3] += rt.right - pivot.x;
					}
					break;
				case 5:
					privatePivot[0] -= 5;
					if (privatePivot[0] < 0) {
						privatePivot[0] += pivot.x;
					}
					privatePivot[1] -= 5;
					if (privatePivot[1] < 0) {
						privatePivot[1] += rt.right - pivot.x;
					}
					privatePivot[2] -= 5;
					if (privatePivot[2] < 0) {
						privatePivot[2] += pivot.x;
					}
					privatePivot[3] -= 5;
					if (privatePivot[3] < 0) {
						privatePivot[3] += rt.right - pivot.x;
					}
					break;
			}
			InvalidateRect(hWnd, NULL, FALSE);
		}
		else if (wParam == VK_RIGHT) {
			switch (selectedNum) {
				case 1:
					privatePivot[0] += 5;
					if (privatePivot[0] > pivot.x) {
						privatePivot[0] -= pivot.x;
					}
					break;
				case 2:
					privatePivot[1] += 5;
					if (privatePivot[1] > rt.right - pivot.x) {
						privatePivot[1] -= rt.right - pivot.x;
					}
					break;
				case 3:
					privatePivot[2] += 5;
					if (privatePivot[2] > pivot.x) {
						privatePivot[2] -= pivot.x;
					}
					break;
				case 4:
					privatePivot[3] += 5;
					if (privatePivot[3] > rt.right - pivot.x) {
						privatePivot[3] -= rt.right - pivot.x;
					}
					break;
				case 5:
					privatePivot[0] += 5;
					if (privatePivot[0] > pivot.x) {
						privatePivot[0] -= pivot.x;
					}
					privatePivot[1] += 5;
					if (privatePivot[1] > rt.right - pivot.x) {
						privatePivot[1] -= rt.right - pivot.x;
					}
					privatePivot[2] += 5;
					if (privatePivot[2] > pivot.x) {
						privatePivot[2] -= pivot.x;
					}
					privatePivot[3] += 5;
					if (privatePivot[3] > rt.right - pivot.x) {
						privatePivot[3] -= rt.right - pivot.x;
					}
					break;
			}
			InvalidateRect(hWnd, NULL, FALSE);
		}
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
			InvalidateRect(hWnd, NULL, FALSE);
		}
		else if (wParam == 'r') {
			if (paintMethod == SRCCOPY) {
				paintMethod = NOTSRCCOPY;
			}
			else if (paintMethod == NOTSRCCOPY) {
				paintMethod = SRCCOPY;
			}
			InvalidateRect(hWnd, NULL, FALSE);
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
			selectedNum = 5;
		}
		else if (wParam == 's') {
			isStarted = false;
			GetClientRect(hWnd, &rt);
			pivot.x = rt.right;
			pivot.y = rt.bottom;
			selectedNum = 0;
			paintMethod = SRCCOPY;
			privatePivot[0] = 0;
			privatePivot[1] = 0;
			privatePivot[2] = 0;
			privatePivot[3] = 0;
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
