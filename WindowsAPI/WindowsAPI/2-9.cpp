#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

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

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	RECT rc;
	static int vpWidth;
	static int vpHeight;
	static int shapes[4] = { 0 , 1 , 2 , 3 };

	switch (uMsg) {
	case WM_CREATE:
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rc);
		vpWidth = rc.right - rc.left;
		vpHeight = rc.bottom - rc.top;

		Rectangle(hDC, vpWidth / 3, vpHeight / 3, vpWidth / 3 * 2, vpHeight / 3 * 2);

		for (int i = 0; i < 4; i++)
		{
			int currentLeft;
			int currentRight;
			int currentTop;
			int currentBottom;
			switch (i)
			{
				case 0:
					currentLeft = vpWidth / 3;
					currentRight = vpWidth / 3 * 2;
					currentTop = 0;
					currentBottom = vpHeight / 3;
					break;
				case 1:
					currentLeft = vpWidth / 3 * 2;
					currentRight = vpWidth;
					currentTop = vpHeight / 3;
					currentBottom = vpHeight / 3 * 2;
					break;
				case 2:
					currentLeft = vpWidth / 3;
					currentRight = vpWidth / 3 * 2;
					currentTop = vpHeight / 3 * 2;
					currentBottom = vpHeight;
					break;
				case 3:
					currentLeft = 0;
					currentRight = vpWidth / 3;
					currentTop = vpHeight / 3;
					currentBottom = vpHeight / 3 * 2;
					break;
			}
			POINT rectPoint[4] = { {currentLeft + vpWidth / 12, currentTop + vpHeight / 12},
										{currentLeft + vpWidth / 12 * 3, currentTop + vpHeight / 12},
										{currentLeft + vpWidth / 12, currentTop + vpHeight / 12 * 3},
										{currentLeft + vpWidth / 12 * 3, currentTop + vpHeight / 12 * 3} };
			POINT pentaPoint[5] = { {(currentLeft + currentRight) / 2, currentTop},
										{currentLeft + vpWidth / 15, currentTop + vpHeight / 15 * 2},
										{currentLeft + vpWidth / 15 * 4, currentBottom},
										{currentLeft + vpWidth / 15 * 2, currentBottom},
										{currentLeft + vpWidth / 15, currentTop + vpHeight / 15 * 2} };
			switch (shapes[i])
			{
				case 0:
					Polygon(hDC, rectPoint, 4);
					break;
				case 1:
					Polygon(hDC, pentaPoint, 4);
					break;
				case 2:
					break;
				case 3:
					break;
			}
		}
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
