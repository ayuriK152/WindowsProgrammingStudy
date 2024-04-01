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
	static SIZE size;
	static TCHAR shapes[6][7];
	static int n;
	static int m;

	switch (uMsg) {
	case WM_CREATE:
		srand((unsigned int)time(NULL));
		n = (rand() % 9) + 2;
		m = (rand() % 9) + 2;
		wsprintf(shapes[0], L"X");
		wsprintf(shapes[1], L"¤©");
		wsprintf(shapes[2], L"¢Â");
		wsprintf(shapes[3], L"¢¹¢·");
		wsprintf(shapes[4], L"¡ã¡ã");
		wsprintf(shapes[5], L"|||");
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		TCHAR nLineStr[500];
		for (int i = 0; ; i++)
		{
			nLineStr[i] = '-';
			nLineStr[i + 1] = '\0';
			GetTextExtentPoint32(hDC, nLineStr, lstrlen(nLineStr), &size);
			if (size.cx >= 800)
				break;
		}
		for (int i = 1; i < n; i++)
		{
			TextOut(hDC, 0, (600 / n) * i, nLineStr, lstrlen(nLineStr));
		}
		for (int i = 1; i < m; i++)
		{
			for (int j = 0; (j + 1) * 15 <= 600; j++)
			{
				TextOut(hDC, (800 / m) * i, j * 15, L"|", 1);
			}
		}
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				int randNum = rand() % 6;
				switch (randNum)
				{
				case 0:
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 1, (600 / n) * j + (600 / n) / 6 * 1, L"X", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 5, (600 / n) * j + (600 / n) / 6 * 1, L"X", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 2, (600 / n) * j + (600 / n) / 6 * 2, L"X", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 4, (600 / n) * j + (600 / n) / 6 * 2, L"X", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 3, (600 / n) * j + (600 / n) / 6 * 3, L"X", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 2, (600 / n) * j + (600 / n) / 6 * 4, L"X", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 4, (600 / n) * j + (600 / n) / 6 * 4, L"X", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 1, (600 / n) * j + (600 / n) / 6 * 5, L"X", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 5, (600 / n) * j + (600 / n) / 6 * 5, L"X", 1);
					break;
				case 1:
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 1, (600 / n) * j + (600 / n) / 6 * 1, L"¤©", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 2, (600 / n) * j + (600 / n) / 6 * 1, L"¤©", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 3, (600 / n) * j + (600 / n) / 6 * 1, L"¤©", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 4, (600 / n) * j + (600 / n) / 6 * 1, L"¤©", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 5, (600 / n) * j + (600 / n) / 6 * 1, L"¤©", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 5, (600 / n) * j + (600 / n) / 6 * 2, L"¤©", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 1, (600 / n) * j + (600 / n) / 6 * 3, L"¤©", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 2, (600 / n) * j + (600 / n) / 6 * 3, L"¤©", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 3, (600 / n) * j + (600 / n) / 6 * 3, L"¤©", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 4, (600 / n) * j + (600 / n) / 6 * 3, L"¤©", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 5, (600 / n) * j + (600 / n) / 6 * 3, L"¤©", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 1, (600 / n) * j + (600 / n) / 6 * 4, L"¤©", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 1, (600 / n) * j + (600 / n) / 6 * 5, L"¤©", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 2, (600 / n) * j + (600 / n) / 6 * 5, L"¤©", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 3, (600 / n) * j + (600 / n) / 6 * 5, L"¤©", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 4, (600 / n) * j + (600 / n) / 6 * 5, L"¤©", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 5, (600 / n) * j + (600 / n) / 6 * 5, L"¤©", 1);
					break;
				case 2:
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 3, (600 / n) * j + (600 / n) / 6 * 1, L"¢Â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 2, (600 / n) * j + (600 / n) / 6 * 2, L"¢Â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 4, (600 / n) * j + (600 / n) / 6 * 2, L"¢Â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 1, (600 / n) * j + (600 / n) / 6 * 3, L"¢Â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 5, (600 / n) * j + (600 / n) / 6 * 3, L"¢Â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 2, (600 / n) * j + (600 / n) / 6 * 4, L"¢Â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 4, (600 / n) * j + (600 / n) / 6 * 4, L"¢Â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 3, (600 / n) * j + (600 / n) / 6 * 5, L"¢Â", 1);
					break;
				case 3:
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 1, (600 / n) * j + (600 / n) / 6 * 1, L"¡â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 5, (600 / n) * j + (600 / n) / 6 * 1, L"¡â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 1, (600 / n) * j + (600 / n) / 6 * 2, L"¡â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 2, (600 / n) * j + (600 / n) / 6 * 2, L"¡â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 4, (600 / n) * j + (600 / n) / 6 * 2, L"¡â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 5, (600 / n) * j + (600 / n) / 6 * 2, L"¡â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 1, (600 / n) * j + (600 / n) / 6 * 3, L"¡â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 2, (600 / n) * j + (600 / n) / 6 * 3, L"¡â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 3, (600 / n) * j + (600 / n) / 6 * 3, L"¡â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 4, (600 / n) * j + (600 / n) / 6 * 3, L"¡â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 5, (600 / n) * j + (600 / n) / 6 * 3, L"¡â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 1, (600 / n) * j + (600 / n) / 6 * 4, L"¡â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 2, (600 / n) * j + (600 / n) / 6 * 4, L"¡â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 4, (600 / n) * j + (600 / n) / 6 * 4, L"¡â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 5, (600 / n) * j + (600 / n) / 6 * 4, L"¡â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 1, (600 / n) * j + (600 / n) / 6 * 5, L"¡â", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 5, (600 / n) * j + (600 / n) / 6 * 5, L"¡â", 1);
					break;
				case 4:
					TextOut(hDC, (800 / m) * i + (800 / m) / 8 * 4, (600 / n) * j + (600 / n) / 8 * 1, L"¡ã", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 8 * 3, (600 / n) * j + (600 / n) / 8 * 2, L"¡ã", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 8 * 4, (600 / n) * j + (600 / n) / 8 * 2, L"¡ã", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 8 * 5, (600 / n) * j + (600 / n) / 8 * 2, L"¡ã", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 8 * 2, (600 / n) * j + (600 / n) / 8 * 3, L"¡ã", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 8 * 3, (600 / n) * j + (600 / n) / 8 * 3, L"¡ã", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 8 * 4, (600 / n) * j + (600 / n) / 8 * 3, L"¡ã", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 8 * 5, (600 / n) * j + (600 / n) / 8 * 3, L"¡ã", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 8 * 6, (600 / n) * j + (600 / n) / 8 * 3, L"¡ã", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 8 * 4, (600 / n) * j + (600 / n) / 8 * 5, L"¡ã", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 8 * 3, (600 / n) * j + (600 / n) / 8 * 6, L"¡ã", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 8 * 4, (600 / n) * j + (600 / n) / 8 * 6, L"¡ã", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 8 * 5, (600 / n) * j + (600 / n) / 8 * 6, L"¡ã", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 8 * 2, (600 / n) * j + (600 / n) / 8 * 7, L"¡ã", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 8 * 3, (600 / n) * j + (600 / n) / 8 * 7, L"¡ã", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 8 * 4, (600 / n) * j + (600 / n) / 8 * 7, L"¡ã", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 8 * 5, (600 / n) * j + (600 / n) / 8 * 7, L"¡ã", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 8 * 6, (600 / n) * j + (600 / n) / 8 * 7, L"¡ã", 1);
					break;
				case 5:
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 1, (600 / n) * j + (600 / n) / 6 * 1, L"¡á", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 3, (600 / n) * j + (600 / n) / 6 * 1, L"¡á", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 5, (600 / n) * j + (600 / n) / 6 * 1, L"¡á", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 1, (600 / n) * j + (600 / n) / 6 * 2, L"¡á", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 3, (600 / n) * j + (600 / n) / 6 * 2, L"¡á", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 5, (600 / n) * j + (600 / n) / 6 * 2, L"¡á", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 1, (600 / n) * j + (600 / n) / 6 * 3, L"¡á", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 3, (600 / n) * j + (600 / n) / 6 * 3, L"¡á", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 5, (600 / n) * j + (600 / n) / 6 * 3, L"¡á", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 1, (600 / n) * j + (600 / n) / 6 * 4, L"¡á", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 3, (600 / n) * j + (600 / n) / 6 * 4, L"¡á", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 5, (600 / n) * j + (600 / n) / 6 * 4, L"¡á", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 1, (600 / n) * j + (600 / n) / 6 * 5, L"¡á", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 3, (600 / n) * j + (600 / n) / 6 * 5, L"¡á", 1);
					TextOut(hDC, (800 / m) * i + (800 / m) / 6 * 5, (600 / n) * j + (600 / n) / 6 * 5, L"¡á", 1);
					break;
				}
				//TextOut(hDC, (800 / m) * i + (800 / m) / 2, (600 / n) * j + (600 / n) / 2, shapes[randNum], lstrlen(shapes[randNum]));
			}
		}
		EndPaint(hWnd, &ps);
		break;

	case WM_CHAR:
		hDC = GetDC(hWnd);
		if (wParam == 'q')
		{
			PostQuitMessage(0);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
