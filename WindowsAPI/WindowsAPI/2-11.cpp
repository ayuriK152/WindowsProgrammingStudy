#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>

void SetShapeInfo(TCHAR str[], int* left, int* right, int* top, int* bottom, int* shape, int* thickness);

struct ShapeInfo {
	int left, top, right, bottom;
	int thickness;
	int shape;
	COLORREF color;
	COLORREF lineColor;
};

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
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;
	RECT rect;
	static SIZE size;
	static TCHAR str[100];
	static COLORREF color;
	static COLORREF lineColor;
	static ShapeInfo shapeInfos[2];
	static POINT triPoints[3];
	static POINT pentaPoints[5];
	static int left, right, top, bottom;
	static int shape = 0;
	static int thickness;
	static int strCount;
	static bool isError = false;
	static bool isAll = false;

	switch (uMsg) {
	case WM_CREATE:
		strCount = 0;
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);

		if (shape != 0)
		{
			hPen = CreatePen(PS_SOLID, shapeInfos[0].thickness, shapeInfos[0].lineColor);
			oldPen = (HPEN)SelectObject(hDC, hPen);
			hBrush = CreateSolidBrush(shapeInfos[0].color);
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			triPoints[0] = { shapeInfos[0].left + (shapeInfos[0].right - shapeInfos[0].left) / 2, shapeInfos[0].top };
			triPoints[1] = { shapeInfos[0].right, shapeInfos[0].bottom };
			triPoints[2] = { shapeInfos[0].left, shapeInfos[0].bottom };

			pentaPoints[0] = { shapeInfos[0].left + (shapeInfos[0].right - shapeInfos[0].left) / 2, shapeInfos[0].top };
			pentaPoints[1] = { shapeInfos[0].right, shapeInfos[0].top + (shapeInfos[0].bottom - shapeInfos[0].top) / 5 * 2 };
			pentaPoints[2] = { shapeInfos[0].left + (shapeInfos[0].right - shapeInfos[0].left) / 5 * 4, shapeInfos[0].bottom };
			pentaPoints[3] = { shapeInfos[0].left + (shapeInfos[0].right - shapeInfos[0].left) / 5 * 1, shapeInfos[0].bottom };
			pentaPoints[4] = { shapeInfos[0].left, shapeInfos[0].top + (shapeInfos[0].bottom - shapeInfos[0].top) / 5 * 2 };

			if (isAll)
			{
				Rectangle(hDC, shapeInfos[0].left, shapeInfos[0].top, shapeInfos[0].right, shapeInfos[0].bottom);
				Polygon(hDC, pentaPoints, 5);
				Polygon(hDC, triPoints, 3);
				MoveToEx(hDC, shapeInfos[0].left, shapeInfos[0].top, NULL);
				LineTo(hDC, shapeInfos[0].right, shapeInfos[0].bottom);
				Ellipse(hDC, shapeInfos[0].left, shapeInfos[0].top, shapeInfos[0].left + 2, shapeInfos[0].top + 2);
				isAll = false;
			}
			else
			{
				switch (shapeInfos[0].shape)
				{
				case 1:
					Ellipse(hDC, shapeInfos[0].left, shapeInfos[0].top, shapeInfos[0].left + 2, shapeInfos[0].top + 2);
					break;
				case 2:
					MoveToEx(hDC, shapeInfos[0].left, shapeInfos[0].top, NULL);
					LineTo(hDC, shapeInfos[0].right, shapeInfos[0].bottom);
					break;
				case 3:
					Polygon(hDC, triPoints, 3);
					break;
				case 4:
					Rectangle(hDC, shapeInfos[0].left, shapeInfos[0].top, shapeInfos[0].right, shapeInfos[0].bottom);
					break;
				case 5:
					Polygon(hDC, pentaPoints, 5);
					break;
				case 6:
					Ellipse(hDC, shapeInfos[0].left, shapeInfos[0].top, shapeInfos[0].right, shapeInfos[0].bottom);
					break;
				}
			}

			SelectObject(hDC, oldPen);
			SelectObject(hDC, oldBrush);
			DeleteObject(hPen);
			DeleteObject(hBrush);
		}

		hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Rectangle(hDC, 0, rect.bottom - 17, rect.right, rect.bottom);
		SelectObject(hDC, oldBrush);

		GetTextExtentPoint32(hDC, str, lstrlen(str), &size);
		TextOut(hDC, 1, rect.bottom - 16, str, lstrlen(str));
		SetCaretPos(size.cx, rect.bottom - 16);

		if (isError)
		{
			TextOut(hDC, 10, 10, L"ERROR : No Shape Exists!!", lstrlen(L"ERROR : No Shape Exists!!"));
		}

		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_UP)
		{
			shapeInfos[0].top -= 5;
			shapeInfos[0].bottom -= 5;
		}
		else if (wParam == VK_DOWN)
		{
			shapeInfos[0].top += 5;
			shapeInfos[0].bottom += 5;
		}
		else if (wParam == VK_LEFT)
		{
			shapeInfos[0].left -= 5;
			shapeInfos[0].right -= 5;
		}
		else if (wParam == VK_RIGHT)
		{
			shapeInfos[0].left += 5;
			shapeInfos[0].right += 5;
		}

		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_CHAR:
		if (wParam == VK_BACK)
		{
			if (strCount == 0)
				break;
			str[--strCount] = '\0';
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		else if (wParam == VK_RETURN)
		{
			isError = false;

			bool flag;
			flag = shape == 0;
			SetShapeInfo(str, &left, &right, &top, &bottom, &shape, &thickness);
			srand((unsigned int)time(NULL));
			color = RGB(rand() % 256, rand() % 256, rand() % 256);
			lineColor = RGB(rand() % 256, rand() % 256, rand() % 256);
			if (flag)
			{
				shapeInfos[1] = { left, top, right, bottom, thickness, shape, color, lineColor };
			}
			else
			{
				shapeInfos[1] = shapeInfos[0];
			}
			shapeInfos[0] = { left, top, right, bottom, thickness, shape, color, lineColor };
			strCount = 0;
			wsprintf(str, L"");
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		else if (wParam == '+')
		{
			if (shape == 0)
			{
				isError = true;
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}

			if (shapeInfos[0].thickness < 10 || shapeInfos[0].shape == 1)
				shapeInfos[0].thickness++;
			else
			{
				shapeInfos[0].left += 2;
				shapeInfos[0].top += 2;
				shapeInfos[0].right += 2;
				shapeInfos[0].bottom += 2;
			}

			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		else if (wParam == '-')
		{
			if (shape == 0)
			{
				isError = true;
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}

			if (shapeInfos[0].thickness > 1)
				shapeInfos[0].thickness--;
			else if (shapeInfos[0].shape != 1)
			{
				if (shapeInfos[0].right - shapeInfos[0].left <= 4 && shapeInfos[0].bottom - shapeInfos[0].top <= 4)
					break;

				shapeInfos[0].left -= 2;
				shapeInfos[0].top -= 2;
				shapeInfos[0].right -= 2;
				shapeInfos[0].bottom -= 2;
			}

			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		else if (wParam == 'i')
		{
			if (shape == 0)
			{
				isError = true;
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}

			srand((unsigned int)time(NULL));
			shapeInfos[0].color = RGB(rand() % 256, rand() % 256, rand() % 256);

			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		else if (wParam == 'o')
		{
			if (shape == 0)
			{
				isError = true;
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}

			srand((unsigned int)time(NULL));
			shapeInfos[0].lineColor = RGB(rand() % 256, rand() % 256, rand() % 256);

			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		else if (wParam == 'p')
		{
			if (shape == 0)
			{
				isError = true;
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}

			ShapeInfo tempShapeInfo;
			tempShapeInfo = shapeInfos[1];
			shapeInfos[1] = shapeInfos[0];
			shapeInfos[0] = tempShapeInfo;

			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		else if (wParam == 'a')
		{
			if (shape == 0)
			{
				isError = true;
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}

			isAll = true;

			InvalidateRect(hWnd, NULL, TRUE);
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

void SetShapeInfo(TCHAR str[], int* left, int* right, int* top, int* bottom, int* shape, int* thickness)
{
	int param[6] = { 0, 0, 0, 0, 0, 0 }, paramCnt = 0;
	for (int i = 0; i < lstrlen(str); i++)
	{
		if (str[i] == ' ')
		{
			paramCnt++;
		}
		else
		{
			param[paramCnt] *= 10;
			param[paramCnt] += str[i] - 48;
		}
	}

	*shape = param[0];
	*left = param[1];
	*right = param[3];
	*top = param[2];
	*bottom = param[4];
	*thickness = param[5];
}