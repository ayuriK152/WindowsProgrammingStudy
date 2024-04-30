#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>

using namespace std;

enum Direction {
	None,
	Up,
	Down,
	Left,
	Right
};

class Player {
public :
	int x, y;
	Direction moveDir;
	COLORREF color;
	bool isHead;
	int moveType;
	Player(int x, int y, Direction moveDir, COLORREF color, bool isHead){
		this->x = x;
		this->y = y;
		this->moveDir = moveDir;
		this->color = color;
		this->isHead = isHead;
		this->moveType = -1;
	}
	Player(int x, int y, Direction moveDir, COLORREF color, bool isHead, int moveType) {
		this->x = x;
		this->y = y;
		this->moveDir = moveDir;
		this->color = color;
		this->isHead = isHead;
		this->moveType = moveType;
	}
};



class Food {
public:
	int x, y;
	COLORREF color;
	Food(int x, int y, COLORREF color) {
		this->x = x;
		this->y = y;
		this->color = color;
	}
};

class Obstacle {
public:
	int x, y;
	Obstacle(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

float DistancePoints(int x, int y, int mouseX, int mouseY) {
	return sqrt(pow(mouseX - x, 2) + pow(mouseY - y, 2));
}

bool IsInCircle(int x, int y, int mouseX, int mouseY, float radius) {
	if (DistancePoints(x, y, mouseX, mouseY) <= radius)
		return true;
	else
		return false;
}

bool IsInRect(int x, int y, int mouseX, int mouseY, int width, int height) {
	if (mouseX > x && mouseX < x + width && mouseY > y && mouseY < y + height)
		return true;
	else
		return false;
}

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
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

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 850, 850, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	srand((unsigned int)time(NULL));
	PAINTSTRUCT ps;
	HDC hDC;
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;
	int oldX, oldY;
	static COLORREF reverseColors[21];
	static vector<Player> snakeQueue;
	static vector<Player> tailQueue;
	static vector<Food> foodQueue;
	static vector<Obstacle> obsQueue;
	static int autoMoveOffset;
	static int moveSpeed;
	static int mouseX, mouseY;
	static bool isGameStarted;
	static bool isInitialized;

	switch (uMsg) {
		case WM_CREATE:
			reverseColors[0] = RGB(0, 63, 255);
			for (int i = 1; i < 21; i++) {
				reverseColors[i] = RGB(rand() % 255, rand() % 255, rand() % 255);
			}

			snakeQueue.push_back(Player(0, 0, Direction::Right, reverseColors[0], true));

			for (int i = 0; i < 20; i++) {
				int tempX, tempY;
				tempX = rand() % 40;
				tempY = rand() % 40;

				bool flag = false;
				for (int j = 0; j < foodQueue.size(); j++) {
					if ((tempX == 0 && tempY == 0) || (tempX == foodQueue[j].x && tempY == foodQueue[j].y))
						flag = true;
				}
				if (flag) {
					i--;
					continue;
				}
					
				foodQueue.push_back(Food(tempX, tempY, reverseColors[i + 1]));
			}
			moveSpeed = 100;
			autoMoveOffset = 1;
			mouseX = 0, mouseY = 0;

			isGameStarted = false;
			isInitialized = false;

			break;

		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);

			for (int x = 0; x <= 40; x++) {
				MoveToEx(hDC, x * 20, 0, NULL);
				LineTo(hDC, x * 20, 800);
			}
			for (int y = 0; y <= 40; y++) {
				MoveToEx(hDC, 0, y * 20, NULL);
				LineTo(hDC, 800, y * 20);
			}

			SetROP2(hDC, R2_XORPEN);
			hPen = (HPEN)GetStockObject(BLACK_PEN);
			SelectObject(hDC, hPen);

			if (!isInitialized) {
				for (int i = 0; i < foodQueue.size(); i++) {
					hBrush = CreateSolidBrush(reverseColors[i + 1]);
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

					Rectangle(hDC, foodQueue[i].x * 20 + 4, foodQueue[i].y * 20 + 4, (foodQueue[i].x + 1) * 20 - 3, (foodQueue[i].y + 1) * 20 - 3);

					DeleteObject(oldBrush);
				}
			}

			for (int i = 0; i < obsQueue.size(); i++) {
				hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

				Rectangle(hDC, obsQueue[i].x * 20 + 2, obsQueue[i].y * 20 + 2, (obsQueue[i].x + 1) * 20 - 1, (obsQueue[i].y + 1) * 20 - 1);

				DeleteObject(oldBrush);
			}

			if (!isInitialized) {
				hBrush = CreateSolidBrush(reverseColors[0]);
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

				Ellipse(hDC, 1, 1, 19, 19);

				DeleteObject(oldBrush);
			}

			isInitialized = true;

			EndPaint(hWnd, &ps);
			break;

		case WM_TIMER:
			hDC = GetDC(hWnd);
			SetROP2(hDC, R2_XORPEN);
			hPen = (HPEN)GetStockObject(BLACK_PEN);
			SelectObject(hDC, hPen);

			for (int i = 0; i < tailQueue.size(); i++) {
				hBrush = CreateSolidBrush(tailQueue[i].color);
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

				Ellipse(hDC, tailQueue[i].x * 20 + 1, tailQueue[i].y * 20 + 1, (tailQueue[i].x + 1) * 20 - 1, (tailQueue[i].y + 1) * 20 - 1);

				switch (tailQueue[i].moveType) {
					case 0: {
						if (tailQueue[i].moveDir == None) {
							tailQueue[i].moveDir = tailQueue[i].y == 0 ? Down : Up;
						}
						if (tailQueue[i].moveDir == Up) {
							if (tailQueue[i].y == 0) {
								tailQueue[i].y += 1;
								tailQueue[i].moveDir = Down;
							}
							else {
								tailQueue[i].y -= 1;
							}
						}
						else if (tailQueue[i].moveDir == Down) {
							if (tailQueue[i].y == 39) {
								tailQueue[i].y -= 1;
								tailQueue[i].moveDir = Up;
							}
							else {
								tailQueue[i].y += 1;
							}
						}

						break;
					}
					case 1: {
						if (tailQueue[i].moveDir == None) {
							tailQueue[i].moveDir = tailQueue[i].x == 0 ? Right : Left;
						}
						if (tailQueue[i].moveDir == Right) {
							if (tailQueue[i].x == 0) {
								tailQueue[i].x += 1;
								tailQueue[i].moveDir = Left;
							}
							else {
								tailQueue[i].x -= 1;
							}
						}
						else if (tailQueue[i].moveDir == Left) {
							if (tailQueue[i].x == 39) {
								tailQueue[i].x -= 1;
								tailQueue[i].moveDir = Right;
							}
							else {
								tailQueue[i].x += 1;
							}
						}

						break;
					}
					case 2: {
						if (tailQueue[i].moveDir == None) {
							if (tailQueue[i].x == 0) {
								if (tailQueue[i].y == 39)
									tailQueue[i].moveDir = Up;
								else
									tailQueue[i].moveDir = Right;
							}
							else if (tailQueue[i].y == 0) {
								if (tailQueue[i].x == 39)
									tailQueue[i].moveDir = Down;
								else
									tailQueue[i].moveDir = Right;
							}
							else if (tailQueue[i].x == 39) {
								if (tailQueue[i].y == 0)
									tailQueue[i].moveDir = Down;
								else
									tailQueue[i].moveDir = Left;
							}
							else if (tailQueue[i].y == 39) {
								if (tailQueue[i].x == 0)
									tailQueue[i].moveDir = Up;
								else
									tailQueue[i].moveDir = Left;
							}
							else {
								tailQueue[i].moveDir = Right;
							}
						}
						if (tailQueue[i].moveDir == Right) {
							tailQueue[i].x += 1;
							tailQueue[i].moveDir = Down;
						}
						else if (tailQueue[i].moveDir == Down) {
							tailQueue[i].y += 1;
							tailQueue[i].moveDir = Left;
						}
						else if (tailQueue[i].moveDir == Left) {
							tailQueue[i].x -= 1;
							tailQueue[i].moveDir = Up;
						}
						else if (tailQueue[i].moveDir == Up) {
							tailQueue[i].y -= 1;
							tailQueue[i].moveDir = Right;
						}

						break;
					}
				}

				if (snakeQueue[0].x == tailQueue[i].x && snakeQueue[0].y == tailQueue[i].y) {
					snakeQueue.push_back(Player(tailQueue[i].x, tailQueue[i].y, None, tailQueue[i].color, false));

					Ellipse(hDC, tailQueue[i].x * 20 + 1, tailQueue[i].y * 20 + 1, (tailQueue[i].x + 1) * 20 - 1, (tailQueue[i].y + 1) * 20 - 1);

					DeleteObject(oldBrush);

					tailQueue.erase(tailQueue.begin() + i);
					continue;
				}

				Ellipse(hDC, tailQueue[i].x * 20 + 1, tailQueue[i].y * 20 + 1, (tailQueue[i].x + 1) * 20 - 1, (tailQueue[i].y + 1) * 20 - 1);

				DeleteObject(oldBrush);
			}

			for (int i = snakeQueue.size() - 1; i >= 1; i--) {
				hBrush = CreateSolidBrush(snakeQueue[i].color);
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

				if (snakeQueue[i].x != snakeQueue[0].x || snakeQueue[i].y != snakeQueue[0].y)
					Ellipse(hDC, snakeQueue[i].x * 20 + 1, snakeQueue[i].y * 20 + 1, (snakeQueue[i].x + 1) * 20 - 1, (snakeQueue[i].y + 1) * 20 - 1);

				snakeQueue[i].x = snakeQueue[i - 1].x;
				snakeQueue[i].y = snakeQueue[i - 1].y;

				Ellipse(hDC, snakeQueue[i].x * 20 + 1, snakeQueue[i].y * 20 + 1, (snakeQueue[i].x + 1) * 20 - 1, (snakeQueue[i].y + 1) * 20 - 1);

				DeleteObject(oldBrush);
			}

			oldX = snakeQueue[0].x;
			oldY = snakeQueue[0].y;
			bool isCollide;
			switch (snakeQueue[0].moveDir) {
				case Direction::Up:
					if (snakeQueue[0].x == 39)
						autoMoveOffset = -1;
					else if (snakeQueue[0].x == 0)
						autoMoveOffset = 1;

					if (snakeQueue[0].y == 0) {
						snakeQueue[0].moveDir = Direction::Down;
						snakeQueue[0].x += autoMoveOffset;
						break;
					}

					isCollide = false;
					for (int i = 0; i < obsQueue.size(); i++) {
						if (obsQueue[i].x == snakeQueue[0].x && obsQueue[i].y + 1 == snakeQueue[0].y) {
							snakeQueue[0].moveDir = autoMoveOffset == -1 ? Direction::Left : Direction::Right;
							snakeQueue[0].x += autoMoveOffset;
							isCollide = true;
							break;
						}
					}
					if (isCollide)
						break;
					snakeQueue[0].y--;
					break;

				case Direction::Down:
					if (snakeQueue[0].x == 39)
						autoMoveOffset = -1;
					else if (snakeQueue[0].x == 0)
						autoMoveOffset = 1;

					if (snakeQueue[0].y == 39) {
						snakeQueue[0].moveDir = Direction::Up;
						snakeQueue[0].x += autoMoveOffset;
						break;
					}

					isCollide = false;
					for (int i = 0; i < obsQueue.size(); i++) {
						if (obsQueue[i].x == snakeQueue[0].x && obsQueue[i].y - 1 == snakeQueue[0].y) {
							snakeQueue[0].moveDir = autoMoveOffset == -1 ? Direction::Left : Direction::Right;
							snakeQueue[0].x += autoMoveOffset;
							isCollide = true;
							break;
						}
					}
					if (isCollide)
						break;
					snakeQueue[0].y++;
					break;

				case Direction::Left:
					if (snakeQueue[0].y == 39)
						autoMoveOffset = -1;
					else if (snakeQueue[0].y == 0)
						autoMoveOffset = 1;

					if (snakeQueue[0].x == 0) {
						snakeQueue[0].moveDir = Direction::Right;
						snakeQueue[0].y += autoMoveOffset;
						break;
					}

					isCollide = false;
					for (int i = 0; i < obsQueue.size(); i++) {
						if (obsQueue[i].x + 1 == snakeQueue[0].x && obsQueue[i].y == snakeQueue[0].y) {
							snakeQueue[0].moveDir = autoMoveOffset == -1 ? Direction::Up : Direction::Down;
							snakeQueue[0].y += autoMoveOffset;
							isCollide = true;
							break;
						}
					}
					if (isCollide)
						break;
					snakeQueue[0].x--;
					break;

				case Direction::Right:
					if (snakeQueue[0].y == 39)
						autoMoveOffset = -1;
					else if (snakeQueue[0].y == 0)
						autoMoveOffset = 1;

					if (snakeQueue[0].x == 39) {
						snakeQueue[0].moveDir = Direction::Left;
						snakeQueue[0].y += autoMoveOffset;
						break;
					}

					isCollide = false;
					for (int i = 0; i < obsQueue.size(); i++) {
						if (obsQueue[i].x - 1 == snakeQueue[0].x && obsQueue[i].y == snakeQueue[0].y) {
							snakeQueue[0].moveDir = autoMoveOffset == -1 ? Direction::Up : Direction::Down;
							snakeQueue[0].y += autoMoveOffset;
							isCollide = true;
							break;
						}
					}
					if (isCollide)
						break;
					snakeQueue[0].x++;
					break;
			}

			for (int i = 0; i < tailQueue.size(); i++) {
				if (snakeQueue[0].x == tailQueue[i].x && snakeQueue[0].y == tailQueue[i].y) {
					snakeQueue.push_back(Player(tailQueue[i].x, tailQueue[i].y, None, tailQueue[i].color, false));

					hBrush = CreateSolidBrush(tailQueue[i].color);
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

					Ellipse(hDC, tailQueue[i].x * 20 + 1, tailQueue[i].y * 20 + 1, (tailQueue[i].x + 1) * 20 - 1, (tailQueue[i].y + 1) * 20 - 1);

					DeleteObject(oldBrush);

					tailQueue.erase(tailQueue.begin() + i);
					break;
				}
			}

			for (int i = 0; i < foodQueue.size(); i++) {
				if (snakeQueue[0].x == foodQueue[i].x && snakeQueue[0].y == foodQueue[i].y) {
					hBrush = CreateSolidBrush(foodQueue[i].color);
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

					Rectangle(hDC, foodQueue[i].x * 20 + 4, foodQueue[i].y * 20 + 4, (foodQueue[i].x + 1) * 20 - 3, (foodQueue[i].y + 1) * 20 - 3);
					Ellipse(hDC, foodQueue[i].x * 20 + 1, foodQueue[i].y * 20 + 1, (foodQueue[i].x + 1) * 20 - 1, (foodQueue[i].y + 1) * 20 - 1);

					DeleteObject(oldBrush);

					tailQueue.push_back(Player(foodQueue[i].x, foodQueue[i].y, None, foodQueue[i].color, false, rand() % 4));
					foodQueue.erase(foodQueue.begin() + i);
					break;
				}
			}

			hBrush = CreateSolidBrush(snakeQueue[0].color);
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			Ellipse(hDC, oldX * 20 + 1, oldY * 20 + 1, (oldX + 1) * 20 - 1, (oldY + 1) * 20 - 1);
			Ellipse(hDC, snakeQueue[0].x * 20 + 1, snakeQueue[0].y * 20 + 1, (snakeQueue[0].x + 1) * 20 - 1, (snakeQueue[0].y + 1) * 20 - 1);

			DeleteObject(oldBrush);

			ReleaseDC(hWnd, hDC);
			break;

		case WM_KEYDOWN:
			if (wParam == VK_UP || wParam == VK_DOWN || wParam == VK_LEFT || wParam == VK_RIGHT) {
				switch (wParam) {
					case VK_UP:
						if (snakeQueue[0].moveDir == Direction::Down)
							break;
						snakeQueue[0].moveDir = Direction::Up;
						break;

					case VK_DOWN:
						if (snakeQueue[0].moveDir == Direction::Up)
							break;
						snakeQueue[0].moveDir = Direction::Down;
						break;

					case VK_LEFT:
						if (snakeQueue[0].moveDir == Direction::Right)
							break;
						snakeQueue[0].moveDir = Direction::Left;
						break;

					case VK_RIGHT:
						if (snakeQueue[0].moveDir == Direction::Left)
							break;
						snakeQueue[0].moveDir = Direction::Right;
						break;
				}
			}
			break;

		case WM_RBUTTONDOWN:
			if (obsQueue.size() >= 20) {
				break;
			}

			hDC = GetDC(hWnd);

			mouseX = LOWORD(lParam);
			mouseY = HIWORD(lParam);


			for (int y = 0; y < 40; y++) {
				for (int x = 0; x < 40; x++) {
					if (IsInRect(x * 20, y * 20, mouseX, mouseY, 20, 20)) {
						SetROP2(hDC, R2_XORPEN);
						hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
						oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

						bool isExits;
						isExits = false;
						for (int i = 0; i < obsQueue.size(); i++) {
							if (obsQueue[i].x == x && obsQueue[i].y == y) {
								Rectangle(hDC, obsQueue[i].x * 20 + 2, obsQueue[i].y * 20 + 2, (obsQueue[i].x + 1) * 20 - 1, (obsQueue[i].y + 1) * 20 - 1);
								obsQueue.erase(obsQueue.begin() + i);
								isExits = true;

								DeleteObject(oldBrush);
								break;
							}
						}
						if (isExits)
							break;
						obsQueue.push_back(Obstacle(x, y));
						Rectangle(hDC, x * 20 + 2, y * 20 + 2, (x + 1) * 20 - 1, (y + 1) * 20 - 1);

						DeleteObject(oldBrush);
						break;
					}
				}
			}

			ReleaseDC(hWnd, hDC);
			break;

		case WM_CHAR:
			srand((unsigned int)time(NULL));
			hDC = GetDC(hWnd);

			if (wParam == '+') {
				if (moveSpeed == 50)
					break;
				moveSpeed -= 50;
				KillTimer(hWnd, NULL);
				SetTimer(hWnd, 1, moveSpeed, NULL);
			}
			else if (wParam == '-') {
				if (moveSpeed == 250)
					break;
				moveSpeed += 50;
				KillTimer(hWnd, NULL);
				SetTimer(hWnd, 1, moveSpeed, NULL);
			}
			else if (wParam == 's') {
				SetTimer(hWnd, 1, moveSpeed, NULL);
				isGameStarted = true;
			}
			else if (wParam == 't') {
				SetROP2(hDC, R2_XORPEN);
				for (int i = 0; i < snakeQueue.size(); i++) {
					hBrush = CreateSolidBrush(snakeQueue[i].color);
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

					Ellipse(hDC, snakeQueue[i].x * 20 + 1, snakeQueue[i].y * 20 + 1, (snakeQueue[i].x + 1) * 20 - 1, (snakeQueue[i].y + 1) * 20 - 1);

					DeleteObject(oldBrush);
				}
				COLORREF tempColor = snakeQueue[0].color;
				for (int i = 0; i < snakeQueue.size() - 1; i++) {
					snakeQueue[i].color = snakeQueue[i + 1].color;

					hBrush = CreateSolidBrush(snakeQueue[i].color);
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

					Ellipse(hDC, snakeQueue[i].x * 20 + 1, snakeQueue[i].y * 20 + 1, (snakeQueue[i].x + 1) * 20 - 1, (snakeQueue[i].y + 1) * 20 - 1);

					DeleteObject(oldBrush);
				}
				snakeQueue[snakeQueue.size() - 1].color = tempColor;

				hBrush = CreateSolidBrush(snakeQueue[snakeQueue.size() - 1].color);
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

				Ellipse(hDC, snakeQueue[snakeQueue.size() - 1].x * 20 + 1, snakeQueue[snakeQueue.size() - 1].y * 20 + 1, (snakeQueue[snakeQueue.size() - 1].x + 1) * 20 - 1, (snakeQueue[snakeQueue.size() - 1].y + 1) * 20 - 1);

				DeleteObject(oldBrush);
			}
			else if (wParam == 'q') {
				PostQuitMessage(0);
			}

			ReleaseDC(hWnd, hDC);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}