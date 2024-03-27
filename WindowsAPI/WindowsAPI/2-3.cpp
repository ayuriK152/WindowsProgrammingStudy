#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>

void UpdateParams(int* x, int* y, int* n, int* count);

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
	TCHAR str[100];
	static int x;
	static int y;
	static int n;
	static int count;
	static int nums[20][20];

	switch (uMsg) {
	case WM_CREATE:
		UpdateParams(&x, &y, &n, &count);
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		srand((unsigned int)time(NULL));
		SetBkColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		SetTextColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		for (int i = 0; i < count; i++)
		{
			str[i] = n + 49;
			str[i + 1] = '\0';
		}
		for (int i = 0; i < count; i++)
		{
			TextOut(hDC, x, y + (i * 20), str, lstrlen(str));
		}
		EndPaint(hWnd, &ps);
		break;

	case WM_CHAR:
		if (wParam == VK_RETURN)
		{
			UpdateParams(&x, &y, &n, &count);
			InvalidateRect(hWnd, NULL, TRUE);
		}
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

void UpdateParams(int* x, int* y, int* n, int* count)
{
	srand((unsigned int)time(NULL));
	*x = (rand() % 61) * 10;
	*y = (rand() % 41) * 10;
	*n = rand() % 9;
	*count = (rand() % 16) + 5;
}
