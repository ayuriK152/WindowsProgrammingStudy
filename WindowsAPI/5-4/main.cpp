#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include "resource.h"

using namespace std;

void PlayerAnimation(int xPos, int yPos, HDC hDC);

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

HDC hDC, mDC;
RECT rt;
BITMAP bmp;
static HBITMAP playerBitmap, bgBitmap;
static int aniIndex;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	srand((unsigned int)time(NULL));
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;

	static POINT magStartPoint, magEndPoint;
	static int drawMode;
	static bool isDrag;
	static bool isMag;

	switch (uMsg) {
	case WM_CREATE:
		playerBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		bgBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		drawMode = SRCCOPY;

		isDrag = false;
		isMag = false;
		aniIndex = 0;

		SetTimer(hWnd, 1000, 100, NULL);
		break;

	case WM_PAINT: {
		GetClientRect(hWnd, &rt);
		hDC = BeginPaint(hWnd, &ps);

		PlayerAnimation(rt.right / 2, rt.bottom / 2, hDC);

		EndPaint(hWnd, &ps);
		break;
	}

	case WM_TIMER:
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_KEYDOWN:
		break;

	case WM_LBUTTONDOWN:
		break;

	case WM_LBUTTONUP: {
		break;
	}

	case WM_MOUSEMOVE:
		break;

	case WM_CHAR:

		break;

	case WM_DESTROY:
		DeleteObject(playerBitmap);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void PlayerAnimation(int xPos, int yPos, HDC hDC) {
	mDC = CreateCompatibleDC(hDC);

	SelectObject(mDC, bgBitmap);
	GetObject(bgBitmap, sizeof(BITMAP), &bmp);
	//BitBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, SRCCOPY);
	StretchBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	SelectObject(mDC, playerBitmap);
	GetObject(playerBitmap, sizeof(BITMAP), &bmp);
	TransparentBlt(hDC, rt.right / 2, rt.bottom / 2, 25 * 2, 25 * 2, mDC, 0 + (aniIndex * 25), 0, 25, 25, RGB(144, 187, 187));
	aniIndex = (aniIndex + 1) % 6;

	DeleteDC(mDC);
}