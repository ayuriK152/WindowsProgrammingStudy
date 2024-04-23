#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

class Ball {
public:
	int x, y;
	Ball(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

class Brick {
public:
	int x, y;
	Brick(int x, int y) {
		this->x = x;
		this->y = y;
	}
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

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 0, 0, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
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
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	static vector<Brick> bricks[3];

	switch (uMsg) {
		case WM_CREATE:
			for (int y = 0; y < 3; y++) {
				for (int x = 0; x < 10; x++) {
					bricks[y].push_back(Brick(x, y));
				}
			}
			break;

		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);

			hPen = (HPEN)GetStockObject(WHITE_PEN);
			oldPen = (HPEN)SelectObject(hDC, hPen);

			for (int y = 0; y < 3; y++) {
				for (int x = 0; x < 10; x++) {
					Rectangle(hDC, x * 60 + 80, y * 20 + 400, (x + 1) * 60 + 80, (y + 1) * 20 + 400);
				}
			}

			EndPaint(hWnd, &ps);
			break;

		case WM_CHAR:
			hDC = GetDC(hWnd);

			if (wParam == 's' || wParam == 'S') {

			}

			else if (wParam == 'p' || wParam == 'P') {

			}

			else if (wParam == 'n' || wParam == 'N') {

			}

			else if (wParam == '+') {

			}

			else if (wParam == '-') {

			}

			else if (wParam == 'q' || wParam == 'Q') {
				PostQuitMessage(0);
				break;
			}

			break;

		case WM_LBUTTONDOWN:
			break;

		case WM_RBUTTONDOWN:
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}