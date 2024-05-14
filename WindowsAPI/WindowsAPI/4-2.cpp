#include <windows.h> 
#include <cstdlib>
#include "resource.h"

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 860, 900, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

struct DrawRect {
	RECT rect;
	int Color1;
	int Color2;
	int Color3;
	int Pen;
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC mdc;
	HBITMAP hBitmap;
	PAINTSTRUCT ps;
	RECT rt;
	HDC hdc;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	static int startX, startY, oldX, oldY;
	int endX, endY;
	static BOOL Drag;
	static int mx, my;
	static BOOL DragRight;

	static struct DrawRect DR[5];
	static int RectNum;
	static int LineNum;

	switch (uMsg)
	{
	case WM_CREATE:
		RectNum = 0;
		LineNum = 1;
		break;

	case WM_CHAR:
		if (wParam == '1')
			RectNum = 0;
		else if (wParam == '2')
			RectNum = 1;
		else if (wParam == '3')
			RectNum = 2;
		else if (wParam == '4')
			RectNum = 3;
		else if (wParam == '5')
			RectNum = 4;

		for (int i = 0; i < 5; i++)
			DR[i].Pen = 0;
		DR[RectNum].Pen = 5;
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_GRID_DOTLINE:
		{
			LineNum = 1;
			break;
		}
		case ID_GRID_SOLIDLINE:
		{
			LineNum = 0;
			break;
		}
		case ID_COLOR_RED:
		{
			DR[RectNum].Color1 = 255;
			DR[RectNum].Color2 = 0;
			DR[RectNum].Color3 = 0;
			break;
		}
		case ID_COLOR_GREEN:
		{
			DR[RectNum].Color1 = 0;
			DR[RectNum].Color2 = 255;
			DR[RectNum].Color3 = 0;
			break;
		}
		case ID_COLOR_BLUE:
		{
			DR[RectNum].Color1 = 0;
			DR[RectNum].Color2 = 0;
			DR[RectNum].Color3 = 255;
			break;
		}
		case ID_COLOR_YELLOW:
		{
			DR[RectNum].Color1 = 255;
			DR[RectNum].Color2 = 255;
			DR[RectNum].Color3 = 0;
			break;
		}
		case ID_COLOR_MINT:
		{
			DR[RectNum].Color1 = 0;
			DR[RectNum].Color2 = 255;
			DR[RectNum].Color3 = 255;
			break;
		}
		case ID_COLOR_PINK:
		{
			DR[RectNum].Color1 = 255;
			DR[RectNum].Color2 = 0;
			DR[RectNum].Color3 = 255;
			break;
		}
		case ID_BORDER_ON:
		{
			for (int i = 0; i < 5; i++)
				DR[i].Pen = 3;
			break;
		}
		case ID_BORDER_OFF:
		{
			for (int i = 0; i < 5; i++)
				DR[i].Pen = 0;
			break;
		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;
		}

	case WM_KEYDOWN:
		if (wParam == VK_RIGHT)
		{
			if (DR[RectNum].rect.right < 800)
			{
				DR[RectNum].rect.left = DR[RectNum].rect.left + 20;
				DR[RectNum].rect.right = DR[RectNum].rect.right + 20;
			}
		}
		else if (wParam == VK_LEFT)
		{
			if (DR[RectNum].rect.left > 0)
			{
				DR[RectNum].rect.left = DR[RectNum].rect.left - 20;
				DR[RectNum].rect.right = DR[RectNum].rect.right - 20;
			}
		}
		else if (wParam == VK_UP)
		{
			if (DR[RectNum].rect.top > 0)
			{
				DR[RectNum].rect.top = DR[RectNum].rect.top - 20;
				DR[RectNum].rect.bottom = DR[RectNum].rect.bottom - 20;
			}
		}
		else if (wParam == VK_DOWN)
		{
			if (DR[RectNum].rect.bottom < 800)
			{
				DR[RectNum].rect.top = DR[RectNum].rect.top + 20;
				DR[RectNum].rect.bottom = DR[RectNum].rect.bottom + 20;
			}
		}

		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_LBUTTONDOWN:
		oldX = startX = LOWORD(lParam);
		oldY = startY = HIWORD(lParam);
		Drag = TRUE;
		break;

	case WM_LBUTTONUP:
		if (startX > oldX)
		{
			int temp = 0;
			oldX = temp;
			oldX = startX;
			startX = oldX;
		}
		if (startY > oldY)
		{
			int temp = 0;
			oldY = temp;
			oldY = startY;
			startY = oldY;
		}
		DR[RectNum].rect.left = startX / 40 * 40;
		DR[RectNum].rect.top = startY / 40 * 40;
		DR[RectNum].rect.right = oldX / 40 * 40;
		DR[RectNum].rect.bottom = oldY / 40 * 40;
		DR[RectNum].Color1 = 255;
		DR[RectNum].Color2 = 0;
		DR[RectNum].Color3 = 0;
		RectNum++;
		Drag = FALSE;
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_MOUSEMOVE:
		if (Drag)
		{
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			oldX = endX;
			oldY = endY;
		}

		if (DragRight)
		{
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			int w = DR[RectNum].rect.right - DR[RectNum].rect.left;
			int h = DR[RectNum].rect.bottom - DR[RectNum].rect.top;
			DR[RectNum].rect.left = mx / 40 * 40;
			DR[RectNum].rect.top = my / 40 * 40;
			DR[RectNum].rect.right = mx + w / 40 * 40;
			DR[RectNum].rect.bottom = my + h / 40 * 40;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;

	case WM_RBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (mx >= DR[0].rect.left && mx <= DR[0].rect.right && my >= DR[0].rect.top && my <= DR[0].rect.bottom)
			RectNum = 0;
		else if (mx >= DR[1].rect.left && mx <= DR[1].rect.right && my >= DR[1].rect.top && my <= DR[1].rect.bottom)
			RectNum = 1;
		else if (mx >= DR[2].rect.left && mx <= DR[2].rect.right && my >= DR[2].rect.top && my <= DR[2].rect.bottom)
			RectNum = 2;
		else if (mx >= DR[3].rect.left && mx <= DR[3].rect.right && my >= DR[3].rect.top && my <= DR[3].rect.bottom)
			RectNum = 3;
		else if (mx >= DR[4].rect.left && mx <= DR[4].rect.right && my >= DR[4].rect.top && my <= DR[4].rect.bottom)
			RectNum = 4;

		for (int i = 0; i < 5; i++)
			DR[i].Pen = 0;
		DR[RectNum].Pen = 5;
		DragRight = TRUE;
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_RBUTTONUP:
		DragRight = FALSE;
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		if (LineNum == 0)
		{
			for (int i = 0; i < 40; i++)
			{
				MoveToEx(hdc, i * 20, 0, NULL);
				LineTo(hdc, i * 20, 800);
			}
			for (int i = 0; i < 40; i++)
			{
				MoveToEx(hdc, 0, i * 20, NULL);
				LineTo(hdc, 800, i * 20);
			}
		}
		else if (LineNum == 1)
		{
			hPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			for (int i = 0; i < 40; i++)
			{
				MoveToEx(hdc, i * 20, 0, NULL);
				LineTo(hdc, i * 20, 800);
			}
			for (int i = 0; i < 40; i++)
			{
				MoveToEx(hdc, 0, i * 20, NULL);
				LineTo(hdc, 800, i * 20);
			}
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);
		}

		for (int i = 0; i < 5; i++)
		{
			hPen = CreatePen(PS_SOLID, DR[i].Pen, RGB(0, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			hBrush = CreateSolidBrush(RGB(DR[i].Color1, DR[i].Color2, DR[i].Color3));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, DR[i].rect.left, DR[i].rect.top, DR[i].rect.right, DR[i].rect.bottom);
			SelectObject(hdc, oldPen);
			SelectObject(hdc, oldBrush);
			DeleteObject(hPen);
			DeleteObject(hBrush);
		}

		for (int i = 1; i < 5; i++)
		{
			RECT rectsum;
			IntersectRect(&rectsum, &DR[0].rect, &DR[i].rect);
			hBrush = CreateSolidBrush(RGB((DR[0].Color1 + DR[i].Color1) / 2, (DR[0].Color2 + DR[i].Color2) / 2, (DR[0].Color3 + DR[i].Color3) / 2));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, rectsum.left, rectsum.top, rectsum.right, rectsum.bottom);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}

		for (int i = 2; i < 5; i++)
		{
			RECT rectsum;
			IntersectRect(&rectsum, &DR[1].rect, &DR[i].rect);
			hBrush = CreateSolidBrush(RGB((DR[1].Color1 + DR[i].Color1) / 2, (DR[1].Color2 + DR[i].Color2) / 2, (DR[1].Color3 + DR[i].Color3) / 2));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, rectsum.left, rectsum.top, rectsum.right, rectsum.bottom);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}

		for (int i = 3; i < 5; i++)
		{
			RECT rectsum;
			IntersectRect(&rectsum, &DR[2].rect, &DR[i].rect);
			hBrush = CreateSolidBrush(RGB((DR[2].Color1 + DR[i].Color1) / 2, (DR[2].Color2 + DR[i].Color2) / 2, (DR[2].Color3 + DR[i].Color3) / 2));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, rectsum.left, rectsum.top, rectsum.right, rectsum.bottom);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}

		for (int i = 4; i < 5; i++)
		{
			RECT rectsum;
			IntersectRect(&rectsum, &DR[3].rect, &DR[i].rect);
			hBrush = CreateSolidBrush(RGB((DR[3].Color1 + DR[i].Color1) / 2, (DR[3].Color2 + DR[i].Color2) / 2, (DR[3].Color3 + DR[i].Color3) / 2));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, rectsum.left, rectsum.top, rectsum.right, rectsum.bottom);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}

		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
