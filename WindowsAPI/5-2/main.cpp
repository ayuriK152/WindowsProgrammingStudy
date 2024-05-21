#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include "resource.h"

using namespace std;

enum Direction {
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

enum Mode {
	NORMAL,
	VERTICAL,
	HORIZONTAL
};

void CALLBACK MovePiece(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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

BITMAP bmp;
static HBITMAP hBitmap;
static POINT piecePoints[25];
static Direction moveDir;
static int pieceNum[5][5];
static int divideCnt;
static int targetPiece;
static POINT moveOffset;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	srand((unsigned int)time(NULL));
	PAINTSTRUCT ps;
	HDC hDC, mDC;
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;
	RECT rt;
	static POINT mousePoint;
	static Mode dragMode;
	static int drawMode;
	static bool isStarted;
	static bool isWatchingEntire;
	static bool isDrag;
	static bool isMoving;
	static bool isClear;

	switch (uMsg) {
	case WM_CREATE:
		GetClientRect(hWnd, &rt);
		isStarted = false;
		isWatchingEntire = true;
		isDrag = false;
		isMoving = false;
		isClear = false;
		moveOffset.x = 0;
		moveOffset.y = 0;
		targetPiece = -1;
		moveDir = NONE;
		dragMode = NORMAL;
		drawMode = SRCCOPY;
		hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		divideCnt = 3;
		for (int y = 0; y < 5; y++) {
			for (int x = 0; x < 5; x++) {
				pieceNum[x][y] = -1;
			}
		}
		for (int i = 0; i < 8; i++) {
			int tempX = rand() % divideCnt;
			int tempY = rand() % divideCnt;
			if (pieceNum[tempX][tempY] != -1) {
				i--;
				continue;
			}
			pieceNum[tempX][tempY] = i;
		}
		for (int y = 0; y < divideCnt; y++) {
			for (int x = 0; x < divideCnt; x++) {
				piecePoints[x + y * divideCnt].x = bmp.bmWidth * x / divideCnt;
				piecePoints[x + y * divideCnt].y = bmp.bmHeight * y / divideCnt;
			}
		}
		break;

	case WM_PAINT: {
		GetClientRect(hWnd, &rt);
		hDC = BeginPaint(hWnd, &ps);
		mDC = CreateCompatibleDC(hDC);
		SelectObject(mDC, hBitmap);
		GetObject(hBitmap, sizeof(BITMAP), &bmp);

		if (abs(moveOffset.x) >= rt.right / divideCnt || abs(moveOffset.y) >= rt.bottom / divideCnt) {
			KillTimer(hWnd, 0);
			moveOffset.x = 0;
			moveOffset.y = 0;
			bool isChanged = false;
			for (int y = 0; y < divideCnt; y++) {
				for (int x = 0; x < divideCnt; x++) {
					if (pieceNum[x][y] == -1) {
						pieceNum[x][y] = targetPiece;
						switch (moveDir) {
						case UP:
							pieceNum[x][y + 1] = -1;
							break;
						case DOWN:
							pieceNum[x][y - 1] = -1;
							break;
						case LEFT:
							pieceNum[x + 1][y] = -1;
							break;
						case RIGHT:
							pieceNum[x - 1][y] = -1;
							break;
						}
						moveDir = NONE;
						targetPiece = -1;
						isChanged = true;
						isMoving = false;
						break;
					}
				}
				if (isChanged)
					break;
			}
		}

		int tempNum = 0;
		isClear = true;
		for (int y = 0; y < 5; y++) {
			for (int x = 0; x < 5; x++) {
				if (pieceNum[x][y] == -1)
					continue;
				if (pieceNum[x][y] != tempNum)
					isClear = false;
				tempNum++;
			}
		}

		if (isWatchingEntire) {
			StretchBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, bmp.bmWidth, bmp.bmHeight, drawMode);
		}
		else {
			for (int y = 0; y < divideCnt; y++) {
				for (int x = 0; x < divideCnt; x++) {
					if (pieceNum[x][y] == -1)
						continue;
					if (targetPiece != pieceNum[x][y]) {
						StretchBlt(hDC, x * rt.right / divideCnt, y * rt.bottom / divideCnt, rt.right / divideCnt, rt.bottom / divideCnt, mDC, piecePoints[pieceNum[x][y]].x, piecePoints[pieceNum[x][y]].y, bmp.bmWidth / divideCnt, bmp.bmHeight / divideCnt, drawMode);
					}
					else {
						StretchBlt(hDC, x * rt.right / divideCnt, y * rt.bottom / divideCnt, rt.right / divideCnt, rt.bottom / divideCnt, mDC, piecePoints[pieceNum[x][y]].x, piecePoints[pieceNum[x][y]].y, bmp.bmWidth / divideCnt, bmp.bmHeight / divideCnt, WHITENESS);
						StretchBlt(hDC, x * rt.right / divideCnt + moveOffset.x, y * rt.bottom / divideCnt + moveOffset.y, rt.right / divideCnt, rt.bottom / divideCnt, mDC, piecePoints[pieceNum[x][y]].x, piecePoints[pieceNum[x][y]].y, bmp.bmWidth / divideCnt, bmp.bmHeight / divideCnt, drawMode);
					}
				}
			}
		}

		if (dragMode == VERTICAL) {
			hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(hDC, hPen);
			hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			for (int x = 0; x < divideCnt; x++) {
				Rectangle(hDC, x * rt.right / divideCnt, 0, (x + 1) * rt.right / divideCnt, rt.bottom);
			}
			SelectObject(hDC, oldPen);
			DeleteObject(hPen);
			SelectObject(hDC, oldBrush);
		}
		else if (dragMode == HORIZONTAL) {
			hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(hDC, hPen);
			hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			for (int y = 0; y < divideCnt; y++) {
				Rectangle(hDC, 0, y * rt.bottom / divideCnt, rt.right, (y + 1) * rt.bottom / divideCnt);
			}
			SelectObject(hDC, oldPen);
			DeleteObject(hPen);
			SelectObject(hDC, oldBrush);
		}

		if (isClear) {
			RECT temp;
			temp.left = rt.right / 2 - 100;
			temp.top = rt.bottom / 2 - 20;
			temp.right = rt.right / 2 + 100;
			temp.bottom = rt.bottom / 2 + 20;
			DrawText(hDC, L"GAME CLEAR!!", lstrlen(L"GAME CLEAR!!"), &temp, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
		}

		DeleteDC(mDC);
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_TIMER:
		hDC = GetDC(hWnd);

		ReleaseDC(hWnd, hDC);
		break;

	case WM_KEYDOWN:
		GetClientRect(hWnd, &rt);

		break;

	case WM_LBUTTONDOWN:
		if (!isStarted)
			break;
		mousePoint.x = LOWORD(lParam);
		mousePoint.y = HIWORD(lParam);
		isDrag = true;

		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_LBUTTONUP: {
		isDrag = false;
		if (dragMode == NORMAL)
			break;
		GetClientRect(hWnd, &rt);
		POINT legacy;
		legacy.x = mousePoint.x;
		legacy.y = mousePoint.y;
		mousePoint.x = LOWORD(lParam);
		mousePoint.y = HIWORD(lParam);

		int legacyArea = -1, currentArea = -1;
		if (dragMode == VERTICAL) {
			for (int x = 0; x < divideCnt; x++) {
				if (x * rt.right / divideCnt <= legacy.x && (x + 1) * rt.right / divideCnt >= legacy.x) {
					legacyArea = x;
					break;
				}
			}
			for (int x = 0; x < divideCnt; x++) {
				if (x * rt.right / divideCnt <= mousePoint.x && (x + 1) * rt.right / divideCnt >= mousePoint.x) {
					currentArea = x;
					break;
				}
			}
		}
		else if (dragMode == HORIZONTAL) {
			for (int y = 0; y < divideCnt; y++) {
				if (y * rt.bottom / divideCnt <= legacy.y && (y + 1) * rt.bottom / divideCnt >= legacy.y) {
					legacyArea = y;
					break;
				}
			}
			for (int y = 0; y < divideCnt; y++) {
				if (y * rt.bottom / divideCnt <= mousePoint.y && (y + 1) * rt.bottom / divideCnt >= mousePoint.y) {
					currentArea = y;
					break;
				}
			}
		}
		if (legacyArea != currentArea && legacyArea != -1 && currentArea != -1) {
			if (dragMode == VERTICAL) {
				for (int y = 0; y < divideCnt; y++) {
					int temp = pieceNum[legacyArea][y];
					pieceNum[legacyArea][y] = pieceNum[currentArea][y];
					pieceNum[currentArea][y] = temp;
				}
			}
			else if (dragMode == HORIZONTAL) {
				for (int x = 0; x < divideCnt; x++) {
					int temp = pieceNum[x][legacyArea];
					pieceNum[x][legacyArea] = pieceNum[x][currentArea];
					pieceNum[x][currentArea] = temp;
				}
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
		
	case WM_MOUSEMOVE:
		if (isMoving || isClear)
			break;
		if (isDrag) {
			if (dragMode == NORMAL) {
				moveDir = abs(mousePoint.x - LOWORD(lParam)) >= abs(mousePoint.y - HIWORD(lParam)) ? (mousePoint.x - LOWORD(lParam) < 0 ? RIGHT : LEFT) : (mousePoint.y - HIWORD(lParam) < 0 ? DOWN : UP);
				for (int y = 0; y < divideCnt; y++) {
					for (int x = 0; x < divideCnt; x++) {
						if (pieceNum[x][y] == -1) {
							switch (moveDir) {
							case UP:
								if (y < divideCnt - 1) {
									targetPiece = pieceNum[x][y + 1];
									isMoving = true;
									SetTimer(hWnd, 0, 5, MovePiece);
								}
								break;
							case DOWN:
								if (y > 0) {
									targetPiece = pieceNum[x][y - 1];
									isMoving = true;
									SetTimer(hWnd, 0, 5, MovePiece);
								}
								break;
							case LEFT:
								if (x < divideCnt - 1) {
									targetPiece = pieceNum[x + 1][y];
									isMoving = true;
									SetTimer(hWnd, 0, 5, MovePiece);
								}
								break;
							case RIGHT:
								if (x > 0) {
									targetPiece = pieceNum[x - 1][y];
									isMoving = true;
									SetTimer(hWnd, 0, 5, MovePiece);
								}
								break;
							}
						}
					}
				}
			}
		}
		break;

	case WM_CHAR:
		if (wParam == 's') {
			if (isStarted)
				break;
			isStarted = true;
			isWatchingEntire = false;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'f') {
			isWatchingEntire = !isWatchingEntire;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'v') {
			if (dragMode == NORMAL) {
				dragMode = VERTICAL;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else {
				dragMode = NORMAL;
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}
		else if (wParam == 'h') {
			if (dragMode == NORMAL) {
				dragMode = HORIZONTAL;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else {
				dragMode = NORMAL;
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}
		else if (wParam == 'q') {
			DeleteObject(hBitmap);
			PostQuitMessage(0);
		}
		break;

	case WM_COMMAND: {
		int command = LOWORD(wParam);
		if (command == ID_GAME_START) {
			if (isStarted)
				break;
			isStarted = true;
			isWatchingEntire = false;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (command == ID_GAME_REVERSEPICTURE) {
			if (drawMode == SRCCOPY) {
				drawMode = NOTSRCCOPY;
			}
			else {
				drawMode = SRCCOPY;
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (command == ID_GAME_WATCHBASEPICTURE) {
			isWatchingEntire = true;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (command == ID_GAME_EXIT) {
			dragMode = NORMAL;
			for (int y = 0; y < divideCnt; y++) {
				for (int x = 0; x < divideCnt; x++) {
					pieceNum[x][y] = x + y * divideCnt;
				}
			}
			pieceNum[divideCnt - 1][divideCnt - 1] = -1;
			InvalidateRect(hWnd, NULL, TRUE);
		}

		else if (command == ID_DIVIDE_3) {
			divideCnt = 3;
			GetClientRect(hWnd, &rt);
			isStarted = false;
			isWatchingEntire = true;
			isDrag = false;
			isMoving = false;
			isClear = false;
			dragMode = NORMAL;
			moveOffset.x = 0;
			moveOffset.y = 0;
			targetPiece = -1;
			GetObject(hBitmap, sizeof(BITMAP), &bmp);
			for (int y = 0; y < 5; y++) {
				for (int x = 0; x < 5; x++) {
					pieceNum[x][y] = -1;
				}
			}
			for (int i = 0; i < 8; i++) {
				int tempX = rand() % divideCnt;
				int tempY = rand() % divideCnt;
				if (pieceNum[tempX][tempY] != -1) {
					i--;
					continue;
				}
				pieceNum[tempX][tempY] = i;
			}
			for (int y = 0; y < divideCnt; y++) {
				for (int x = 0; x < divideCnt; x++) {
					piecePoints[x + y * divideCnt].x = bmp.bmWidth * x / divideCnt;
					piecePoints[x + y * divideCnt].y = bmp.bmHeight * y / divideCnt;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (command == ID_DIVIDE_4) {
			divideCnt = 4;
			GetClientRect(hWnd, &rt);
			isStarted = false;
			isWatchingEntire = true;
			isDrag = false;
			isMoving = false;
			isClear = false;
			dragMode = NORMAL;
			moveOffset.x = 0;
			moveOffset.y = 0;
			targetPiece = -1;
			GetObject(hBitmap, sizeof(BITMAP), &bmp);
			for (int y = 0; y < 5; y++) {
				for (int x = 0; x < 5; x++) {
					pieceNum[x][y] = -1;
				}
			}
			for (int i = 0; i < 15; i++) {
				int tempX = rand() % divideCnt;
				int tempY = rand() % divideCnt;
				if (pieceNum[tempX][tempY] != -1) {
					i--;
					continue;
				}
				pieceNum[tempX][tempY] = i;
			}
			for (int y = 0; y < divideCnt; y++) {
				for (int x = 0; x < divideCnt; x++) {
					piecePoints[x + y * divideCnt].x = bmp.bmWidth * x / divideCnt;
					piecePoints[x + y * divideCnt].y = bmp.bmHeight * y / divideCnt;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (command == ID_DIVIDE_5) {
			divideCnt = 5;
			GetClientRect(hWnd, &rt);
			isStarted = false;
			isWatchingEntire = true;
			isDrag = false;
			isMoving = false;
			isClear = false;
			dragMode = NORMAL;
			moveOffset.x = 0;
			moveOffset.y = 0;
			targetPiece = -1;
			GetObject(hBitmap, sizeof(BITMAP), &bmp);
			for (int y = 0; y < 5; y++) {
				for (int x = 0; x < 5; x++) {
					pieceNum[x][y] = -1;
				}
			}
			for (int i = 0; i < 24; i++) {
				int tempX = rand() % divideCnt;
				int tempY = rand() % divideCnt;
				if (pieceNum[tempX][tempY] != -1) {
					i--;
					continue;
				}
				pieceNum[tempX][tempY] = i;
			}
			for (int y = 0; y < divideCnt; y++) {
				for (int x = 0; x < divideCnt; x++) {
					piecePoints[x + y * divideCnt].x = bmp.bmWidth * x / divideCnt;
					piecePoints[x + y * divideCnt].y = bmp.bmHeight * y / divideCnt;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}

		else if (command == ID_PICTURE_PIC1) {
			GetClientRect(hWnd, &rt);
			isStarted = false;
			isWatchingEntire = true;
			isDrag = false;
			isMoving = false;
			isClear = false;
			dragMode = NORMAL;
			moveOffset.x = 0;
			moveOffset.y = 0;
			targetPiece = -1;
			hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
			GetObject(hBitmap, sizeof(BITMAP), &bmp);
			divideCnt = 3;
			for (int y = 0; y < 5; y++) {
				for (int x = 0; x < 5; x++) {
					pieceNum[x][y] = -1;
				}
			}
			for (int i = 0; i < 8; i++) {
				int tempX = rand() % divideCnt;
				int tempY = rand() % divideCnt;
				if (pieceNum[tempX][tempY] != -1) {
					i--;
					continue;
				}
				pieceNum[tempX][tempY] = i;
			}
			for (int y = 0; y < divideCnt; y++) {
				for (int x = 0; x < divideCnt; x++) {
					piecePoints[x + y * divideCnt].x = bmp.bmWidth * x / divideCnt;
					piecePoints[x + y * divideCnt].y = bmp.bmHeight * y / divideCnt;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (command == ID_PICTURE_PIC2) {
			GetClientRect(hWnd, &rt);
			isStarted = false;
			isWatchingEntire = true;
			isDrag = false;
			isMoving = false;
			isClear = false;
			dragMode = NORMAL;
			moveOffset.x = 0;
			moveOffset.y = 0;
			targetPiece = -1;
			hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
			GetObject(hBitmap, sizeof(BITMAP), &bmp);
			divideCnt = 3;
			for (int y = 0; y < 5; y++) {
				for (int x = 0; x < 5; x++) {
					pieceNum[x][y] = -1;
				}
			}
			for (int i = 0; i < 8; i++) {
				int tempX = rand() % divideCnt;
				int tempY = rand() % divideCnt;
				if (pieceNum[tempX][tempY] != -1) {
					i--;
					continue;
				}
				pieceNum[tempX][tempY] = i;
			}
			for (int y = 0; y < divideCnt; y++) {
				for (int x = 0; x < divideCnt; x++) {
					piecePoints[x + y * divideCnt].x = bmp.bmWidth * x / divideCnt;
					piecePoints[x + y * divideCnt].y = bmp.bmHeight * y / divideCnt;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	}

	case WM_DESTROY:
		DeleteObject(hBitmap);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CALLBACK MovePiece(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
	switch (moveDir) {
	case UP:
		moveOffset.y -= 5;
		break;
	case DOWN:
		moveOffset.y += 5;
		break;
	case LEFT:
		moveOffset.x -= 5;
		break;
	case RIGHT:
		moveOffset.x += 5;
		break;
	}
	InvalidateRect(hWnd, NULL, FALSE);
}
