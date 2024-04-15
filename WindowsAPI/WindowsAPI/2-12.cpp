#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#define CELLSIZE 18

using namespace std;

struct Shape
{
	int x, y;
	int size;
	int shapeIdx;
	int colorIdx;
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

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 1000, 1000, NULL, (HMENU)NULL, hInstance, NULL);
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
	static Shape board[50][50];
	static int boardSize, rectLen;
	static vector<Shape> shapeQueue;
	static Shape selectedShape;
	static int selectedIdx;
	static COLORREF baseColors[4] = { RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255), RGB(255, 255, 0) };
	static bool isChanged;

	switch (uMsg) {
	case WM_CREATE:
		for (int y = 0; y < 50; y++)
		{
			for (int x = 0; x < 50; x++)
			{
				board[y][x] = { x, y, 0, -1, -1 };
			}
		}
		boardSize = 40;
		rectLen = 900 / boardSize;
		selectedShape = { -1, -1, 0, -1, -1 };
		selectedIdx = -1;
		isChanged = false;
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		if (shapeQueue.size() > 10)
		{
			board[shapeQueue[0].y][shapeQueue[0].x] = { shapeQueue[0].x, shapeQueue[0].y, 0, -1, -1 };
			shapeQueue.erase(shapeQueue.begin());
			if (selectedIdx >= 0)
				selectedIdx--;
			if (selectedIdx == -1)
				selectedShape = { -1, -1, 0, -1, -1 };
		}

		hPen = CreatePen(BS_SOLID, 1, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(hDC, hPen);
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

		for (int y = 0; y < boardSize; y++)
		{
			for (int x = 0; x < boardSize; x++)
			{
				Rectangle(hDC, x * rectLen, y * rectLen, (x + 1) * rectLen, (y + 1) * rectLen);
			}
		}

		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);
		SelectObject(hDC, oldPen);
		DeleteObject(hPen);

		if (selectedIdx != -1)
			shapeQueue.push_back(selectedShape);

		for (int i = 0; i < shapeQueue.size(); i++)
		{
			if (i == shapeQueue.size() - 1 && selectedIdx != -1)
			{
				hPen = CreatePen(BS_SOLID, 1, RGB(0, 0, 0));
				oldPen = (HPEN)SelectObject(hDC, hPen);
				hBrush = CreateSolidBrush(RGB(255, 255, 255));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

				Rectangle(hDC, shapeQueue[i].x * rectLen, shapeQueue[i].y * rectLen, (shapeQueue[i].x + 1) * rectLen, (shapeQueue[i].y + 1) * rectLen);

				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);
				SelectObject(hDC, oldPen);
				DeleteObject(hPen);

				hPen = CreatePen(BS_SOLID, 3, RGB(0, 255, 255));
				oldPen = (HPEN)SelectObject(hDC, hPen);
			}
			else
			{
				hPen = CreatePen(BS_SOLID, 1, RGB(0, 0, 0));
				oldPen = (HPEN)SelectObject(hDC, hPen);
			}

			int shapeSize;
			shapeSize = rectLen - (2 - shapeQueue[i].size) * 2;

			switch (shapeQueue[i].shapeIdx)
			{
				case 0:
					hBrush = CreateSolidBrush(baseColors[shapeQueue[i].colorIdx]);
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

					Ellipse(hDC, shapeQueue[i].x * rectLen + (2 - shapeQueue[i].size) * 2, shapeQueue[i].y * rectLen + (2 - shapeQueue[i].size) * 2, (shapeQueue[i].x + 1) * rectLen - (2 - shapeQueue[i].size) * 2, (shapeQueue[i].y + 1) * rectLen - (2 - shapeQueue[i].size) * 2);
					break;
				case 1:
					hBrush = CreateSolidBrush(baseColors[shapeQueue[i].colorIdx]);
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

					POINT triPoints[3];
					triPoints[0] = { shapeQueue[i].x * rectLen + rectLen / 2, shapeQueue[i].y * rectLen + (2 - shapeQueue[i].size) * 2 };
					triPoints[1] = { (shapeQueue[i].x + 1) * rectLen - (2 - shapeQueue[i].size) * 2, (shapeQueue[i].y + 1) * rectLen - (2 - shapeQueue[i].size) * 2 };
					triPoints[2] = { shapeQueue[i].x * rectLen + (2 - shapeQueue[i].size) * 2, (shapeQueue[i].y + 1) * rectLen - (2 - shapeQueue[i].size) * 2 };
					Polygon(hDC, triPoints, 3);
					break;
				case 2:
					hBrush = CreateSolidBrush(baseColors[shapeQueue[i].colorIdx]);
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

					Rectangle(hDC, shapeQueue[i].x * rectLen + (2 - shapeQueue[i].size) * 2, shapeQueue[i].y * rectLen + (2 - shapeQueue[i].size) * 2, (shapeQueue[i].x + 1) * rectLen - (2 - shapeQueue[i].size) * 2, (shapeQueue[i].y + 1) * rectLen - (2 - shapeQueue[i].size) * 2);
					break;
			}
			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
			SelectObject(hDC, oldPen);
			DeleteObject(hPen);
		}

		if (selectedIdx != -1)
			shapeQueue.pop_back();

		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN:
		if ((wParam == VK_UP || wParam == VK_DOWN || wParam == VK_LEFT || wParam == VK_RIGHT) && selectedIdx != -1)
		{
			switch (wParam)
			{
				case VK_UP:
					shapeQueue[selectedIdx].y = shapeQueue[selectedIdx].y == 0 ? boardSize - 1 : shapeQueue[selectedIdx].y - 1;
					selectedShape.y = selectedShape.y == 0 ? boardSize - 1 : selectedShape.y - 1;
					break;
				case VK_DOWN:
					shapeQueue[selectedIdx].y = shapeQueue[selectedIdx].y == boardSize - 1 ? 0 : shapeQueue[selectedIdx].y + 1;
					selectedShape.y = selectedShape.y == boardSize - 1 ? 0 : selectedShape.y + 1;
					break;
				case VK_LEFT:
					shapeQueue[selectedIdx].x = shapeQueue[selectedIdx].x == 0 ? boardSize - 1 : shapeQueue[selectedIdx].x - 1;
					selectedShape.x = selectedShape.x == 0 ? boardSize - 1 : selectedShape.x - 1;
					break;
				case VK_RIGHT:
					shapeQueue[selectedIdx].x = shapeQueue[selectedIdx].x == boardSize - 1 ? 0 : shapeQueue[selectedIdx].x + 1;
					selectedShape.x = selectedShape.x == boardSize - 1 ? 0 : selectedShape.x + 1;
					break;
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

	case WM_CHAR:
		srand((unsigned int)time(NULL));

		if (wParam == 's')
		{
			boardSize = 30;
			rectLen = 900 / boardSize;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'm')
		{
			boardSize = 40;
			rectLen = 900 / boardSize;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'l')
		{
			boardSize = 50;
			rectLen = 900 / boardSize;
			InvalidateRect(hWnd, NULL, TRUE);
		}

		else if (wParam == 'e')
		{
			int tempX = rand() % boardSize, tempY = rand() % boardSize;
			while (board[tempY][tempX].shapeIdx != -1)
			{
				tempX = rand() % boardSize;
				tempY = rand() % boardSize;
			}
			board[tempY][tempX] = { tempX, tempY, 2, 0, rand() % 4 };
			shapeQueue.push_back(board[tempY][tempX]);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 't')
		{
			int tempX = rand() % boardSize, tempY = rand() % boardSize;
			while (board[tempY][tempX].shapeIdx != -1)
			{
				tempX = rand() % boardSize;
				tempY = rand() % boardSize;
			}
			board[tempY][tempX] = { tempX, tempY, 2, 1, rand() % 4 };
			shapeQueue.push_back(board[tempY][tempX]);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'r')
		{
			int tempX = rand() % boardSize, tempY = rand() % boardSize;
			while (board[tempY][tempX].shapeIdx != -1)
			{
				tempX = rand() % boardSize;
				tempY = rand() % boardSize;
			}
			board[tempY][tempX] = { tempX, tempY, 2, 2, rand() % 4 };
			shapeQueue.push_back(board[tempY][tempX]);
			InvalidateRect(hWnd, NULL, TRUE);
		}

		else if (wParam >= 48 && wParam <= 57)
		{
			if (wParam - 48 >= shapeQueue.size())
				break;
			selectedShape = shapeQueue[wParam - 48];
			selectedIdx = wParam - 48;
			InvalidateRect(hWnd, NULL, TRUE);
		}

		else if (wParam == '+' && selectedIdx != -1)
		{
			if (selectedShape.size == 2)
				break;
			shapeQueue[selectedIdx].size++;
			selectedShape.size++;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == '-' && selectedIdx != -1)
		{
			if (selectedShape.size == 0)
				break;
			shapeQueue[selectedIdx].size--;
			selectedShape.size--;
			InvalidateRect(hWnd, NULL, TRUE);
		}

		else if (wParam == 'd')
		{
			if (selectedIdx == -1)
				break;
			shapeQueue.erase(shapeQueue.begin() + selectedIdx);
			selectedIdx = -1;
			selectedShape = { -1, -1, 0, -1, -1 };
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'p')
		{
			shapeQueue.clear();
			selectedIdx = -1;
			selectedShape = { -1, -1, 0, -1, -1 };
			isChanged = false;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'q')
		{
			PostQuitMessage(0);
		}

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}