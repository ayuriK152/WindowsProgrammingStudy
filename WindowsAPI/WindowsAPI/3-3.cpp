#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

enum Direction {
	None,
	Up,
	Down,
	Right,
	Left
};

class Car {
public:
	int x, y;
	int speed;
	bool isMoving;
	Direction direction;
	Car(int x, int y, int speed, Direction direction) {
		this->x = x;
		this->y = y;
		this->speed = speed;
		this->direction = direction;
		isMoving = true;
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

static vector<Car> cars;
static int carWidth = 30;
static int carHeight = 15;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	srand((unsigned int)time(NULL));
	RECT rt;
	PAINTSTRUCT ps;
	HDC hDC;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	switch (uMsg) {
		case WM_CREATE:
			GetClientRect(hWnd, &rt);
			cars.push_back(Car(100, rt.bottom / 2 + 25, rand() % 3 + 1, Right));
			cars.push_back(Car(100, rt.bottom / 2 + 75, rand() % 3 + 1, Right));
			cars.push_back(Car(700, rt.bottom / 2 - 25, rand() % 3 + 1, Left));
			cars.push_back(Car(700, rt.bottom / 2 - 75, rand() % 3 + 1, Left));
			SetTimer(hWnd, 1, 50, NULL);
			break;

		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rt);

			MoveToEx(hDC, 0, rt.bottom / 2 - 50, NULL);
			LineTo(hDC, rt.right, rt.bottom / 2 - 50);
			MoveToEx(hDC, 0, rt.bottom / 2 - 100, NULL);
			LineTo(hDC, rt.right, rt.bottom / 2 - 100);
			MoveToEx(hDC, 0, rt.bottom / 2 + 50, NULL);
			LineTo(hDC, rt.right, rt.bottom / 2 + 50);
			MoveToEx(hDC, 0, rt.bottom / 2 + 100, NULL);
			LineTo(hDC, rt.right, rt.bottom / 2 + 100);

			MoveToEx(hDC, 0, rt.bottom / 2, NULL);
			LineTo(hDC, rt.right, rt.bottom / 2);
			MoveToEx(hDC, rt.right / 2, 0, NULL);
			LineTo(hDC, rt.right / 2, rt.bottom);

			SetROP2(hDC, R2_XORPEN);
			hPen = (HPEN)GetStockObject(WHITE_PEN);
			oldPen = (HPEN)SelectObject(hDC, hPen);
			hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			for (int i = 0; i < cars.size(); i++) {
				if (cars[i].direction == Up || cars[i].direction == Down) {
					Rectangle(hDC, cars[i].x - carHeight, cars[i].y - carWidth, cars[i].x + carHeight, cars[i].y + carWidth);
				}
				else if (cars[i].direction == Right || cars[i].direction == Left) {
					Rectangle(hDC, cars[i].x - carWidth, cars[i].y - carHeight, cars[i].x + carWidth, cars[i].y + carHeight);
				}
			}

			EndPaint(hWnd, &ps);
			break;

		case WM_TIMER: {
			hDC = GetDC(hWnd);
			GetClientRect(hWnd, &rt);

			SetROP2(hDC, R2_XORPEN);
			hPen = (HPEN)GetStockObject(WHITE_PEN);
			oldPen = (HPEN)SelectObject(hDC, hPen);
			hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			for (int i = 0; i < cars.size(); i++) {
				int oldPos;
				switch (cars[i].direction) {
					case Up:
						Rectangle(hDC, cars[i].x - carHeight, cars[i].y - carWidth, cars[i].x + carHeight, cars[i].y + carWidth);
						oldPos = cars[i].y;
						cars[i].y += 10 * cars[i].speed;
						Rectangle(hDC, cars[i].x - carHeight, cars[i].y - carWidth, cars[i].x + carHeight, cars[i].y + carWidth);
						if (cars[i].y - carWidth >= rt.bottom) {
							Rectangle(hDC, cars[i].x - carHeight, oldPos - carWidth - rt.bottom, cars[i].x + carHeight, oldPos + carWidth - rt.bottom);
							cars[i].y = cars[i].y - rt.bottom;
							Rectangle(hDC, cars[i].x - carHeight, cars[i].y - carWidth, cars[i].x + carHeight, cars[i].y + carWidth);
						}
						if (cars[i].y + carWidth >= rt.bottom) {
							if (oldPos + carWidth >= rt.bottom) {
								Rectangle(hDC, cars[i].x - carHeight, oldPos - carWidth - rt.bottom, cars[i].x + carHeight, oldPos + carWidth - rt.bottom);
							}
							Rectangle(hDC, cars[i].x - carHeight, cars[i].y - carWidth - rt.bottom, cars[i].x + carHeight, cars[i].y + carWidth - rt.bottom);
						}
						break;
					case Down:
						Rectangle(hDC, cars[i].x - carHeight, cars[i].y - carWidth, cars[i].x + carHeight, cars[i].y + carWidth);
						oldPos = cars[i].y;
						cars[i].y -= 10 * cars[i].speed;
						Rectangle(hDC, cars[i].x - carHeight, cars[i].y - carWidth, cars[i].x + carHeight, cars[i].y + carWidth);
						if (cars[i].y + carWidth <= 0) {
							Rectangle(hDC, cars[i].x - carHeight, oldPos - carWidth + rt.bottom, cars[i].x + carHeight, oldPos + carWidth + rt.bottom);
							cars[i].y = cars[i].y + rt.bottom;
							Rectangle(hDC, cars[i].x - carHeight, cars[i].y - carWidth, cars[i].x + carHeight, cars[i].y + carWidth);
						}
						if (cars[i].y - carWidth <= 0) {
							if (oldPos - carWidth <= 0) {
								Rectangle(hDC, cars[i].x - carHeight, oldPos - carWidth + rt.bottom, cars[i].x + carHeight, oldPos + carWidth + rt.bottom);
							}
							Rectangle(hDC, cars[i].x - carHeight, cars[i].y - carWidth + rt.bottom, cars[i].x + carHeight, cars[i].y + carWidth + rt.bottom);
						}
						break;
					case Right:
						Rectangle(hDC, cars[i].x - carWidth, cars[i].y - carHeight, cars[i].x + carWidth, cars[i].y + carHeight);
						oldPos = cars[i].x;
						cars[i].x += 10 * cars[i].speed;
						Rectangle(hDC, cars[i].x - carWidth, cars[i].y - carHeight, cars[i].x + carWidth, cars[i].y + carHeight);
						if (cars[i].x - carWidth >= rt.right) {
							Rectangle(hDC, oldPos - carWidth - rt.right, cars[i].y - carHeight, oldPos + carWidth - rt.right, cars[i].y + carHeight);
							cars[i].x = cars[i].x - rt.right;
							Rectangle(hDC, cars[i].x - carWidth, cars[i].y - carHeight, cars[i].x + carWidth, cars[i].y + carHeight);
						}
						if (cars[i].x + carWidth >= rt.right) {
							if (oldPos + carWidth >= rt.right) {
								Rectangle(hDC, oldPos - carWidth - rt.right, cars[i].y - carHeight, oldPos + carWidth - rt.right, cars[i].y + carHeight);
							}
							Rectangle(hDC, cars[i].x - carWidth - rt.right, cars[i].y - carHeight, cars[i].x + carWidth - rt.right, cars[i].y + carHeight);
						}
						break;
					case Left:
						Rectangle(hDC, cars[i].x - carWidth, cars[i].y - carHeight, cars[i].x + carWidth, cars[i].y + carHeight);
						oldPos = cars[i].x;
						cars[i].x -= 10 * cars[i].speed;
						Rectangle(hDC, cars[i].x - carWidth, cars[i].y - carHeight, cars[i].x + carWidth, cars[i].y + carHeight);
						if (cars[i].x + carWidth <= 0) {
							Rectangle(hDC, oldPos - carWidth + rt.right, cars[i].y - carHeight, oldPos + carWidth + rt.right, cars[i].y + carHeight);
							cars[i].x = cars[i].x + rt.right;
							Rectangle(hDC, cars[i].x - carWidth, cars[i].y - carHeight, cars[i].x + carWidth, cars[i].y + carHeight);
						}
						if (cars[i].x - carWidth <= 0) {
							if (oldPos - carWidth <= 0) {
								Rectangle(hDC, oldPos - carWidth + rt.right, cars[i].y - carHeight, oldPos + carWidth + rt.right, cars[i].y + carHeight);
							}
							Rectangle(hDC, cars[i].x - carWidth + rt.right, cars[i].y - carHeight, cars[i].x + carWidth + rt.right, cars[i].y + carHeight);
						}
						break;
				}
			}

			ReleaseDC(hWnd, hDC);
			break;
		}

		case WM_CHAR: {
			if (wParam == '+') {

			}
			else if (wParam == '-') {

			}
			else if (wParam == 'a' || wParam == 'A') {

			}
			else if (wParam == 'q' || wParam == 'Q') {

			}
			break;
		}

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}