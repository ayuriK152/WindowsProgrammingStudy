#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>

using namespace std;

class Letter
{
public:
	int x, y;
	TCHAR letter;
	Letter(int x, int y, TCHAR letter)
	{
		this->x = x;
		this->y = y;
		this->letter = letter;
	}
};

class Player
{
public:
	int x, y;
	Player(int x, int y)
	{
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

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 630, 650, NULL, (HMENU)NULL, hInstance, NULL);
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
	srand((unsigned int)time(NULL));
	PAINTSTRUCT ps;
	HDC hDC;
	RECT rect;
	HBRUSH hBrush, oldBrush;
	static TCHAR words[5][10] = { L"KOREA", L"JAPAN", L"FRANCE", L"GERMANY", L"ITALY" };
	static int currentWordIdx;
	static int startPosX;
	static vector<Letter> currentWord;
	static Player player(9, 19);

	switch (uMsg) {
	case WM_CREATE:
		currentWordIdx = rand() % 5;
		startPosX = 9 - lstrlen(words[currentWordIdx]) / 2;
		for (int i = 0; i < lstrlen(words[currentWordIdx]); i++)
		{
			currentWord.push_back(Letter(startPosX + i, 1, words[currentWordIdx][i]));
		}

		int randIdx[3];
		randIdx[0] = { rand() % lstrlen(words[currentWordIdx]) };
		randIdx[1] = { rand() % lstrlen(words[currentWordIdx]) };
		randIdx[2] = { rand() % lstrlen(words[currentWordIdx]) };
		for (int i = 0; i < 3; i++)
		{
			currentWord[randIdx[i]].x = rand() % 20;
			currentWord[randIdx[i]].y = rand() % 20;
			if (currentWord[randIdx[i]].x == 9, currentWord[randIdx[i]].x == 19)
			{
				i--;
				continue;
			}
			TCHAR tempRand;
			bool randLetterFlag = true;
			while (randLetterFlag)
			{
				randLetterFlag = false;
				tempRand = rand() % 26 + 65;
				for (int j = 0; j < lstrlen(words[currentWordIdx]); j++)
				{
					if (tempRand == words[currentWordIdx][j])
					{
						randLetterFlag = true;
						break;
					}
				}
			}
			currentWord.push_back(Letter(rand() % 20, rand() % 20, rand() % 26 + 65));
		}
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		for (int x = 0; x <= 20; x++)
		{
			MoveToEx(hDC, x * 30, 0, NULL);
			LineTo(hDC, x * 30, 600);
		}
		for (int y = 0; y <= 20; y++)
		{
			MoveToEx(hDC, 0, y * 30, NULL);
			LineTo(hDC, 600, y * 30);
		}

		for (int i = 0; i < currentWord.size(); i++)
		{
			TCHAR temp[1];
			temp[0] = currentWord[i].letter;
			rect.left = currentWord[i].x * 30;
			rect.top = currentWord[i].y * 30;
			rect.right = (currentWord[i].x + 1) * 30;
			rect.bottom = (currentWord[i].y + 1) * 30;
			DrawText(hDC, temp, 1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}

		hBrush = CreateSolidBrush(RGB(255, 150, 0));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Ellipse(hDC, player.x * 30 + 4, player.y * 30 + 4, (player.x + 1) * 30 - 4, (player.y + 1) * 30 - 4);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);

		bool flag;
		flag = true;
		for (int i = 0; i < lstrlen(words[currentWordIdx]); i++)
		{
			if (currentWord[i].y != 1 || currentWord[i].x - i != startPosX)
			{
				flag = false;
				break;
			}
		}
		if (flag)
		{
			rect.left = 0;
			rect.top = 0;
			rect.right = 90;
			rect.bottom = 30;
			DrawText(hDC, L"COMPLETE!!", lstrlen(L"COMPLETE!!"), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}

		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_UP)
		{
			bool isNext = false, isNextNext = false;
			for (int i = 0; i < currentWord.size(); i++)
			{
				if (player.x == currentWord[i].x && ((player.y - 1 < 0 ? player.y + 19 : player.y - 1) == currentWord[i].y))
				{
					isNext = true;
					break;
				}
			}
			for (int i = 0; i < currentWord.size(); i++)
			{
				if (player.x == currentWord[i].x && ((player.y - 2 < 0 ? player.y + 18 : player.y - 2) == currentWord[i].y))
				{
					isNextNext = true;
					break;
				}
			}
			if (isNext && isNextNext)
				break;

			player.y = player.y == 0 ? 19 : player.y - 1;
			for (int i = 0; i < currentWord.size(); i++)
			{
				if (player.x == currentWord[i].x && player.y == currentWord[i].y)
				{
					currentWord[i].y = currentWord[i].y == 0 ? 19 : currentWord[i].y - 1;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == VK_DOWN)
		{
			bool isNext = false, isNextNext = false;
			for (int i = 0; i < currentWord.size(); i++)
			{
				if (player.x == currentWord[i].x && ((player.y + 1 > 19 ? player.y - 19 : player.y + 1) == currentWord[i].y))
				{
					isNext = true;
					break;
				}
			}
			for (int i = 0; i < currentWord.size(); i++)
			{
				if (player.x == currentWord[i].x && ((player.y + 2 > 19 ? player.y - 18 : player.y + 2) == currentWord[i].y))
				{
					isNextNext = true;
					break;
				}
			}
			if (isNext && isNextNext)
				break;

			player.y = player.y == 19 ? 0 : player.y + 1;
			for (int i = 0; i < currentWord.size(); i++)
			{
				if (player.x == currentWord[i].x && player.y == currentWord[i].y)
				{
					currentWord[i].y = currentWord[i].y == 19 ? 0 : currentWord[i].y + 1;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == VK_LEFT)
		{
			bool isNext = false, isNextNext = false;
			for (int i = 0; i < currentWord.size(); i++)
			{
				if (player.y == currentWord[i].y && ((player.x - 1 < 0 ? player.x + 19 : player.x - 1) == currentWord[i].x))
				{
					isNext = true;
					break;
				}
			}
			for (int i = 0; i < currentWord.size(); i++)
			{
				if (player.y == currentWord[i].y && ((player.x - 2 < 0 ? player.x + 18 : player.x - 2) == currentWord[i].x))
				{
					isNextNext = true;
					break;
				}
			}
			if (isNext && isNextNext)
				break;

			player.x = player.x == 0 ? 19 : player.x - 1;
			for (int i = 0; i < currentWord.size(); i++)
			{
				if (player.x == currentWord[i].x && player.y == currentWord[i].y)
				{
					currentWord[i].x = currentWord[i].x == 0 ? 19 : currentWord[i].x - 1;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == VK_RIGHT)
		{
			bool isNext = false, isNextNext = false;
			for (int i = 0; i < currentWord.size(); i++)
			{
				if (player.y == currentWord[i].y && ((player.x + 1 > 19 ? player.x - 19 : player.x + 1) == currentWord[i].x))
				{
					isNext = true;
					break;
				}
			}
			for (int i = 0; i < currentWord.size(); i++)
			{
				if (player.y == currentWord[i].y && ((player.x + 2 > 19 ? player.x - 18 : player.x + 2) == currentWord[i].x))
				{
					isNextNext = true;
					break;
				}
			}
			if (isNext && isNextNext)
				break;

			player.x = player.x == 19 ? 0 : player.x + 1;
			for (int i = 0; i < currentWord.size(); i++)
			{
				if (player.x == currentWord[i].x && player.y == currentWord[i].y)
				{
					currentWord[i].x = currentWord[i].x == 19 ? 0 : currentWord[i].x + 1;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

	case WM_CHAR:
		if (wParam == 's')
		{
			currentWord.clear();
			player.x = 9;
			player.y = 19;
			currentWordIdx = rand() % 5;
			startPosX = 9 - lstrlen(words[currentWordIdx]) / 2;
			for (int i = 0; i < lstrlen(words[currentWordIdx]); i++)
			{
				currentWord.push_back(Letter(startPosX + i, 1, words[currentWordIdx][i]));
			}

			int randIdx[3];
			randIdx[0] = { rand() % lstrlen(words[currentWordIdx]) };
			randIdx[1] = { rand() % lstrlen(words[currentWordIdx]) };
			randIdx[2] = { rand() % lstrlen(words[currentWordIdx]) };
			for (int i = 0; i < 3; i++)
			{
				currentWord[randIdx[i]].x = rand() % 20;
				currentWord[randIdx[i]].y = rand() % 20;
				if (currentWord[randIdx[i]].x == 9, currentWord[randIdx[i]].x == 19)
				{
					i--;
					continue;
				}
				TCHAR tempRand;
				bool randLetterFlag = true;
				while (randLetterFlag)
				{
					randLetterFlag = false;
					tempRand = rand() % 26 + 65;
					for (int j = 0; j < lstrlen(words[currentWordIdx]); j++)
					{
						if (tempRand == words[currentWordIdx][j])
						{
							randLetterFlag = true;
							break;
						}
					}
				}
				currentWord.push_back(Letter(rand() % 20, rand() % 20, rand() % 26 + 65));
			}

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