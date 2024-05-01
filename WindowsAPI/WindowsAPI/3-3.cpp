#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

void CALLBACK AutoSign(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void ChangeSign(HWND hWnd, HDC hDC);
bool IsInCircle(int x1, int y1, int x2, int y2);

static int carWidth = 30;
static int carHeight = 15;

enum Direction {
	None,
	Up,
	Down,
	Right,
	Left
};

enum Sign {
	Vertical,
	VerticalReady,
	Horizontal,
	HorizontalReady
};

class Car {
public:
	int x, y;
	int left, top, right, bottom;
	int speed;
	bool isMoving;
	Direction direction;
	Car(int x, int y, int speed, Direction direction) {
		this->x = x;
		this->y = y;
		this->speed = speed;
		this->direction = direction;
		this->isMoving = true;
		if (direction == Up || direction == Down) {
			this->left = x - carHeight;
			this->top = y - carWidth;
			this->right = x + carHeight;
			this->bottom = y + carWidth;
		}
		else if (direction == Right || direction == Left) {
			this->left = x - carWidth;
			this->top = y - carHeight;
			this->right = x + carWidth;
			this->bottom = y + carHeight;
		}
	}
	Car(int x, int y, int speed, Direction direction, bool isMoving) {
		this->x = x;
		this->y = y;
		this->speed = speed;
		this->direction = direction;
		this->isMoving = isMoving;
		if (direction == Up || direction == Down) {
			this->left = x - carHeight;
			this->top = y - carWidth;
			this->right = x + carHeight;
			this->bottom = y + carWidth;
		}
		else if (direction == Right || direction == Left) {
			this->left = x - carWidth;
			this->top = y - carHeight;
			this->right = x + carWidth;
			this->bottom = y + carHeight;
		}
	}
	void MovePos(int x, int y) {
		this->x += x;
		this->y += y;
		if (direction == Up || direction == Down) {
			left = this->x - carHeight;
			top = this->y - carWidth;
			right = this->x + carHeight;
			bottom = this->y + carWidth;
		}
		else if (direction == Right || direction == Left) {
			left = this->x - carWidth;
			top = this->y - carHeight;
			right = this->x + carWidth;
			bottom = this->y + carHeight;
		}
	}
	void SetPos(int x, int y) {
		this->x = x;
		this->y = y;
		if (direction == Up || direction == Down) {
			left = this->x - carHeight;
			top = this->y - carWidth;
			right = this->x + carHeight;
			bottom = this->y + carWidth;
		}
		else if (direction == Right || direction == Left) {
			left = this->x - carWidth;
			top = this->y - carHeight;
			right = this->x + carWidth;
			bottom = this->y + carHeight;
		}
	}
};

class Player {
public:
	int x, y;
	bool isMoving;
	Direction direction;
	Player() {};
	Player(int x, int y) {
		this->x = x;
		this->y = y;
		isMoving = false;
		direction = Down;
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
static int crosswalkOffset = 180;
static Sign sign = Horizontal;
static Player player;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	srand((unsigned int)time(NULL));
	RECT rt;
	PAINTSTRUCT ps;
	HDC hDC;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	static int carSpeed = 8;

	switch (uMsg) {
		case WM_CREATE:
			GetClientRect(hWnd, &rt);
			player = Player(rt.right / 2 - 130, rt.bottom / 2 - 130);
			cars.push_back(Car(rt.right / 2 + 25, 500, rand() % 3 + 1, Up));
			cars.push_back(Car(rt.right / 2 + 75, 500, rand() % 3 + 1, Up));
			cars.push_back(Car(rt.right / 2 - 25, 50, rand() % 3 + 1, Down));
			cars.push_back(Car(rt.right / 2 - 75, 50, rand() % 3 + 1, Down));
			cars.push_back(Car(50, rt.bottom / 2 + 25, rand() % 3 + 1, Right, true));
			cars.push_back(Car(50, rt.bottom / 2 + 75, rand() % 3 + 1, Right, true));
			cars.push_back(Car(750, rt.bottom / 2 - 25, rand() % 3 + 1, Left, true));
			cars.push_back(Car(750, rt.bottom / 2 - 75, rand() % 3 + 1, Left, true));
			SetTimer(hWnd, 1, 45, NULL);
			break;

		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rt);

			hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			Rectangle(hDC, 0, rt.bottom / 2 - 110, rt.right, rt.bottom / 2 + 110);
			Rectangle(hDC, rt.right / 2 - 110, 0, rt.right / 2 + 110, rt.bottom);
			
			SelectObject(hDC, oldBrush);

			hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			Rectangle(hDC, rt.right / 2 - crosswalkOffset - 30, rt.bottom / 2, rt.right / 2 - crosswalkOffset + 3, rt.bottom / 2 + 102);
			Rectangle(hDC, rt.right / 2 + crosswalkOffset - 3, rt.bottom / 2 - 102, rt.right / 2 + crosswalkOffset + 30, rt.bottom / 2);
			Rectangle(hDC, rt.right / 2 - 102, rt.bottom / 2 - crosswalkOffset - 30, rt.right / 2, rt.bottom / 2 - crosswalkOffset + 3);
			Rectangle(hDC, rt.right / 2, rt.bottom / 2 + crosswalkOffset - 3, rt.right / 2 + 102, rt.bottom / 2 + crosswalkOffset + 30);

			SelectObject(hDC, oldBrush);

			hPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
			oldPen = (HPEN)SelectObject(hDC, hPen);

			MoveToEx(hDC, 0, rt.bottom / 2 - 100, NULL);
			LineTo(hDC, rt.right / 2 - crosswalkOffset, rt.bottom / 2 - 100);
			MoveToEx(hDC, rt.right / 2 + crosswalkOffset, rt.bottom / 2 - 100, NULL);
			LineTo(hDC, rt.right, rt.bottom / 2 - 100);
			MoveToEx(hDC, 0, rt.bottom / 2 + 100, NULL);
			LineTo(hDC, rt.right / 2 - crosswalkOffset, rt.bottom / 2 + 100);
			MoveToEx(hDC, rt.right / 2 + crosswalkOffset, rt.bottom / 2 + 100, NULL);
			LineTo(hDC, rt.right, rt.bottom / 2 + 100);
			MoveToEx(hDC, rt.right / 2 - 100, 0, NULL);
			LineTo(hDC, rt.right / 2 - 100, rt.bottom / 2 - crosswalkOffset);
			MoveToEx(hDC, rt.right / 2 - 100, rt.bottom / 2 + crosswalkOffset, NULL);
			LineTo(hDC, rt.right / 2 - 100, rt.bottom);
			MoveToEx(hDC, rt.right / 2 + 100, 0, NULL);
			LineTo(hDC, rt.right / 2 + 100, rt.bottom / 2 - crosswalkOffset);
			MoveToEx(hDC, rt.right / 2 + 100, rt.bottom / 2 + crosswalkOffset, NULL);
			LineTo(hDC, rt.right / 2 + 100, rt.bottom);

			SelectObject(hDC, oldPen);
			DeleteObject(hPen);

			hPen = CreatePen(PS_DOT, 3, RGB(255, 255, 255));
			oldPen = (HPEN)SelectObject(hDC, hPen);

			MoveToEx(hDC, 0, rt.bottom / 2 - 50, NULL);
			LineTo(hDC, rt.right / 2 - crosswalkOffset, rt.bottom / 2 - 50);
			MoveToEx(hDC, rt.right / 2 + crosswalkOffset, rt.bottom / 2 - 50, NULL);
			LineTo(hDC, rt.right, rt.bottom / 2 - 50);
			MoveToEx(hDC, 0, rt.bottom / 2 + 50, NULL);
			LineTo(hDC, rt.right / 2 - crosswalkOffset, rt.bottom / 2 + 50);
			MoveToEx(hDC, rt.right / 2 + crosswalkOffset, rt.bottom / 2 + 50, NULL);
			LineTo(hDC, rt.right, rt.bottom / 2 + 50);
			MoveToEx(hDC, rt.right / 2 - 50, 0, NULL);
			LineTo(hDC, rt.right / 2 - 50, rt.bottom / 2 - crosswalkOffset);
			MoveToEx(hDC, rt.right / 2 - 50, rt.bottom / 2 + crosswalkOffset, NULL);
			LineTo(hDC, rt.right / 2 - 50, rt.bottom);
			MoveToEx(hDC, rt.right / 2 + 50, 0, NULL);
			LineTo(hDC, rt.right / 2 + 50, rt.bottom / 2 - crosswalkOffset);
			MoveToEx(hDC, rt.right / 2 + 50, rt.bottom / 2 + crosswalkOffset, NULL);
			LineTo(hDC, rt.right / 2 + 50, rt.bottom);

			SelectObject(hDC, oldPen);
			DeleteObject(hPen);

			hPen = CreatePen(PS_SOLID, 5, RGB(255, 191, 0));
			oldPen = (HPEN)SelectObject(hDC, hPen);

			MoveToEx(hDC, 0, rt.bottom / 2, NULL);
			LineTo(hDC, rt.right / 2 - crosswalkOffset, rt.bottom / 2);
			MoveToEx(hDC, rt.right / 2 + crosswalkOffset, rt.bottom / 2, NULL);
			LineTo(hDC, rt.right, rt.bottom / 2);
			MoveToEx(hDC, rt.right / 2, 0, NULL);
			LineTo(hDC, rt.right / 2, rt.bottom / 2 - crosswalkOffset);
			MoveToEx(hDC, rt.right / 2, rt.bottom / 2 + crosswalkOffset, NULL);
			LineTo(hDC, rt.right / 2, rt.bottom);

			SelectObject(hDC, oldPen);
			DeleteObject(hPen);

			// Ω≈»£µÓ
			ChangeSign(hWnd, hDC);

			SetROP2(hDC, R2_XORPEN);
			hPen = (HPEN)GetStockObject(WHITE_PEN);
			oldPen = (HPEN)SelectObject(hDC, hPen);
			hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			for (int i = 0; i < cars.size(); i++) {
				Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
			}

			SelectObject(hDC, oldPen);
			SelectObject(hDC, oldBrush);

			hBrush = CreateSolidBrush(RGB(255, 255, 0));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			Ellipse(hDC, player.x - 15, player.y - 15, player.x + 15, player.y + 15);

			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);

			EndPaint(hWnd, &ps);
			break;

		case WM_TIMER: {
			hDC = GetDC(hWnd);
			GetClientRect(hWnd, &rt);

			SetROP2(hDC, R2_XORPEN);
			hPen = (HPEN)GetStockObject(WHITE_PEN);
			oldPen = (HPEN)SelectObject(hDC, hPen);
			hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			for (int i = 0; i < cars.size(); i++) {
				int oldPos;
				if (!cars[i].isMoving)
					continue;
				switch (cars[i].direction) {
					case Up:
						if (sign != Vertical) {
							if (cars[i].top - 20 <= rt.bottom / 2 + crosswalkOffset && cars[i].top > rt.bottom / 2 + crosswalkOffset) {
								Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
								cars[i].SetPos(cars[i].x, rt.bottom / 2 + crosswalkOffset + 40 + carWidth);
								Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
								cars[i].isMoving = false;
								break;
							}
						}
						Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
						oldPos = cars[i].y;
						cars[i].MovePos(0, -carSpeed * cars[i].speed);
						Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
						if (cars[i].bottom <= 0) {
							Rectangle(hDC, cars[i].left, oldPos - carWidth + rt.bottom, cars[i].right, oldPos + carWidth + rt.bottom);
							cars[i].SetPos(cars[i].x, cars[i].y + rt.bottom);
							Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
						}
						if (cars[i].top <= 0) {
							if (oldPos - carWidth <= 0) {
								Rectangle(hDC, cars[i].left, oldPos - carWidth + rt.bottom, cars[i].right, oldPos + carWidth + rt.bottom);
							}
							Rectangle(hDC, cars[i].left, cars[i].top + rt.bottom, cars[i].right, cars[i].bottom + rt.bottom);
						}
						break;
					case Down:
						if (sign != Vertical) {
							if (cars[i].bottom + 20 <= rt.bottom / 2 - crosswalkOffset && cars[i].top < rt.bottom / 2 - crosswalkOffset) {
								Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
								cars[i].SetPos(cars[i].x, rt.bottom / 2 - crosswalkOffset - 40 - carWidth);
								Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
								cars[i].isMoving = false;
								break;
							}
						}
						Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
						oldPos = cars[i].y;
						cars[i].MovePos(0, carSpeed * cars[i].speed);
						Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
						if (cars[i].top >= rt.bottom) {
							Rectangle(hDC, cars[i].left, oldPos - carWidth - rt.bottom, cars[i].right, oldPos + carWidth - rt.bottom);
							cars[i].SetPos(cars[i].x, cars[i].y - rt.bottom);
							Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
						}
						if (cars[i].bottom >= rt.bottom) {
							if (oldPos + carWidth >= rt.bottom) {
								Rectangle(hDC, cars[i].left, oldPos - carWidth - rt.bottom, cars[i].right, oldPos + carWidth - rt.bottom);
							}
							Rectangle(hDC, cars[i].left, cars[i].top - rt.bottom, cars[i].right, cars[i].bottom - rt.bottom);
						}
						break;
					case Right:
						if (sign != Horizontal) {
							if (cars[i].right + 40 >= rt.right / 2 - crosswalkOffset && cars[i].right <= rt.right / 2 - crosswalkOffset) {
								Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
								cars[i].SetPos(rt.right / 2 - crosswalkOffset - 40 - carWidth, cars[i].y);
								Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
								cars[i].isMoving = false;
								break;
							}
						}
						Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
						oldPos = cars[i].x;
						cars[i].MovePos(carSpeed * cars[i].speed, 0);
						Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
						if (cars[i].left >= rt.right) {
							Rectangle(hDC, oldPos - carWidth - rt.right, cars[i].top, oldPos + carWidth - rt.right, cars[i].bottom);
							cars[i].SetPos(cars[i].x - rt.right, cars[i].y);
							Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
						}
						if (cars[i].right >= rt.right) {
							if (oldPos + carWidth >= rt.right) {
								Rectangle(hDC, oldPos - carWidth - rt.right, cars[i].top, oldPos + carWidth - rt.right, cars[i].bottom);
							}
							Rectangle(hDC, cars[i].left - rt.right, cars[i].top, cars[i].right - rt.right, cars[i].bottom);
						}
						break;
					case Left:
						if (sign != Horizontal) {
							if (cars[i].left - 40 <= rt.right / 2 + crosswalkOffset && cars[i].left >= rt.right / 2 + crosswalkOffset) {
								Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
								cars[i].SetPos(rt.right / 2 + crosswalkOffset + 40 + carWidth, cars[i].y);
								Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
								cars[i].isMoving = false;
								break;
							}
						}
						Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
						oldPos = cars[i].x;
						cars[i].MovePos(-carSpeed * cars[i].speed, 0);
						Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
						if (cars[i].right <= 0) {
							Rectangle(hDC, oldPos - carWidth + rt.right, cars[i].top, oldPos + carWidth + rt.right, cars[i].bottom);
							cars[i].SetPos(cars[i].x + rt.right, cars[i].y);
							Rectangle(hDC, cars[i].left, cars[i].top, cars[i].right, cars[i].bottom);
						}
						if (cars[i].left <= 0) {
							if (oldPos - carWidth <= 0) {
								Rectangle(hDC, oldPos - carWidth + rt.right, cars[i].top, oldPos + carWidth + rt.right, cars[i].bottom);
							}
							Rectangle(hDC, cars[i].left + rt.right, cars[i].top, cars[i].right + rt.right, cars[i].bottom);
						}
						break;
				}
			}

			SelectObject(hDC, oldPen);
			SelectObject(hDC, oldBrush);

			if (player.isMoving) {
				hBrush = CreateSolidBrush(RGB(255, 255, 0));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

				Ellipse(hDC, player.x - 15, player.y - 15, player.x + 15, player.y + 15);
				if (player.direction == Up) {
					player.y -= 5;
				}
				else if (player.direction == Down) {
					player.y += 5;
				}
				else if (player.direction == Right) {
					player.x += 5;
				}
				else if (player.direction == Left) {
					player.x -= 5;
				}
				Ellipse(hDC, player.x - 15, player.y - 15, player.x + 15, player.y + 15);

				if (player.direction == Up && player.y == rt.bottom / 2 - 130) {
					player.isMoving = false;
					player.direction = Left;
				}
				else if (player.direction == Down && player.y == rt.bottom / 2 + 130) {
					player.isMoving = false;
					player.direction = Right;
				}
				else if (player.direction == Right && player.x == rt.right / 2 + 130) {
					player.isMoving = false;
					player.direction = Up;
				}
				else if (player.direction == Left && player.x == rt.right / 2 - 130) {
					player.isMoving = false;
					player.direction = Down;
				}

				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);
			}

			ReleaseDC(hWnd, hDC);
			break;
		}

		case WM_CHAR: {
			hDC = GetDC(hWnd);

			if (wParam == '+') {
				if (carSpeed >= 12)
					break;
				carSpeed += 2;
			}
			else if (wParam == '-') {
				if (carSpeed <= 4)
					break;
				carSpeed -= 2;
			}
			else if (wParam == 'a' || wParam == 'A') {
				SetTimer(hWnd, 2, 3000, AutoSign);
			}
			else if (wParam == 'q' || wParam == 'Q') {
				PostQuitMessage(0);
			}

			ReleaseDC(hWnd, hDC);
			break;
		}

		case WM_LBUTTONDOWN: {
			GetClientRect(hWnd, &rt);
			int mx, my;
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			if (sign == Vertical &&
				(IsInCircle(mx, my, rt.right / 2 + 165, rt.bottom / 2 + crosswalkOffset + 15) ||
				IsInCircle(mx, my, rt.right / 2 - 165, rt.bottom / 2 - crosswalkOffset - 15))) {
				KillTimer(hWnd, 2);
				sign = VerticalReady;
				ChangeSign(hWnd, GetDC(hWnd));
			}
			else if (sign == VerticalReady &&
				(IsInCircle(mx, my, rt.right / 2 + 135, rt.bottom / 2 + crosswalkOffset + 15) ||
					IsInCircle(mx, my, rt.right / 2 - 135, rt.bottom / 2 - crosswalkOffset - 15))) {
				KillTimer(hWnd, 2);
				sign = Horizontal;
				ChangeSign(hWnd, GetDC(hWnd));
			}
			else if (sign == Horizontal &&
				(IsInCircle(mx, my, rt.right / 2 + crosswalkOffset + 15, rt.bottom / 2 - 165) ||
					IsInCircle(mx, my, rt.right / 2 - crosswalkOffset - 15, rt.bottom / 2 + 165))) {
				KillTimer(hWnd, 2);
				sign = HorizontalReady;
				ChangeSign(hWnd, GetDC(hWnd));
			}
			else if (sign == HorizontalReady &&
				(IsInCircle(mx, my, rt.right / 2 + crosswalkOffset + 15, rt.bottom / 2 - 135) ||
					IsInCircle(mx, my, rt.right / 2 - crosswalkOffset - 15, rt.bottom / 2 + 135))) {
				KillTimer(hWnd, 2);
				sign = Vertical;
				ChangeSign(hWnd, GetDC(hWnd));
			}
			break;
		}

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CALLBACK AutoSign(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
	sign = (Sign)(((int)sign + 1) % 4);
	if (sign == Vertical || sign == Horizontal) {
		SetTimer(hWnd, 2, 3000, AutoSign);
	}
	else {
		SetTimer(hWnd, 2, 1000, AutoSign);
	}
	ChangeSign(hWnd, GetDC(hWnd));
}

void ChangeSign(HWND hWnd, HDC hDC) {
	RECT rt;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	GetClientRect(hWnd, &rt);

	hPen = (HPEN)GetStockObject(WHITE_PEN);
	oldPen = (HPEN)SelectObject(hDC, hPen);
	hBrush = CreateSolidBrush(RGB(63, 0, 0));
	oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	Ellipse(hDC, rt.right / 2 + 120, rt.bottom / 2 + crosswalkOffset, rt.right / 2 + 150, rt.bottom / 2 + crosswalkOffset + 30);
	Ellipse(hDC, rt.right / 2 - 120, rt.bottom / 2 - crosswalkOffset, rt.right / 2 - 150, rt.bottom / 2 - crosswalkOffset - 30);
	Ellipse(hDC, rt.right / 2 + crosswalkOffset, rt.bottom / 2 - 150, rt.right / 2 + crosswalkOffset + 30, rt.bottom / 2 - 120);
	Ellipse(hDC, rt.right / 2 - crosswalkOffset, rt.bottom / 2 + 150, rt.right / 2 - crosswalkOffset - 30, rt.bottom / 2 + 120);

	SelectObject(hDC, oldBrush);
	DeleteObject(hBrush);

	hBrush = CreateSolidBrush(RGB(63, 47, 0));
	oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	Ellipse(hDC, rt.right / 2 + 150, rt.bottom / 2 + crosswalkOffset, rt.right / 2 + 180, rt.bottom / 2 + crosswalkOffset + 30);
	Ellipse(hDC, rt.right / 2 - 150, rt.bottom / 2 - crosswalkOffset, rt.right / 2 - 180, rt.bottom / 2 - crosswalkOffset - 30);
	Ellipse(hDC, rt.right / 2 + crosswalkOffset, rt.bottom / 2 - 150, rt.right / 2 + crosswalkOffset + 30, rt.bottom / 2 - 180);
	Ellipse(hDC, rt.right / 2 - crosswalkOffset, rt.bottom / 2 + 150, rt.right / 2 - crosswalkOffset - 30, rt.bottom / 2 + 180);

	SelectObject(hDC, oldBrush);
	DeleteObject(hBrush);

	hBrush = CreateSolidBrush(RGB(0, 63, 0));
	oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	Ellipse(hDC, rt.right / 2 + 180, rt.bottom / 2 + crosswalkOffset, rt.right / 2 + 210, rt.bottom / 2 + crosswalkOffset + 30);
	Ellipse(hDC, rt.right / 2 - 180, rt.bottom / 2 - crosswalkOffset, rt.right / 2 - 210, rt.bottom / 2 - crosswalkOffset - 30);
	Ellipse(hDC, rt.right / 2 + crosswalkOffset, rt.bottom / 2 - 180, rt.right / 2 + crosswalkOffset + 30, rt.bottom / 2 - 210);
	Ellipse(hDC, rt.right / 2 - crosswalkOffset, rt.bottom / 2 + 180, rt.right / 2 - crosswalkOffset - 30, rt.bottom / 2 + 210);

	SelectObject(hDC, oldBrush);
	DeleteObject(hBrush);

	hBrush = CreateSolidBrush(RGB(255, 0, 0));
	oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	if (sign != Horizontal && sign != HorizontalReady) {
		Ellipse(hDC, rt.right / 2 + crosswalkOffset, rt.bottom / 2 - 150, rt.right / 2 + crosswalkOffset + 30, rt.bottom / 2 - 120);
		Ellipse(hDC, rt.right / 2 - crosswalkOffset, rt.bottom / 2 + 150, rt.right / 2 - crosswalkOffset - 30, rt.bottom / 2 + 120);
	}
	else if (sign != Vertical && sign != VerticalReady) {
		Ellipse(hDC, rt.right / 2 + 120, rt.bottom / 2 + crosswalkOffset, rt.right / 2 + 150, rt.bottom / 2 + crosswalkOffset + 30);
		Ellipse(hDC, rt.right / 2 - 120, rt.bottom / 2 - crosswalkOffset, rt.right / 2 - 150, rt.bottom / 2 - crosswalkOffset - 30);
	}

	SelectObject(hDC, oldBrush);
	DeleteObject(hBrush);

	hBrush = CreateSolidBrush(RGB(255, 191, 0));
	oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	if (sign == HorizontalReady) {
		Ellipse(hDC, rt.right / 2 + crosswalkOffset, rt.bottom / 2 - 150, rt.right / 2 + crosswalkOffset + 30, rt.bottom / 2 - 180);
		Ellipse(hDC, rt.right / 2 - crosswalkOffset, rt.bottom / 2 + 150, rt.right / 2 - crosswalkOffset - 30, rt.bottom / 2 + 180);
	}
	else if (sign == VerticalReady) {
		Ellipse(hDC, rt.right / 2 + 150, rt.bottom / 2 + crosswalkOffset, rt.right / 2 + 180, rt.bottom / 2 + crosswalkOffset + 30);
		Ellipse(hDC, rt.right / 2 - 150, rt.bottom / 2 - crosswalkOffset, rt.right / 2 - 180, rt.bottom / 2 - crosswalkOffset - 30);
	}

	SelectObject(hDC, oldBrush);
	DeleteObject(hBrush);

	hBrush = CreateSolidBrush(RGB(0, 255, 0));
	oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	if (sign == Horizontal) {
		Ellipse(hDC, rt.right / 2 + crosswalkOffset, rt.bottom / 2 - 180, rt.right / 2 + crosswalkOffset + 30, rt.bottom / 2 - 210);
		Ellipse(hDC, rt.right / 2 - crosswalkOffset, rt.bottom / 2 + 180, rt.right / 2 - crosswalkOffset - 30, rt.bottom / 2 + 210);

		if (player.direction == Left || player.direction == Right) {
			player.isMoving = true;
		}
	}
	else if (sign == Vertical) {
		Ellipse(hDC, rt.right / 2 + 180, rt.bottom / 2 + crosswalkOffset, rt.right / 2 + 210, rt.bottom / 2 + crosswalkOffset + 30);
		Ellipse(hDC, rt.right / 2 - 180, rt.bottom / 2 - crosswalkOffset, rt.right / 2 - 210, rt.bottom / 2 - crosswalkOffset - 30);

		if (player.direction == Up || player.direction == Down) {
			player.isMoving = true;
		}
	}

	for (int i = 0; i < cars.size(); i++) {
		if (sign == Vertical && (cars[i].direction == Up || cars[i].direction == Down))
			cars[i].isMoving = true;
		else if (sign == Horizontal && (cars[i].direction == Right || cars[i].direction == Left))
			cars[i].isMoving = true;
	}

	SelectObject(hDC, oldBrush);
	DeleteObject(hBrush);

	SelectObject(hDC, oldPen);
}

bool IsInCircle(int x1, int y1, int x2, int y2) {
	if (sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)) < 15)
		return true;
	else
		return false;
}