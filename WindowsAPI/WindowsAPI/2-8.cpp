#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>

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

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
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
	static SIZE size;
	static TCHAR str[10][31];
	static int strCount;
	static int currentLine;
	static bool _isUpperCase;
	static bool _isInsert;
	static bool _isF2;
	static bool _isPageDn;
	static bool _isPageUp;

	switch (uMsg) {
	case WM_CREATE:
		strCount = 0;
		currentLine = 0;
		_isUpperCase = false;
		_isInsert = false;
		_isF2 = false;
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		if (str[currentLine][strCount] == '\0')
			GetTextExtentPoint32(hDC, str[currentLine], lstrlen(str[currentLine]), &size);
		else
		{
			TCHAR tempStr[31];
			tempStr[0] = '\0';
			for (int i = 0; i < strCount; i++)
			{
				if (str[currentLine][i] == '\0')
					break;
				tempStr[i] = str[currentLine][i];
				tempStr[i + 1] = '\0';
			}
			GetTextExtentPoint32(hDC, tempStr, lstrlen(tempStr), &size);
		}
		for (int i = 0; i < 10; i++)
		{
			if (_isF2)
			{
				TextOut(hDC, 0, i * 18, L"____", 4);
				TextOut(hDC, 32, i * 18, str[i], lstrlen(str[i]));
			}
			else if (_isPageDn)
			{
				bool isWordStarted = false;
				TCHAR tempStr[100];
				int tempStrCnt = 0;
				for (int j = 0; j < lstrlen(str[i]); j++)
				{
					if (str[i][j] != ' ')
					{
						if (!isWordStarted)
						{
							tempStr[tempStrCnt++] = '(';
							isWordStarted = true;
						}
						tempStr[tempStrCnt++] = str[i][j];
						if (str[i][j] >= 97 && str[i][j] <= 122)
						{
							tempStr[tempStrCnt - 1] -= 32;
						}
					}
					else if (str[i][j] == ' ')
					{
						if (isWordStarted)
						{
							tempStr[tempStrCnt++] = ')';
							isWordStarted = false;
						}
						tempStr[tempStrCnt++] = str[i][j];
					}
				}
				if (isWordStarted)
					tempStr[tempStrCnt++] = ')';
				tempStr[tempStrCnt++] = '\0';
				TextOut(hDC, 0, i * 18, tempStr, lstrlen(tempStr));
			}
			else if (_isPageUp)
			{
				TCHAR tempStr[31];
				int tempStrCnt = 0;
				for (int j = 0; j < lstrlen(str[i]); j++)
				{
					if (str[i][j] != ' ')
						tempStr[tempStrCnt++] = str[i][j];
				}
				tempStr[tempStrCnt++] = '\0';
				TextOut(hDC, 0, i * 18, tempStr, lstrlen(tempStr));
			}
			else
				TextOut(hDC, 0, i * 18, str[i], lstrlen(str[i]));
		}
		SetCaretPos(size.cx, currentLine * 18);
		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN:

		if (wParam == VK_HOME)
		{
			strCount = 0;
		}

		else if (wParam == VK_END)
		{
			for (; strCount < 31; strCount++)
			{
				if (str[currentLine][strCount] == '\0')
					break;
			}
		}

		else if (wParam == VK_F1)
		{
			_isUpperCase = !_isUpperCase;
		}

		else if (wParam == VK_F2)
		{
			_isF2 = !_isF2;
		}

		else if (wParam == VK_F3)
		{
			for (int i = 9; i > 0; i--)
			{
				wsprintf(str[i], str[i - 1]);
			}
			wsprintf(str[0], L"");
			strCount = lstrlen(str[currentLine]);
		}

		else if (wParam == VK_F4)
		{
			TCHAR tempStr[10][31];
			for (int i = 0; i < 10; i++)
			{
				wsprintf(tempStr[i], str[9 - i]);
			}
			for (int i = 0; i < 10; i++)
			{
				wsprintf(str[i], tempStr[i]);
			}
			strCount = lstrlen(str[currentLine]);
		}

		else if (wParam == VK_INSERT)
		{
			_isInsert = !_isInsert;
		}

		else if (wParam == VK_DELETE)
		{
			if (str[currentLine][strCount] != ' ' && str[currentLine][strCount] != '\0')
			{
				int start = strCount - 1;
				int end = strCount + 1;
				for (; start > 0; start--)
				{
					if (str[currentLine][start] == ' ')
						break;
				}
				for (; end < 31; end++)
				{
					if (str[currentLine][end] == ' ' || str[currentLine][end] == '\0')
						break;
				}
				strCount = start;
				for (int i = end; i < 31; i++)
				{
					str[currentLine][start++] = str[currentLine][i];
				}
				str[currentLine][start] = '\0';
			}
		}

		else if (wParam == VK_LEFT)
		{
			if (strCount > 0)
				strCount--;
		}
		else if (wParam == VK_RIGHT)
		{
			if (strCount < lstrlen(str[currentLine]))
				strCount++;
		}
		else if (wParam == VK_UP)
		{
			currentLine = currentLine == 0 ? 9 : currentLine - 1;
			while (strCount > 0)
			{
				if (str[currentLine][strCount - 1] == '\0')
					strCount--;
				else
				{
					break;
				}
			}
		}
		else if (wParam == VK_DOWN)
		{
			currentLine = currentLine == 9 ? 0 : currentLine + 1;
			while (strCount > 0)
			{
				if (str[currentLine][strCount - 1] == '\0')
					strCount--;
				else
				{
					break;
				}
			}
		}

		else if (wParam == VK_NEXT)
		{
			_isPageDn = !_isPageDn;
		}

		else if (wParam == VK_PRIOR)
		{
			_isPageUp = !_isPageUp;
		}

		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_CHAR:
		hDC = GetDC(hWnd);
		if (wParam == VK_RETURN)
		{
			strCount = 0;
			currentLine = currentLine == 9 ? 0 : currentLine + 1;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}

		else if (wParam == VK_BACK)
		{
			if (strCount > 0) {
				strCount--;
				for (int i = strCount; i < 30; i++)
				{
					if (str[currentLine][i + 1] == '\0')
					{
						str[currentLine][i] = '\0';
						break;
					}
					str[currentLine][i] = str[currentLine][i + 1];
				}
			}
			else if (currentLine > 0) {
				currentLine--;
				strCount = lstrlen(str[currentLine]);
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}

		else if (wParam == VK_ESCAPE)
		{
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 31; j++)
				{
					str[i][j] = '\0';
				}
			}
			currentLine = 0;
			strCount = 0;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}

		else if (wParam == VK_TAB)
		{
			if (strCount > 24)
			{
				while (strCount < 31)
				{
					str[currentLine][strCount++] = ' ';
					str[currentLine][strCount] = '\0';
				}
				strCount = 0;
				currentLine = currentLine == 9 ? 0 : currentLine + 1;
			}
			else
			{
				for (int i = 0; i < 5; i++)
				{
					str[currentLine][strCount++] = ' ';
				}
				str[currentLine][strCount] = '\0';
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}

		else if (wParam == '+')
		{
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < lstrlen(str[i]); j++)
				{
					if ((str[i][j] >= 97 && str[i][j] <= 122) ||
						(str[i][j] >= 65 && str[i][j] <= 90) ||
						(str[i][j] >= 48 && str[i][j] <= 57))
					{
						if (str[i][j] == 122)
							str[i][j] = 97;
						else if (str[i][j] == 90)
							str[i][j] = 65;
						else if (str[i][j] == 57)
							str[i][j] = 48;
						else
							str[i][j]++;
					}
				}
			}
			break;
		}

		else if (wParam == '-')
		{
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < lstrlen(str[i]); j++)
				{
					if ((str[i][j] >= 97 && str[i][j] <= 122) ||
						(str[i][j] >= 65 && str[i][j] <= 90) ||
						(str[i][j] >= 48 && str[i][j] <= 57))
					{
						if (str[i][j] == 97)
							str[i][j] = 122;
						else if (str[i][j] == 65)
							str[i][j] = 90;
						else if (str[i][j] == 48)
							str[i][j] = 57;
						else
							str[i][j]--;
					}
				}
			}
			break;
		}

		else if (wParam == 'q')
		{
			PostQuitMessage(0);
			break;
		}

		if (_isUpperCase)
		{
			if (wParam >= 97 && wParam <= 122)
			{
				wParam -= 32;
			}
		}
		else
		{
			if (wParam >= 65 && wParam <= 90)
			{
				wParam += 32;
			}
		}

		if (_isInsert && str[currentLine][29] == '\0')
		{
			for (int i = 29; i > strCount; i--)
			{
				str[currentLine][i] = str[currentLine][i - 1];
			}
			str[currentLine][strCount] = wParam;
		}
		else
		{
			if (str[currentLine][strCount] == '\0')
			{
				str[currentLine][strCount++] = wParam;
				str[currentLine][strCount] = '\0';
			}
			else
			{
				str[currentLine][strCount++] = wParam;
			}
		}

		if (strCount >= 30)
		{
			currentLine = currentLine == 9 ? 0 : currentLine + 1;
			strCount = 0;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_DESTROY:
		HideCaret(hWnd);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}