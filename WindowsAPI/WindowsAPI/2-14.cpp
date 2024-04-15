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
	TCHAR letter[10];
	Letter(int x, int y, const TCHAR letter[])
	{
		this->x = x;
		this->y = y;
		wsprintf(this->letter, letter);
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
	static vector<Letter> letters;
	static Player player(9, 19);
	static int mode = 0;

	switch (uMsg) {
	case WM_CREATE:
		mode = 0;
		letters.clear();
		for (int i = 0; i < 10; i++)
		{
			int tempX, tempY;
			bool flag = false;
			tempX = rand() % 20;
			tempY = rand() % 20;

			for (int j = 0; j < letters.size(); j++)
			{
				if ((letters[j].x == tempX && letters[j].y == tempY) || (letters[j].x == 9 && letters[j].y == 19))
				{
					i--;
					flag = true;
					break;
				}
			}
			if (flag)
				continue;

			TCHAR tempLetter[10];
			wsprintf(tempLetter, L"%d", i);
			letters.push_back(Letter(tempX, tempY, tempLetter));
		}

		InvalidateRect(hWnd, NULL, TRUE);
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

		for (int i = 0; i < letters.size(); i++)
		{
			rect.left = letters[i].x * 30;
			rect.top = letters[i].y * 30;
			rect.right = (letters[i].x + 1) * 30;
			rect.bottom = (letters[i].y + 1) * 30;
			DrawText(hDC, letters[i].letter, lstrlen(letters[i].letter), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}

		hBrush = CreateSolidBrush(RGB(255, 150, 0));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Ellipse(hDC, player.x * 30 + 4, player.y * 30 + 4, (player.x + 1) * 30 - 4, (player.y + 1) * 30 - 4);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);

		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN:
		break;

	case WM_CHAR:
		if (wParam == 'w')
		{
			player.y = player.y == 0 ? 19 : player.y - 1;
			for (int i = 0; i < letters.size(); i++)
			{
				if (player.x == letters[i].x && player.y == letters[i].y)
				{
					letters[i].y = letters[i].y == 0 ? 19 : letters[i].y - 1;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 's')
		{
			player.y = player.y == 19 ? 0 : player.y + 1;
			for (int i = 0; i < letters.size(); i++)
			{
				if (player.x == letters[i].x && player.y == letters[i].y)
				{
					letters[i].y = letters[i].y == 19 ? 0 : letters[i].y + 1;
					for (int j = 0; j < letters.size(); j++)
					{
						if (i == j)
							continue;
						if (letters[i].x == letters[j].x && letters[i].y == letters[j].y)
						{
							int a, b;
							a = 0;
							b = 0;
							for (int k = 0; k < lstrlen(letters[i].letter); k++)
							{
								a *= 10;
								a += letters[i].letter[k] - 48;
							}
							for (int k = 0; k < lstrlen(letters[i].letter); k++)
							{
								b *= 10;
								b += letters[j].letter[k] - 48;
							}
							wsprintf(letters[i].letter, L"%d", a + b);
							letters.erase(letters.begin() + j);
						}
					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'a')
		{
			player.x = player.x == 0 ? 19 : player.x - 1;
			for (int i = 0; i < letters.size(); i++)
			{
				if (player.x == letters[i].x && player.y == letters[i].y)
				{
					letters[i].x = letters[i].x == 0 ? 19 : letters[i].x - 1;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'd')
		{
			player.x = player.x == 19 ? 0 : player.x + 1;
			for (int i = 0; i < letters.size(); i++)
			{
				if (player.x == letters[i].x && player.y == letters[i].y)
				{
					letters[i].x = letters[i].x == 19 ? 0 : letters[i].x + 1;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}

		else if (wParam == '1')
		{
			if (mode == 0)
				break;
			mode = 0;
			letters.clear();
			for (int i = 0; i < 10; i++)
			{
				int tempX, tempY;
				bool flag = false;
				tempX = rand() % 20;
				tempY = rand() % 20;

				for (int j = 0; j < letters.size(); j++)
				{
					if ((letters[j].x == tempX && letters[j].y == tempY) || (letters[j].x == 9 && letters[j].y == 19))
					{
						i--;
						flag = true;
						break;
					}
				}
				if (flag)
					continue;

				letters.push_back(Letter(tempX, tempY, L""));
			}
		}

		else if (wParam == '2')
		{
			if (mode == 1)
				break;
			mode = 1;
		}

		else if (wParam == '3')
		{
			if (mode == 2)
				break;
			mode = 2;
		}

		else if (wParam == 'p')
		{
			letters.clear();
			player.x = 9;
			player.y = 19;

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