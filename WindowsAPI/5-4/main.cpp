#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include "resource.h"

using namespace std;

enum PlayerStatus {
	Idle,
	Up,
	Down,
	Left,
	Right,
	ClickMove,
	IdleJump,
	UpJump,
	DownJump,
	LeftJump,
	RightJump
};

void CALLBACK RollbackSpriteSize(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void PlayerAnimation(HDC hDC);

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

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

HDC hDC, mDC;
RECT rt;
BITMAP bmp;
static HBITMAP playerBitmap, bgBitmap;
static PlayerStatus playerStatus;
static POINT mousePoint;
static POINT playerPos;
static vector<POINT> twinPos;
static int playerMoveSpeed;
static int playerAnimRefreshSpeed;
static int playerAniIndex;
static vector<int> twinAniIndex;
static int spriteSize;
static int selectedTwin;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	srand((unsigned int)time(NULL));
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;

	static int drawMode;
	static bool isDrag;
	static bool isMag;

	switch (uMsg) {
	case WM_CREATE:
		GetClientRect(hWnd, &rt);

		playerBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		bgBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		drawMode = SRCCOPY;

		isDrag = false;
		isMag = false;
		playerPos = { rt.right / 2, rt.bottom / 2 };
		playerAniIndex = 0;
		playerMoveSpeed = 18;
		playerAnimRefreshSpeed = 75;
		spriteSize = 2;
		selectedTwin = -1;

		SetTimer(hWnd, 1000, playerAnimRefreshSpeed, NULL);
		break;

	case WM_PAINT: {
		GetClientRect(hWnd, &rt);
		hDC = BeginPaint(hWnd, &ps);

		PlayerAnimation(hDC);

		EndPaint(hWnd, &ps);
		break;
	}

	case WM_TIMER:
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_KEYDOWN: {
		if (wParam == VK_UP) {
			if (playerStatus != Idle) {
				break;
			}
			playerAniIndex = 0;
			playerStatus = Up;
			KillTimer(hWnd, 1000);
			SetTimer(hWnd, 1000, playerAnimRefreshSpeed, NULL);
		}
		else if (wParam == VK_DOWN) {
			if (playerStatus != Idle) {
				break;
			}
			playerAniIndex = 0;
			playerStatus = Down;
			KillTimer(hWnd, 1000);
			SetTimer(hWnd, 1000, playerAnimRefreshSpeed, NULL);
		}
		else if (wParam == VK_LEFT) {
			if (playerStatus != Idle) {
				break;
			}
			playerAniIndex = 0;
			playerStatus = Left;
			KillTimer(hWnd, 1000);
			SetTimer(hWnd, 1000, playerAnimRefreshSpeed, NULL);
		}
		else if (wParam == VK_RIGHT) {
			if (playerStatus != Idle) {
				break;
			}
			playerAniIndex = 0;
			playerStatus = Right;
			KillTimer(hWnd, 1000);
			SetTimer(hWnd, 1000, playerAnimRefreshSpeed, NULL);
		}
		break;
	}

	case WM_KEYUP: {
		if (wParam == VK_UP) {
			if (playerStatus != Up) {
				break;
			}
			playerAniIndex = 0;
			playerStatus = Idle;
			KillTimer(hWnd, 1000);
			SetTimer(hWnd, 1000, playerAnimRefreshSpeed, NULL);
		}
		else if (wParam == VK_DOWN) {
			if (playerStatus != Down) {
				break;
			}
			playerAniIndex = 0;
			playerStatus = Idle;
			KillTimer(hWnd, 1000);
			SetTimer(hWnd, 1000, playerAnimRefreshSpeed, NULL);
		}
		else if (wParam == VK_LEFT) {
			if (playerStatus != Left) {
				break;
			}
			playerAniIndex = 0;
			playerStatus = Idle;
			KillTimer(hWnd, 1000);
			SetTimer(hWnd, 1000, playerAnimRefreshSpeed, NULL);
		}
		else if (wParam == VK_RIGHT) {
			if (playerStatus != Right) {
				break;
			}
			playerAniIndex = 0;
			playerStatus = Idle;
			KillTimer(hWnd, 1000);
			SetTimer(hWnd, 1000, playerAnimRefreshSpeed, NULL);
		}
		break;
	}

	case WM_LBUTTONDOWN:
		mousePoint = { LOWORD(lParam), HIWORD(lParam) };
		if (mousePoint.x >= playerPos.x && mousePoint.x <= playerPos.x + 25 * spriteSize &&
			mousePoint.y >= playerPos.y && mousePoint.y <= playerPos.y + 25 * spriteSize) {
			playerAniIndex = 0;
			playerStatus = IdleJump;
		}
		break;

	case WM_RBUTTONDOWN:
		mousePoint = { LOWORD(lParam), HIWORD(lParam) };
		playerStatus = ClickMove;
		break;

	case WM_MOUSEMOVE:
		break;

	case WM_CHAR:
		if (wParam == 'j') {
			switch (playerStatus) {
				case Idle:
					playerAniIndex = 0;
					playerStatus = IdleJump;
					break;
				case Up:
					playerAniIndex = 0;
					playerStatus = UpJump;
					break;
				case Down:
					playerAniIndex = 0;
					playerStatus = DownJump;
					break;
				case Left:
					playerAniIndex = 0;
					playerStatus = LeftJump;
					break;
				case Right:
					playerAniIndex = 0;
					playerStatus = RightJump;
					break;
			}
		}
		else if (wParam == 'e') {
			spriteSize = 3;
			SetTimer(hWnd, 1001, 2000, RollbackSpriteSize);
		}
		else if (wParam == 's') {
			spriteSize = 1;
			SetTimer(hWnd, 1001, 2000, RollbackSpriteSize);
		}
		else if (wParam == 't') {
			if (twinPos.size() >= 5) {
				twinPos.erase(twinPos.begin());
				twinAniIndex.erase(twinAniIndex.begin());
				selectedTwin -= selectedTwin >= 0 ? 1 : 0;
			}
			twinPos.push_back({rand() % (rt.right - 50), rand() % (rt.bottom - 50)});
			twinAniIndex.push_back(0);
		}
		else if (wParam >= 48 && wParam <= 52) {
			if (wParam - 48 < twinPos.size())
				selectedTwin = wParam - 48;
		}
		else if (wParam == 'q') {
			DeleteObject(bgBitmap);
			DeleteObject(playerBitmap);
			PostQuitMessage(0);
		}
		break;

	case WM_DESTROY:
		DeleteObject(bgBitmap);
		DeleteObject(playerBitmap);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CALLBACK RollbackSpriteSize(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
	spriteSize = 2;
	KillTimer(hWnd, 1001);
}

void PlayerAnimation(HDC hDC) {
	mDC = CreateCompatibleDC(hDC);

	SelectObject(mDC, bgBitmap);
	GetObject(bgBitmap, sizeof(BITMAP), &bmp);
	StretchBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	SelectObject(mDC, playerBitmap);
	GetObject(playerBitmap, sizeof(BITMAP), &bmp);
	switch (playerStatus) {
		case Idle:
			if (playerPos.y >= rt.bottom - 25 * spriteSize) {
				playerPos.y = rt.bottom - 25 * spriteSize;
			}
			if (playerPos.x >= rt.right - 25 * spriteSize) {
				playerPos.x = rt.right - 25 * spriteSize;
			}
			TransparentBlt(hDC, playerPos.x, playerPos.y, 25 * spriteSize, 25 * spriteSize, mDC, 0 + (playerAniIndex * 25), 0, 25, 25, RGB(144, 187, 187));
			break;

		case Up:
			playerPos.y -= playerMoveSpeed;
			if (playerPos.y <= 0) {
				playerPos.y = 0;
			}
			TransparentBlt(hDC, playerPos.x, playerPos.y, 25 * spriteSize, 25 * spriteSize, mDC, (playerAniIndex * 25), 150, 25, 25, RGB(144, 187, 187));
			break;

		case Down:
			playerPos.y += playerMoveSpeed;
			if (playerPos.y >= rt.bottom - 25 * spriteSize) {
				playerPos.y = rt.bottom - 25 * spriteSize;
			}
			TransparentBlt(hDC, playerPos.x, playerPos.y, 25 * spriteSize, 25 * spriteSize, mDC, (playerAniIndex * 25), 100, 25, 25, RGB(144, 187, 187));
			break;

		case Left:
			playerPos.x -= playerMoveSpeed;
			if (playerPos.x <= 0) {
				playerPos.x = 0;
			}
			StretchBlt(mDC, 25 + (playerAniIndex * 25), 125, -25, 25, mDC, (playerAniIndex * 25), 125, 25, 25, SRCCOPY);
			TransparentBlt(hDC, playerPos.x, playerPos.y, 25 * spriteSize, 25 * spriteSize, mDC, (playerAniIndex * 25), 125, 25, 25, RGB(144, 187, 187));
			StretchBlt(mDC, 25 + (playerAniIndex * 25), 125, -25, 25, mDC, (playerAniIndex * 25), 125, 25, 25, SRCCOPY);
			break;

		case Right:
			playerPos.x += playerMoveSpeed;
			if (playerPos.x >= rt.right - 25 * spriteSize) {
				playerPos.x = rt.right - 25 * spriteSize;
			}
			TransparentBlt(hDC, playerPos.x, playerPos.y, 25 * spriteSize, 25 * spriteSize, mDC, (playerAniIndex * 25), 125, 25, 25, RGB(144, 187, 187));
			break;

		case ClickMove:
			if (playerPos.x < mousePoint.x) {
				if (mousePoint.x - playerPos.x < playerMoveSpeed) {
					playerPos.x = mousePoint.x;
				}
				else {
					playerPos.x += playerMoveSpeed;
				}
			}
			else if (playerPos.x > mousePoint.x) {
				if (playerPos.x - mousePoint.x < playerMoveSpeed) {
					playerPos.x = mousePoint.x;
				}
				else {
					playerPos.x -= playerMoveSpeed;
				}
			}
			if (playerPos.y < mousePoint.y) {
				if (mousePoint.y - playerPos.y < playerMoveSpeed) {
					playerPos.y = mousePoint.y;
				}
				else {
					playerPos.y += playerMoveSpeed;
				}
			}
			else if (playerPos.y > mousePoint.y) {
				if (playerPos.y - mousePoint.y < playerMoveSpeed) {
					playerPos.y = mousePoint.y;
				}
				else {
					playerPos.y -= playerMoveSpeed;
				}
			}
			if (playerPos.x >= rt.right - 25 * spriteSize) {
				playerPos.x = rt.right - 25 * spriteSize;
			}
			if (playerPos.y >= rt.bottom - 25 * spriteSize) {
				playerPos.y = rt.bottom - 25 * spriteSize;
			}
			if (playerPos.x < mousePoint.x) {
				TransparentBlt(hDC, playerPos.x, playerPos.y, 25 * spriteSize, 25 * spriteSize, mDC, (playerAniIndex * 25), 125, 25, 25, RGB(144, 187, 187));
			}
			else {
				StretchBlt(mDC, 25 + (playerAniIndex * 25), 125, -25, 25, mDC, (playerAniIndex * 25), 125, 25, 25, SRCCOPY);
				TransparentBlt(hDC, playerPos.x, playerPos.y, 25 * spriteSize, 25 * spriteSize, mDC, (playerAniIndex * 25), 125, 25, 25, RGB(144, 187, 187));
				StretchBlt(mDC, 25 + (playerAniIndex * 25), 125, -25, 25, mDC, (playerAniIndex * 25), 125, 25, 25, SRCCOPY);
			}
			if (playerPos.x == mousePoint.x && playerPos.y == mousePoint.y) {
				playerStatus = Idle;
			}
			break;

		case IdleJump:
			if (playerAniIndex < 3) {
				playerPos.y -= 15;
			}
			else {
				playerPos.y += 15;
			}
			TransparentBlt(hDC, playerPos.x, playerPos.y, 25 * spriteSize, 25 * spriteSize, mDC, (playerAniIndex * 25), 175, 25, 25, RGB(144, 187, 187));
			if (playerAniIndex >= 5) {
				playerAniIndex = 0;
				playerStatus = Idle;
			}
			break;

		case UpJump:
			playerPos.y -= playerMoveSpeed;
			if (playerPos.y <= 0) {
				playerPos.y = 0;
			}
			if (playerAniIndex < 3) {
				playerPos.y -= 15;
			}
			else {
				playerPos.y += 15;
			}
			TransparentBlt(hDC, playerPos.x, playerPos.y, 25 * spriteSize, 25 * spriteSize, mDC, (playerAniIndex * 25), 175, 25, 25, RGB(144, 187, 187));
			if (playerAniIndex >= 5) {
				playerAniIndex = 0;
				playerStatus = Idle;
			}
			break;

		case DownJump:
			playerPos.y += playerMoveSpeed;
			if (playerPos.y >= rt.bottom - 25 * spriteSize) {
				playerPos.y = rt.bottom - 25 * spriteSize;
			}
			if (playerAniIndex < 3) {
				playerPos.y -= 15;
			}
			else {
				playerPos.y += 15;
			}
			TransparentBlt(hDC, playerPos.x, playerPos.y, 25 * spriteSize, 25 * spriteSize, mDC, (playerAniIndex * 25), 175, 25, 25, RGB(144, 187, 187));
			if (playerAniIndex >= 5) {
				playerAniIndex = 0;
				playerStatus = Idle;
			}
			break;

		case LeftJump:
			playerPos.x -= playerMoveSpeed;
			if (playerPos.x <= 0) {
				playerPos.x = 0;
			}
			if (playerAniIndex < 3) {
				playerPos.y -= 15;
			}
			else {
				playerPos.y += 15;
			}
			StretchBlt(mDC, 25 + (playerAniIndex * 25), 175, -25, 25, mDC, (playerAniIndex * 25), 175, 25, 25, SRCCOPY);
			TransparentBlt(hDC, playerPos.x, playerPos.y, 25 * spriteSize, 25 * spriteSize, mDC, (playerAniIndex * 25), 175, 25, 25, RGB(144, 187, 187));
			StretchBlt(mDC, 25 + (playerAniIndex * 25), 175, -25, 25, mDC, (playerAniIndex * 25), 175, 25, 25, SRCCOPY);
			if (playerAniIndex >= 5) {
				playerAniIndex = 0;
				playerStatus = Idle;
			}
			break;

		case RightJump:
			playerPos.x += playerMoveSpeed;
			if (playerPos.x >= rt.right - 25 * spriteSize) {
				playerPos.x = rt.right - 25 * spriteSize;
			}
			if (playerAniIndex < 3) {
				playerPos.y -= 15;
			}
			else {
				playerPos.y += 15;
			}
			TransparentBlt(hDC, playerPos.x, playerPos.y, 25 * spriteSize, 25 * spriteSize, mDC, (playerAniIndex * 25), 175, 25, 25, RGB(144, 187, 187));
			if (playerAniIndex >= 5) {
				playerAniIndex = 0;
				playerStatus = Idle;
			}
			break;
	}
	playerAniIndex = (playerAniIndex + 1) % 6;

	for (int i = 0; i < twinPos.size(); i++) {
		if (selectedTwin == i) {

			HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
			HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
			HPEN oldPen = (HPEN)SelectObject(hDC, hPen);

			twinAniIndex[i] = playerAniIndex;
			switch (playerStatus) {
				case Idle:
					if (twinPos[i].x >= rt.right - 25 * spriteSize) {
						twinPos[i].x = rt.right - 25 * spriteSize;
					}
					if (twinPos[i].y >= rt.bottom - 25 * spriteSize) {
						twinPos[i].y = rt.bottom - 25 * spriteSize;
					}
					Rectangle(hDC, twinPos[i].x, twinPos[i].y, twinPos[i].x + 25 * spriteSize, twinPos[i].y + 25 * spriteSize);
					TransparentBlt(hDC, twinPos[i].x, twinPos[i].y, 25 * spriteSize, 25 * spriteSize, mDC, 0 + (twinAniIndex[i] * 25), 0, 25, 25, RGB(144, 187, 187));
					break;

				case Up:
					twinPos[i].y -= playerMoveSpeed;
					if (twinPos[i].y <= 0) {
						twinPos[i].y = 0;
					}
					Rectangle(hDC, twinPos[i].x, twinPos[i].y, twinPos[i].x + 25 * spriteSize, twinPos[i].y + 25 * spriteSize);
					TransparentBlt(hDC, twinPos[i].x, twinPos[i].y, 25 * spriteSize, 25 * spriteSize, mDC, (twinAniIndex[i] * 25), 150, 25, 25, RGB(144, 187, 187));
					break;

				case Down:
					twinPos[i].y += playerMoveSpeed;
					if (twinPos[i].y >= rt.bottom - 25 * spriteSize) {
						twinPos[i].y = rt.bottom - 25 * spriteSize;
					}
					Rectangle(hDC, twinPos[i].x, twinPos[i].y, twinPos[i].x + 25 * spriteSize, twinPos[i].y + 25 * spriteSize);
					TransparentBlt(hDC, twinPos[i].x, twinPos[i].y, 25 * spriteSize, 25 * spriteSize, mDC, (twinAniIndex[i] * 25), 100, 25, 25, RGB(144, 187, 187));
					break;

				case Left:
					twinPos[i].x -= playerMoveSpeed;
					if (twinPos[i].x <= 0) {
						twinPos[i].x = 0;
					}
					Rectangle(hDC, twinPos[i].x, twinPos[i].y, twinPos[i].x + 25 * spriteSize, twinPos[i].y + 25 * spriteSize);
					StretchBlt(mDC, 25 + (twinAniIndex[i] * 25), 125, -25, 25, mDC, (twinAniIndex[i] * 25), 125, 25, 25, SRCCOPY);
					TransparentBlt(hDC, twinPos[i].x, twinPos[i].y, 25 * spriteSize, 25 * spriteSize, mDC, (twinAniIndex[i] * 25), 125, 25, 25, RGB(144, 187, 187));
					StretchBlt(mDC, 25 + (twinAniIndex[i] * 25), 125, -25, 25, mDC, (twinAniIndex[i] * 25), 125, 25, 25, SRCCOPY);
					break;

				case Right:
					twinPos[i].x += playerMoveSpeed;
					if (twinPos[i].x >= rt.right - 25 * spriteSize) {
						twinPos[i].x = rt.right - 25 * spriteSize;
					}
					Rectangle(hDC, twinPos[i].x, twinPos[i].y, twinPos[i].x + 25 * spriteSize, twinPos[i].y + 25 * spriteSize);
					TransparentBlt(hDC, twinPos[i].x, twinPos[i].y, 25 * spriteSize, 25 * spriteSize, mDC, (twinAniIndex[i] * 25), 125, 25, 25, RGB(144, 187, 187));
					break;

				case ClickMove:
					if (twinPos[i].x < mousePoint.x) {
						if (mousePoint.x - twinPos[i].x < playerMoveSpeed) {
							twinPos[i].x = mousePoint.x;
						}
						else {
							twinPos[i].x += playerMoveSpeed;
						}
					}
					else if (twinPos[i].x > mousePoint.x) {
						if (twinPos[i].x - mousePoint.x < playerMoveSpeed) {
							twinPos[i].x = mousePoint.x;
						}
						else {
							twinPos[i].x -= playerMoveSpeed;
						}
					}
					if (twinPos[i].y < mousePoint.y) {
						if (mousePoint.y - twinPos[i].y < playerMoveSpeed) {
							twinPos[i].y = mousePoint.y;
						}
						else {
							twinPos[i].y += playerMoveSpeed;
						}
					}
					else if (twinPos[i].y > mousePoint.y) {
						if (twinPos[i].y - mousePoint.y < playerMoveSpeed) {
							twinPos[i].y = mousePoint.y;
						}
						else {
							twinPos[i].y -= playerMoveSpeed;
						}
					}
					if (twinPos[i].x >= rt.right - 25 * spriteSize) {
						twinPos[i].x = rt.right - 25 * spriteSize;
					}
					if (twinPos[i].y >= rt.bottom - 25 * spriteSize) {
						twinPos[i].y = rt.bottom - 25 * spriteSize;
					}
					if (twinPos[i].x < mousePoint.x) {
						TransparentBlt(hDC, twinPos[i].x, twinPos[i].y, 25 * spriteSize, 25 * spriteSize, mDC, (twinAniIndex[i] * 25), 125, 25, 25, RGB(144, 187, 187));
					}
					else {
						StretchBlt(mDC, 25 + (twinAniIndex[i] * 25), 125, -25, 25, mDC, (twinAniIndex[i] * 25), 125, 25, 25, SRCCOPY);
						TransparentBlt(hDC, twinPos[i].x, twinPos[i].y, 25 * spriteSize, 25 * spriteSize, mDC, (twinAniIndex[i] * 25), 125, 25, 25, RGB(144, 187, 187));
						StretchBlt(mDC, 25 + (twinAniIndex[i] * 25), 125, -25, 25, mDC, (twinAniIndex[i] * 25), 125, 25, 25, SRCCOPY);
					}
					if (twinPos[i].x == mousePoint.x && twinPos[i].y == mousePoint.y) {
						playerStatus = Idle;
					}
					break;

				case IdleJump:
					if (twinAniIndex[i] < 3) {
						twinPos[i].y -= 15;
					}
					else {
						twinPos[i].y += 15;
					}
					Rectangle(hDC, twinPos[i].x, twinPos[i].y, twinPos[i].x + 25 * spriteSize, twinPos[i].y + 25 * spriteSize);
					TransparentBlt(hDC, twinPos[i].x, twinPos[i].y, 25 * spriteSize, 25 * spriteSize, mDC, (twinAniIndex[i] * 25), 175, 25, 25, RGB(144, 187, 187));
					if (twinAniIndex[i] >= 5) {
						twinAniIndex[i] = 0;
					}
					break;

				case UpJump:
					twinPos[i].y -= playerMoveSpeed;
					if (twinPos[i].y <= 0) {
						twinPos[i].y = 0;
					}
					if (twinAniIndex[i] < 3) {
						twinPos[i].y -= 15;
					}
					else {
						twinPos[i].y += 15;
					}
					Rectangle(hDC, twinPos[i].x, twinPos[i].y, twinPos[i].x + 25 * spriteSize, twinPos[i].y + 25 * spriteSize);
					TransparentBlt(hDC, twinPos[i].x, twinPos[i].y, 25 * spriteSize, 25 * spriteSize, mDC, (twinAniIndex[i] * 25), 175, 25, 25, RGB(144, 187, 187));
					if (twinAniIndex[i] >= 5) {
						twinAniIndex[i] = 0;
					}
					break;

				case DownJump:
					twinPos[i].y += playerMoveSpeed;
					if (twinPos[i].y >= rt.bottom - 25 * spriteSize) {
						twinPos[i].y = rt.bottom - 25 * spriteSize;
					}
					if (twinAniIndex[i] < 3) {
						twinPos[i].y -= 15;
					}
					else {
						twinPos[i].y += 15;
					}
					Rectangle(hDC, twinPos[i].x, twinPos[i].y, twinPos[i].x + 25 * spriteSize, twinPos[i].y + 25 * spriteSize);
					TransparentBlt(hDC, twinPos[i].x, twinPos[i].y, 25 * spriteSize, 25 * spriteSize, mDC, (twinAniIndex[i] * 25), 175, 25, 25, RGB(144, 187, 187));
					if (twinAniIndex[i] >= 5) {
						twinAniIndex[i] = 0;
					}
					break;

				case LeftJump:
					twinPos[i].x -= playerMoveSpeed;
					if (twinPos[i].x <= 0) {
						twinPos[i].x = 0;
					}
					if (twinAniIndex[i] < 3) {
						twinPos[i].y -= 15;
					}
					else {
						twinPos[i].y += 15;
					}
					Rectangle(hDC, twinPos[i].x, twinPos[i].y, twinPos[i].x + 25 * spriteSize, twinPos[i].y + 25 * spriteSize);
					StretchBlt(mDC, 25 + (twinAniIndex[i] * 25), 175, -25, 25, mDC, (twinAniIndex[i] * 25), 175, 25, 25, SRCCOPY);
					TransparentBlt(hDC, twinPos[i].x, twinPos[i].y, 25 * spriteSize, 25 * spriteSize, mDC, (twinAniIndex[i] * 25), 175, 25, 25, RGB(144, 187, 187));
					StretchBlt(mDC, 25 + (twinAniIndex[i] * 25), 175, -25, 25, mDC, (twinAniIndex[i] * 25), 175, 25, 25, SRCCOPY);
					if (twinAniIndex[i] >= 5) {
						twinAniIndex[i] = 0;
					}
					break;

				case RightJump:
					twinPos[i].x += playerMoveSpeed;
					if (twinPos[i].x >= rt.right - 25 * spriteSize) {
						twinPos[i].x = rt.right - 25 * spriteSize;
					}
					if (twinAniIndex[i] < 3) {
						twinPos[i].y -= 15;
					}
					else {
						twinPos[i].y += 15;
					}
					Rectangle(hDC, twinPos[i].x, twinPos[i].y, twinPos[i].x + 25 * spriteSize, twinPos[i].y + 25 * spriteSize);
					TransparentBlt(hDC, twinPos[i].x, twinPos[i].y, 25 * spriteSize, 25 * spriteSize, mDC, (twinAniIndex[i] * 25), 175, 25, 25, RGB(144, 187, 187));
					if (twinAniIndex[i] >= 5) {
						twinAniIndex[i] = 0;
					}
					break;
			}
			SelectObject(hDC, oldBrush);
			SelectObject(hDC, oldPen);
			DeleteObject(hPen);
		}
		else {
			if (twinPos[i].y >= rt.bottom - 25 * 2) {
				twinPos[i].y = rt.bottom - 25 * 2;
			}
			if (twinPos[i].x >= rt.right - 25 * 2) {
				twinPos[i].x = rt.right - 25 * 2;
			}
			TransparentBlt(hDC, twinPos[i].x, twinPos[i].y, 25 * 2, 25 * 2, mDC, 0 + (twinAniIndex[i] * 25), 0, 25, 25, RGB(144, 187, 187));
		}
		twinAniIndex[i] = (twinAniIndex[i] + 1) % 6;
	}

	DeleteDC(mDC);
}