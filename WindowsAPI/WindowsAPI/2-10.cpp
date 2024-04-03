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

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 815, 840, NULL, (HMENU)NULL, hInstance, NULL);
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
	/* 플레이어 -	1
	*  골 -			2
	*  장애물 -		3, (255, 0, 0)
	*  색 변경 -		4, (0, 255, 0)
	*  축소 -		5, (255, 255, 0)
	*  확대 -		6, (0, 0, 255)
	*  모양 -		7, (255, 190, 0)
	*/
	PAINTSTRUCT ps;
	HDC hDC;
	HBRUSH hBrush, oldBrush;
	static int board[40][40];

	switch (uMsg) {
	case WM_CREATE:
		srand((unsigned int)time(NULL));
		for (int i = 0; i < 40; i++)
		{
			for (int j = 0; j < 40; j++)
			{
				board[i][j] = 0;
			}
		}
		for (int i = 0; i < 20; i++)
		{
			int x = rand() % 40;
			int y = rand() % 40;
			if (board[x][y] != 0)
			{
				i--;
				continue;
			}
			board[x][y] = 3 + (i / 4);
		}
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		for (int i = 0; i < 41; i++)
		{
			MoveToEx(hDC, 0, i * 20, NULL);
			LineTo(hDC, 800, i * 20);
		}
		for (int i = 0; i < 41; i++)
		{
			MoveToEx(hDC, i * 20, 0, NULL);
			LineTo(hDC, i * 20, 800);
		}
		for (int i = 0; i < 40; i++)
		{
			for (int j = 0; j < 40; j++)
			{
				hBrush = CreateSolidBrush(RGB(255, 255, 255));
				switch (board[i][j])
				{
					case 0:
						hBrush = CreateSolidBrush(RGB(255, 255, 255));
						break;
					case 1:
						break;
					case 2:
						break;
					case 3:
						hBrush = CreateSolidBrush(RGB(255, 0, 0));
						break;
					case 4:
						hBrush = CreateSolidBrush(RGB(0, 255, 0));
						break;
					case 5:
						hBrush = CreateSolidBrush(RGB(255, 255, 0));
						break;
					case 6:
						hBrush = CreateSolidBrush(RGB(0, 0, 255));
						break;
					case 7:
						hBrush = CreateSolidBrush(RGB(255, 190, 0));
						break;
				}
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Rectangle(hDC, j * 20, i * 20, (j + 1) * 20, (i + 1) * 20);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);
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
