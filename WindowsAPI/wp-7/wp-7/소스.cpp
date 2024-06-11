#include <windows.h> 
#include <TCHAR.H>
#include <cstdlib>
#include <ctime>
#include <math.h>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
	WndClass.lpszClassName = L"ParentClass";
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	WndClass.hCursor = LoadCursor(NULL, IDC_HELP);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	WndClass.lpszClassName = L"ChildClass";
	WndClass.lpfnWndProc = (WNDPROC)ChildProc;
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(L"ParentClass", NULL, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hInstance, NULL);

	//hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 840, 840, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}



	return Message.wParam;
}

struct Circle {
	double x;
	double y;
	double d;
	int size;
};

#define ID_R1 100
#define ID_R2 101
#define ID_R3 102
#define ID_R4 103
#define ID_R5 104
#define ID_R6 105
#define ID_R7 106
#define ID_R8 107
#define ID_R9 108

#define IDC_BUTTON1 200
#define IDC_BUTTON2 201
#define IDC_BUTTON3 202
#define IDC_BUTTON4 203
#define IDC_BUTTON5 204

#define IDC_EDIT 300

int Color[3];
struct Circle MainCircle;
int Draw;
int Move;
double dx = 0;
double dy = 0;
#define PI 3.1415926
int c = 0;
TCHAR chat[10];

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	srand((unsigned int)time(NULL));
	HDC mdc;
	HBITMAP hBitmap;
	RECT rt;
	PAINTSTRUCT ps;
	HDC hdc;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	HWND hDlg = NULL;
	HWND child_hWnd;
	HWND b1, b2, b3, b4, b5 ;
	HWND r1, r2, r3, r4, r5, r6, r7, r8, r9, e1;


	switch (uMsg) {
	case WM_CREATE:
		child_hWnd = CreateWindow(L"ChildClass", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_THICKFRAME, 10, 10, 510, 510, hWnd, NULL, g_hInst, NULL);
		
		CreateWindow(L"button", L"Shape", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 600, 50, 150, 100, hWnd, (HMENU)0, g_hInst, NULL);
		r1 = CreateWindow(L"button", L"Circle", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,620, 70, 100, 20, hWnd, (HMENU)ID_R1, g_hInst, NULL);
		r2 = CreateWindow(L"button", L"Rectangle", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 620, 90, 100, 20, hWnd, (HMENU)ID_R2, g_hInst, NULL);
		r3 = CreateWindow(L"button", L"Triangle", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 620, 110, 100, 20, hWnd, (HMENU)ID_R3, g_hInst, NULL);
		CheckRadioButton(hWnd, ID_R1, ID_R2, ID_R3);
		
		CreateWindow(L"button", L"Size", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 600, 150, 150, 100, hWnd, (HMENU)0, g_hInst, NULL);
		r4 = CreateWindow(L"button", L"S", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 620, 170, 100, 20, hWnd, (HMENU)ID_R4, g_hInst, NULL);
		r5 = CreateWindow(L"button", L"M", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 620, 190, 100, 20, hWnd, (HMENU)ID_R5, g_hInst, NULL);
		r6 = CreateWindow(L"button", L"L", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 620, 210, 100, 20, hWnd, (HMENU)ID_R6, g_hInst, NULL);
		CheckRadioButton(hWnd, ID_R4, ID_R5, ID_R6);
		
		CreateWindow(L"button", L"Color", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 600, 250, 150, 100, hWnd, (HMENU)0, g_hInst, NULL);
		r7 = CreateWindow(L"button", L"R", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 620, 270, 100, 20, hWnd, (HMENU)ID_R7, g_hInst, NULL);
		r8 = CreateWindow(L"button", L"G", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 620, 290, 100, 20, hWnd, (HMENU)ID_R8, g_hInst, NULL);
		r9 = CreateWindow(L"button", L"B", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 620, 310, 100, 20, hWnd, (HMENU)ID_R9, g_hInst, NULL);
		CheckRadioButton(hWnd, ID_R7, ID_R8, ID_R9);
		
		b1 = CreateWindow(L"button", L"Move",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 600, 350, 50, 30, hWnd, (HMENU)IDC_BUTTON1, g_hInst, NULL);
		b2 = CreateWindow(L"button", L"Stop", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 670, 350, 50, 30, hWnd, (HMENU)IDC_BUTTON2, g_hInst, NULL);
		b3 = CreateWindow(L"button", L"+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 600, 400, 50, 30, hWnd, (HMENU)IDC_BUTTON3, g_hInst, NULL);
		b4 = CreateWindow(L"button", L"-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 670, 400, 50, 30, hWnd, (HMENU)IDC_BUTTON4, g_hInst, NULL);
		b5 = CreateWindow(L"button", L"List", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 600, 450, 50, 30, hWnd, (HMENU)IDC_BUTTON5, g_hInst, NULL);
		
		e1 = CreateWindow(L"edit", L"editing", WS_CHILD | WS_VISIBLE | WS_BORDER, 600, 500, 100, 50, hWnd, (HMENU)IDC_EDIT, g_hInst, NULL);

		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) 
		{
		case ID_R1:
			Draw = 1;
			break;
		case ID_R2:
			MainCircle.x = 110.0;
			MainCircle.y = 110.0;
			Draw = 2;
			break;
		case ID_R3:
			MainCircle.x = 260.0;
			MainCircle.y = 110.0;
			Draw = 3;
			break;
		case ID_R4:
			MainCircle.size = 10;
			break;
		case ID_R5:
			MainCircle.size = 20;
			break;
		case ID_R6:
			MainCircle.size = 30;
			break;
		case ID_R7:
			Color[0] = 255;
			Color[1] = 0;
			Color[2] = 0;
			break;
		case ID_R8:
			Color[0] = 0;
			Color[1] = 255;
			Color[2] = 0;
			break;
		case ID_R9:
			Color[0] = 0;
			Color[1] = 0;
			Color[2] = 255;
			break;

		case IDC_BUTTON1:
			Move = 1 - Move;
			MainCircle.d = 1;
			break;
		case IDC_BUTTON2:
			if(MainCircle.d != 0)
				MainCircle.d = 0;
			else
				MainCircle.d = 1;
			break;
		case IDC_BUTTON3:
			MainCircle.d = 2;
			break;
		case IDC_BUTTON4:
			MainCircle.d = 1;
			break;

		case IDC_BUTTON5:
			c = 1 - c;
			break;

		case IDC_EDIT:
			GetDlgItemText(hWnd, IDC_EDIT, chat, 50);
			SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)chat);
			break;

		}
		break;

	case WM_PAINT:
		GetClientRect(hWnd, &rt);
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK ChildProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC mdc;
	HBITMAP hBitmap;
	RECT rt;
	PAINTSTRUCT ps;
	HDC hdc;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	static int x = 0;
	

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hwnd, 1, 100, NULL);
		SetTimer(hwnd, 2, 50, NULL);
		break;

	case WM_PAINT:
		GetClientRect(hwnd, &rt);
		hdc = BeginPaint(hwnd, &ps);
		mdc = CreateCompatibleDC(hdc); //--- 메모리 DC 만들기
		hBitmap = CreateCompatibleBitmap(hdc, rt.right, rt.bottom); //--- 메모리 DC와 연결할 비트맵 만들기
		SelectObject(mdc, (HBITMAP)hBitmap);
		Rectangle(mdc, 0, 0, rt.right, rt.bottom);

		MoveToEx(mdc, 10, 260, NULL);
		LineTo(mdc, 510, 260);
		MoveToEx(mdc, 260, 10, NULL);
		LineTo(mdc, 260, 510);

		if (Draw == 1)
		{
			Ellipse(mdc, 110, 110, 410, 410);
			MoveToEx(mdc, 10, 260, NULL);
			LineTo(mdc, 510, 260);
			MoveToEx(mdc, 260, 10, NULL);
			LineTo(mdc, 260, 510);
			hBrush = CreateSolidBrush(RGB(Color[0], Color[1], Color[2]));
			oldBrush = (HBRUSH)SelectObject(mdc, hBrush);
			Ellipse(mdc, MainCircle.x - MainCircle.size, MainCircle.y - MainCircle.size, MainCircle.x + MainCircle.size, MainCircle.y + MainCircle.size);
			SelectObject(mdc, oldBrush);
			DeleteObject(hBrush);
		}

		else if (Draw == 2)
		{
			MoveToEx(mdc, 110, 110, NULL);
			LineTo(mdc, 110, 410);
			LineTo(mdc, 410, 410);
			LineTo(mdc, 410, 110);
			LineTo(mdc, 110, 110);
			hBrush = CreateSolidBrush(RGB(Color[0], Color[1], Color[2]));
			oldBrush = (HBRUSH)SelectObject(mdc, hBrush);
			Ellipse(mdc, MainCircle.x - MainCircle.size, MainCircle.y - MainCircle.size, MainCircle.x + MainCircle.size, MainCircle.y + MainCircle.size);
			SelectObject(mdc, oldBrush);
			DeleteObject(hBrush);
		}

		else if (Draw == 3)
		{
			MoveToEx(mdc, 260, 110, NULL);
			LineTo(mdc, 110, 410);
			LineTo(mdc, 410, 410);
			LineTo(mdc, 260, 110);
			hBrush = CreateSolidBrush(RGB(Color[0], Color[1], Color[2]));
			oldBrush = (HBRUSH)SelectObject(mdc, hBrush);
			Ellipse(mdc, MainCircle.x - MainCircle.size, MainCircle.y - MainCircle.size, MainCircle.x + MainCircle.size, MainCircle.y + MainCircle.size);
			SelectObject(mdc, oldBrush);
			DeleteObject(hBrush);
		}

		if(c == 1)
			TextOut(mdc, 10, 10, chat, lstrlen(chat));


		BitBlt(hdc, 0, 0, rt.right, rt.bottom, mdc, 0, 0, SRCCOPY);
		DeleteDC(mdc);
		DeleteObject(hBitmap);
		EndPaint(hwnd, &ps);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			if (MainCircle.d == 1)
			{
				if (Move == 1)
				{
					if (Draw == 1)
					{
						if (x < 360)
						{
							MainCircle.x = 260 + 150 * (sin(x * PI / 180));
							MainCircle.y = 260 + 150 * (cos(x * PI / 180));
							x += 5;
						}
						else
						{
							x = 0;
						}
					}
					else if (Draw == 2)
					{
						if (MainCircle.x == 110 && MainCircle.y == 110)
						{
							dx = (110 - 110) / 10;
							dy = (410 - 110) / 10;
						}
						else if (MainCircle.x == 110 && MainCircle.y == 410)
						{
							dx = (410 - 110) / 10;
							dy = (410 - 410) / 10;
						}
						else if (MainCircle.x == 410 && MainCircle.y == 410)
						{
							dx = (410 - 410) / 10;
							dy = (110 - 410) / 10;
						}
						else if (MainCircle.x == 410 && MainCircle.y == 110)
						{
							dx = (110 - 410) / 10;
							dy = (110 - 110) / 10;
						}
						MainCircle.x += dx;
						MainCircle.y += dy;
					}
					else if (Draw == 3)
					{
						if (MainCircle.x == 260 && MainCircle.y == 110)
						{
							dx = (110 - 260) / 10;
							dy = (410 - 110) / 10;
						}
						else if (MainCircle.x == 110 && MainCircle.y == 410)
						{
							dx = (410 - 110) / 10;
							dy = (410 - 410) / 10;
						}
						else if (MainCircle.x == 410 && MainCircle.y == 410)
						{
							dx = (260 - 410) / 10;
							dy = (110 - 410) / 10;
						}
						MainCircle.x += dx;
						MainCircle.y += dy;
					}
				}
			}
			break;
		case 2:
			if (MainCircle.d == 2)
			{
				if (Move == 1)
				{
					if (Draw == 1)
					{
						if (x < 360)
						{
							MainCircle.x = 260 + 150 * (sin(x * PI / 180));
							MainCircle.y = 260 + 150 * (cos(x * PI / 180));
							x += 5;
						}
						else
						{
							x = 0;
						}
					}
					else if (Draw == 2)
					{
						if (MainCircle.x == 110 && MainCircle.y == 110)
						{
							dx = (110 - 110) / 10;
							dy = (410 - 110) / 10;
						}
						else if (MainCircle.x == 110 && MainCircle.y == 410)
						{
							dx = (410 - 110) / 10;
							dy = (410 - 410) / 10;
						}
						else if (MainCircle.x == 410 && MainCircle.y == 410)
						{
							dx = (410 - 410) / 10;
							dy = (110 - 410) / 10;
						}
						else if (MainCircle.x == 410 && MainCircle.y == 110)
						{
							dx = (110 - 410) / 10;
							dy = (110 - 110) / 10;
						}
						MainCircle.x += dx;
						MainCircle.y += dy;
					}
					else if (Draw == 3)
					{
						if (MainCircle.x == 260 && MainCircle.y == 110)
						{
							dx = (110 - 260) / 10;
							dy = (410 - 110) / 10;
						}
						else if (MainCircle.x == 110 && MainCircle.y == 410)
						{
							dx = (410 - 110) / 10;
							dy = (410 - 410) / 10;
						}
						else if (MainCircle.x == 410 && MainCircle.y == 410)
						{
							dx = (260 - 410) / 10;
							dy = (110 - 410) / 10;
						}
						MainCircle.x += dx;
						MainCircle.y += dy;
					}
				}
			}
			break;
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		KillTimer(hwnd, 2);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}