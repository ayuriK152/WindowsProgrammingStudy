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
	HBRUSH hBrush, oldBrush;
	POINT triPoint[3];
	POINT rectPoint[4];
	POINT pentaPoint[5];

	int currentLeft;
	int currentRight;
	int currentTop;
	int currentBottom;
	static int vpWidth;
	static int vpHeight;
	static int shapes[4] = { 0 , 1 , 2 , 3 };
	static COLORREF baseColor[4] = { RGB(255, 0, 0), RGB(255, 255, 0), RGB(0, 255, 0), RGB(0, 0, 255) };
	static COLORREF randColor = NULL;
	static int currentShape;

	switch (uMsg) {
	case WM_CREATE:
		srand((unsigned int)time(NULL));
		currentShape = rand() % 4;
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rc);
		vpWidth = rc.right - rc.left;
		vpHeight = rc.bottom - rc.top;

		currentLeft = vpWidth / 24 * 9;
		currentRight = vpWidth / 24 * 15;
		currentTop = vpHeight / 24 * 9;
		currentBottom = vpHeight / 24 * 15;

		triPoint[0] = { currentLeft + (currentRight - currentLeft) / 2, currentTop };
		triPoint[1] = { currentRight, currentBottom };
		triPoint[2] = { currentLeft, currentBottom };

		rectPoint[0] = { currentLeft, currentTop };
		rectPoint[1] = { currentRight, currentTop };
		rectPoint[2] = { currentLeft, currentBottom };
		rectPoint[3] = { currentRight, currentBottom };

		pentaPoint[0] = { (currentLeft + currentRight) / 2, currentTop };
		pentaPoint[1] = { currentRight, currentTop + (currentBottom - currentTop) / 5 * 2 };
		pentaPoint[2] = { currentLeft + (currentRight - currentLeft) / 4 * 3, currentBottom };
		pentaPoint[3] = { currentLeft + (currentRight - currentLeft) / 4, currentBottom };
		pentaPoint[4] = { currentLeft, currentTop + (currentBottom - currentTop) / 5 * 2 };

		Rectangle(hDC, vpWidth / 3, vpHeight / 3, vpWidth / 3 * 2, vpHeight / 3 * 2);

		if (randColor != NULL)
			hBrush = CreateSolidBrush(randColor);
		else
			hBrush = CreateSolidBrush(baseColor[currentShape]);
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		switch (currentShape)
		{
		case 0:
			Polygon(hDC, rectPoint, 4);
			break;
		case 1:
			Polygon(hDC, pentaPoint, 5);
			break;
		case 2:
			Pie(hDC, currentLeft, currentTop, currentRight, currentBottom, currentLeft + (currentRight - currentLeft) / 2, currentTop, currentRight, currentTop + (currentBottom - currentTop) / 2);
			break;
		case 3:
			Polygon(hDC, triPoint, 3);
			break;
		}
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);

		for (int i = 0; i < 4; i++)
		{
			if (currentShape == shapes[i] && randColor != NULL)
				hBrush = CreateSolidBrush(randColor);
			else
				hBrush = CreateSolidBrush(baseColor[shapes[i]]);
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			switch (i)
			{
				case 0:
					currentLeft = vpWidth / 12 * 5;
					currentRight = vpWidth / 12 * 7;
					currentTop = vpHeight / 12;
					currentBottom = vpHeight / 12 * 3;
					break;
				case 1:
					currentLeft = vpWidth / 12 * 9;
					currentRight = vpWidth / 12 * 11;
					currentTop = vpHeight / 12 * 5;
					currentBottom = vpHeight / 12 * 7;
					break;
				case 2:
					currentLeft = vpWidth / 12 * 5;
					currentRight = vpWidth / 12 * 7;
					currentTop = vpHeight / 12 * 9;
					currentBottom = vpHeight / 12 * 11;
					break;
				case 3:
					currentLeft = vpWidth / 12;
					currentRight = vpWidth / 12 * 3;
					currentTop = vpHeight / 12 * 5;
					currentBottom = vpHeight / 12 * 7;
					break;
			}

			triPoint[0] = { currentLeft + (currentRight - currentLeft) / 2, currentTop };
			triPoint[1] = { currentRight, currentBottom };
			triPoint[2] = { currentLeft, currentBottom };

			rectPoint[0] = { currentLeft, currentTop };
			rectPoint[1] = { currentRight, currentTop };
			rectPoint[2] = { currentLeft, currentBottom };
			rectPoint[3] = { currentRight, currentBottom };

			pentaPoint[0] = { (currentLeft + currentRight) / 2, currentTop };
			pentaPoint[1] = { currentRight, currentTop + (currentBottom - currentTop) / 5 * 2 };
			pentaPoint[2] = { currentLeft + (currentRight - currentLeft) / 4 * 3, currentBottom };
			pentaPoint[3] = { currentLeft + (currentRight - currentLeft) / 4, currentBottom };
			pentaPoint[4] = { currentLeft, currentTop + (currentBottom - currentTop) / 5 * 2 };
			switch (shapes[i])
			{
				case 0:
					Polygon(hDC, rectPoint, 4);
					break;
				case 1:
					Polygon(hDC, pentaPoint, 5);
					break;
				case 2:
					Pie(hDC, currentLeft, currentTop, currentRight, currentBottom, currentLeft + (currentRight - currentLeft) / 2, currentTop, currentRight, currentTop + (currentBottom - currentTop) / 2);
					break;
				case 3:
					Polygon(hDC, triPoint, 3);
					break;
			}

			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
		}
		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN:
		srand((unsigned int)time(NULL));
		if (wParam == 's' || wParam == 'S')
		{
			currentShape = 0;
			randColor = RGB(rand() % 256, rand() % 256, rand() % 256);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'p' || wParam == 'P')
		{
			currentShape = 1;
			randColor = RGB(rand() % 256, rand() % 256, rand() % 256);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'e' || wParam == 'E')
		{
			currentShape = 2;
			randColor = RGB(rand() % 256, rand() % 256, rand() % 256);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 't' || wParam == 'T')
		{
			currentShape = 3;
			randColor = RGB(rand() % 256, rand() % 256, rand() % 256);
			InvalidateRect(hWnd, NULL, TRUE);
		}

		else if (wParam == VK_LEFT)
		{
			int temp = shapes[0];
			for (int i = 0; i < 3; i++)
			{
				shapes[i] = shapes[i + 1];
			}
			shapes[3] = temp;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == VK_RIGHT)
		{
			int temp = shapes[3];
			for (int i = 3; i > 0; i--)
			{
				shapes[i] = shapes[i - 1];
			}
			shapes[0] = temp;
			InvalidateRect(hWnd, NULL, TRUE);
		}

		else if (wParam == 'q' || wParam == 'Q')
		{
			PostQuitMessage(0);
			break;
		}
		break;

	case WM_KEYUP:
		randColor = NULL;
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
