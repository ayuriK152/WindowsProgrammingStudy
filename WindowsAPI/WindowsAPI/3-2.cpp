#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

int CCW(int x1, int y1, int x2, int y2, int x3, int y3) {
	return (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
}

class Ball {
public:
	int x, y;
	int speed;
	int direction;
	Ball(int x, int y) {
		this->x = x;
		this->y = y;
		speed = 1;
		direction = 0;
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

static Ball ball(400, 20);
static vector<Brick> bricks[3];

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	srand((unsigned int)time(NULL));
	RECT rt;
	PAINTSTRUCT ps;
	HDC hDC;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	static TCHAR lifeStr[20];
	static int ballSize = 8;
	static int brickWidth = 60;
	static int brickHeight = 20;
	static int lifeCnt = 3;
	static int brokenBricks = 0;
	static bool isStarted = false;
	static bool isPaused = false;
	static bool isGameover = false;

	switch (uMsg) {
		case WM_CREATE:
			for (int y = 0; y < 3; y++) {
				for (int x = 0; x < 10; x++) {
					bricks[y].push_back(Brick(x * 60 + 80, y * 20 + 400));
				}
			}
			wsprintf(lifeStr, L"LIFE:");
			for (int i = 0; i < 3; i++) {
				lifeStr[5 + i] = 'O';
			}
			lifeStr[8] = '\0';
			break;

		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);

			SetROP2(hDC, R2_XORPEN);
			hPen = (HPEN)GetStockObject(BLACK_PEN);
			oldPen = (HPEN)SelectObject(hDC, hPen);
			hBrush = CreateSolidBrush(RGB(255, 63, 255));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			Ellipse(hDC, ball.x - ballSize, ball.y - ballSize, ball.x + ballSize, ball.y + ballSize);

			hBrush = CreateSolidBrush(RGB(0, 63, 255));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < bricks[i].size(); j++) {
					Rectangle(hDC, bricks[i][j].x, bricks[i][j].y, bricks[i][j].x + brickWidth, bricks[i][j].y + brickHeight);
				}
			}

			SelectObject(hDC, hPen);
			DeleteObject(hBrush);

			TextOut(hDC, 0, 0, lifeStr, lstrlen(lifeStr));

			EndPaint(hWnd, &ps);
			break;
			
		case WM_TIMER: {
			if (isPaused || isGameover || brokenBricks >= 30)
				break;

			GetClientRect(hWnd, &rt);
			hDC = GetDC(hWnd);

			SetROP2(hDC, R2_XORPEN);
			hPen = (HPEN)GetStockObject(BLACK_PEN);
			oldPen = (HPEN)SelectObject(hDC, hPen);
			hBrush = CreateSolidBrush(RGB(255, 63, 255));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			if (ball.direction == 0) {
				Ellipse(hDC, ball.x - ballSize, ball.y - ballSize, ball.x + ballSize, ball.y + ballSize);
				ball.y += ball.speed * 5;
				Ellipse(hDC, ball.x - ballSize, ball.y - ballSize, ball.x + ballSize, ball.y + ballSize);

				if (ball.y + ballSize >= rt.bottom) {
					lifeCnt -= 1;
					lifeStr[5 + lifeCnt] = 'X';
					if (lifeCnt > 0) {
						Ellipse(hDC, ball.x - ballSize, ball.y - ballSize, ball.x + ballSize, ball.y + ballSize);
						ball.x = 400;
						ball.y = ballSize;
						ball.speed = 1;
						KillTimer(hWnd, 1);
						isStarted = false;
						Ellipse(hDC, ball.x - ballSize, ball.y - ballSize, ball.x + ballSize, ball.y + ballSize);
					}
					else
						isGameover = true;
				}
			}
			else {
				Ellipse(hDC, ball.x - ballSize, ball.y - ballSize, ball.x + ballSize, ball.y + ballSize);
				ball.y -= (5 * ball.speed / 2) * ball.speed;
				ball.x += (5 * ball.direction * 2 / ball.speed) * ball.speed;
				if ((ball.x - ballSize <= 0 && ball.direction == -1) || (ball.x + ballSize >= rt.right && ball.direction == 1))
					ball.direction *= -1;
				if (ball.y - ballSize <= 0) {
					ball.direction = 0;
					ball.speed = rand() % 3 + 1;
				}
				Ellipse(hDC, ball.x - ballSize, ball.y - ballSize, ball.x + ballSize, ball.y + ballSize);
			}

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < bricks[i].size(); j++) {
					if (ball.x > bricks[i][j].x - ballSize &&
						ball.x < bricks[i][j].x + brickWidth + ballSize &&
						ball.y > bricks[i][j].y - ballSize &&
						ball.y < bricks[i][j].y + brickHeight + ballSize) {

						DeleteObject(oldBrush);
						hBrush = CreateSolidBrush(RGB(0, 63, 255));
						oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
						Rectangle(hDC, bricks[i][j].x, bricks[i][j].y, bricks[i][j].x + brickWidth, bricks[i][j].y + brickHeight);
						bricks[i].erase(bricks[i].begin() + j);
						DeleteObject(oldBrush);
						j--;
						brokenBricks++;
						if (ball.direction == 0)
							ball.direction = rand() % 2 == 0 ? -1 : 1;
						else
							ball.direction *= -1;
					}
				}
			}

			TextOut(hDC, 0, 0, lifeStr, lstrlen(lifeStr));

			if (brokenBricks >= 30)
				TextOut(hDC, 360, 250, L"YOU WIN!", lstrlen(L"YOU WIN!"));
			if (isGameover)
				TextOut(hDC, 360, 250, L"GAMEOVER", lstrlen(L"GAMEOVER"));

			DeleteObject(oldBrush);
			ReleaseDC(hWnd, hDC);
			break;
		}

		case WM_CHAR:
			hDC = GetDC(hWnd);

			if (wParam == 's' || wParam == 'S') {
				SetTimer(hWnd, 1, 30, NULL);
				isStarted = true;
			}

			else if (wParam == 'p' || wParam == 'P') {
				isPaused = !isPaused;
				if (isPaused)
					SetTextColor(hDC, RGB(0, 0, 0));
				else
					SetTextColor(hDC, RGB(255, 255, 255));
				TCHAR briefStr[30];
				wsprintf(briefStr, L"BROKEN BRICKS:%d", brokenBricks);
				TextOut(hDC, 0, 30, briefStr, lstrlen(briefStr));
			}

			else if (wParam == 'n' || wParam == 'N') {
				KillTimer(hWnd, 1);
				isStarted = false;
				isPaused = false;
				isGameover = false;
				lifeCnt = 3;
				brokenBricks = 0;
				for (int y = 0; y < 3; y++) {
					bricks[y].clear();
					for (int x = 0; x < 10; x++) {
						bricks[y].push_back(Brick(x * 60 + 80, y * 20 + 400));
					}
					lifeStr[5 + y] = 'O';
				}
				ball.x = 400;
				ball.y = ballSize;
				ball.speed = 1;
				InvalidateRect(hWnd, NULL, TRUE);
			}

			else if (wParam == '+') {
				if (ball.speed == 3 || isPaused || !isStarted)
					break;
				ball.speed += 1;
			}

			else if (wParam == '-') {
				if (ball.speed == 1 || isPaused || !isStarted)
					break;
				ball.speed -= 1;
			}

			else if (wParam == 'q' || wParam == 'Q') {
				PostQuitMessage(0);
				break;
			}

			break;

		case WM_LBUTTONDOWN:
			hDC = GetDC(hWnd);

			SetROP2(hDC, R2_XORPEN);
			hPen = (HPEN)GetStockObject(BLACK_PEN);
			hBrush = CreateSolidBrush(RGB(0, 63, 255));
			oldPen = (HPEN)SelectObject(hDC, hPen);
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < bricks[i].size(); j++) {
					Rectangle(hDC, bricks[i][j].x, bricks[i][j].y, bricks[i][j].x + brickWidth, bricks[i][j].y + brickHeight);
					bricks[i][j].x -= 10;
				}
			}

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < bricks[i].size(); j++) {
					Rectangle(hDC, bricks[i][j].x, bricks[i][j].y, bricks[i][j].x + brickWidth, bricks[i][j].y + brickHeight);
				}
			}

			SelectObject(hDC, hPen);
			DeleteObject(hBrush);

			ReleaseDC(hWnd, hDC);
			break;

		case WM_RBUTTONDOWN:
			hDC = GetDC(hWnd);

			SetROP2(hDC, R2_XORPEN);
			hPen = (HPEN)GetStockObject(BLACK_PEN);
			hBrush = CreateSolidBrush(RGB(0, 63, 255));
			oldPen = (HPEN)SelectObject(hDC, hPen);
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < bricks[i].size(); j++) {
					Rectangle(hDC, bricks[i][j].x, bricks[i][j].y, bricks[i][j].x + brickWidth, bricks[i][j].y + brickHeight);
					bricks[i][j].x += 10;
				}
			}

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < bricks[i].size(); j++) {
					Rectangle(hDC, bricks[i][j].x, bricks[i][j].y, bricks[i][j].x + brickWidth, bricks[i][j].y + brickHeight);
				}
			}

			SelectObject(hDC, hPen);
			DeleteObject(hBrush);

			ReleaseDC(hWnd, hDC);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}