#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>

void UpdateParam(int* x, int* y, int* n, int* m, TCHAR str[]);

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
	static TCHAR str[100];
	static int strCount;
	static int x;
	static int y;
	static int n;
	static int m;

	switch (uMsg) {
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		for (int i = 0; i < m; i++)
		{
			TCHAR clacStr[100];
			wsprintf(clacStr, L"%d * %d = %d", n, i + 1, n * (i + 1));
			TextOut(hDC, x, y + (i * 20), clacStr, lstrlen(clacStr));
		}
		GetTextExtentPoint32(hDC, str, lstrlen(str), &size);
		TextOut(hDC, 0, 0, str, lstrlen(str));
		SetCaretPos(size.cx, 0);
		EndPaint(hWnd, &ps);
		break;

	case WM_CHAR:
		hDC = GetDC(hWnd);
		if (wParam == VK_RETURN)
		{
			UpdateParam(&x, &y, &n, &m, str);
			wsprintf(str, L"");
			strCount = 0;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		else if (wParam == VK_BACK)
		{
			strCount--;
			str[strCount] = '\0';
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		else if (wParam == 'q')
		{
			PostQuitMessage(0);
			break;
		}
		str[strCount++] = wParam;
		str[strCount] = '\0';
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_DESTROY:
		HideCaret(hWnd);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void UpdateParam(int* x, int* y, int* n, int* m, TCHAR str[])
{
	int parseCnt = 0;
	int temp = 0;
	for (int i = 0; ; i++)
	{
		if (str[i] == '\0') {
			*m = temp;
			break;
		}
		if (str[i] == ' ')
		{
			switch (parseCnt)
			{
				case 0:
					*x = temp;
					break;
				case 1:
					*y = temp;
					break;
				case 2:
					*n = temp;
					break;
			}
			temp = 0;
			parseCnt++;
		}
		else
		{
			temp *= 10;
			temp += str[i] - 48;
		}
	}
}
