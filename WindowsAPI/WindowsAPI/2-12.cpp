#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#define CELLSIZE 18

struct Shape
{
	int shape;
	int color;
	int size;
	int x, y;
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
	static Shape shape[50][50];
	static Shape* shapeQueue[10];
	static Shape* selectedShape = NULL;
	static Shape hidedShape = { -1, -1, 0, -1, -1 };
	static int shapeQueueCnt = 0;
	static int boardSize = 40;
	static COLORREF baseColors[4] = { RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255), RGB(255, 255, 0) };
	static bool isShapeByColors = false;

	switch (uMsg) {
	case WM_CREATE:
		for (int i = 0; i < 50; i++)
		{
			for (int j = 0; j < 50; j++)
			{
				shape[i][j] = { -1, -1, 0, j, i };
			}
		}
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		for (int i = 0; i <= boardSize; i++)
		{
			MoveToEx(hDC, 0, i * CELLSIZE, NULL);
			LineTo(hDC, boardSize * CELLSIZE, i * CELLSIZE);
		}
		for (int i = 0; i <= boardSize; i++)
		{
			MoveToEx(hDC, i * CELLSIZE, 0, NULL);
			LineTo(hDC, i * CELLSIZE, boardSize * CELLSIZE);
		}

		if (!isShapeByColors)
		{
			for (int i = 0; i < boardSize; i++)
			{
				for (int j = 0; j < boardSize; j++)
				{
					if (shape[i][j].shape != -1)
					{
						hBrush = CreateSolidBrush(baseColors[shape[i][j].color]);
						oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
						switch (shape[i][j].shape)
						{
						case 0:
							Ellipse(hDC, j * CELLSIZE, i * CELLSIZE, (j + 1) * CELLSIZE, (i + 1) * CELLSIZE);
							break;
						case 1:
							POINT triPoints[3];
							triPoints[0] = { j * CELLSIZE + CELLSIZE / 2, i * CELLSIZE };
							triPoints[1] = { (j + 1) * CELLSIZE, (i + 1) * CELLSIZE };
							triPoints[2] = { j * CELLSIZE, (i + 1) * CELLSIZE };
							Polygon(hDC, triPoints, 3);
							break;
						case 2:
							Rectangle(hDC, j * CELLSIZE, i * CELLSIZE, (j + 1) * CELLSIZE, (i + 1) * CELLSIZE);
							break;
						}
					}
				}
			}
		}
		else
		{
			for (int i = 0; i <= boardSize; i++)
			{
				for (int j = 0; j <= boardSize; j++)
				{
					if (shape[i][j].shape != -1)
					{
						hBrush = CreateSolidBrush(baseColors[shape[i][j].color]);
						oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
						switch (shape[i][j].color)
						{
						case 0:
							POINT diaPoints[4];
							diaPoints[0] = { j * CELLSIZE + CELLSIZE / 2, i * CELLSIZE };
							diaPoints[1] = { (j + 1) * CELLSIZE, i * CELLSIZE + CELLSIZE / 2 };
							diaPoints[2] = { j * CELLSIZE + CELLSIZE / 2, (i + 1) * CELLSIZE };
							diaPoints[3] = { j * CELLSIZE, i * CELLSIZE + CELLSIZE / 2 };
							Polygon(hDC, diaPoints, 4);
							break;
						case 1:
							POINT triPoints[3];
							triPoints[0] = { j * CELLSIZE + CELLSIZE / 2, (i + 1) * CELLSIZE };
							triPoints[1] = { (j + 1) * CELLSIZE, i * CELLSIZE };
							triPoints[2] = { j * CELLSIZE, i * CELLSIZE };
							Polygon(hDC, triPoints, 3);
							break;
						case 2:
							POINT pentaPoints[5];
							pentaPoints[0] = { j * CELLSIZE + CELLSIZE / 2, i * CELLSIZE };
							pentaPoints[1] = { (j + 1) * CELLSIZE, i * CELLSIZE + CELLSIZE / 5 * 2 };
							pentaPoints[2] = { j * CELLSIZE + CELLSIZE / 5 * 4, (i + 1) * CELLSIZE };
							pentaPoints[3] = { j * CELLSIZE + CELLSIZE / 5, (i + 1) * CELLSIZE };
							pentaPoints[4] = { j * CELLSIZE, i * CELLSIZE + CELLSIZE / 5 * 2 };
							Polygon(hDC, pentaPoints, 5);
							break;
						case 3:
							POINT startPoints[5];
							startPoints[0] = { j * CELLSIZE + CELLSIZE / 2, i * CELLSIZE };
							startPoints[1] = { j * CELLSIZE + CELLSIZE / 5, (i + 1) * CELLSIZE };
							startPoints[2] = { (j + 1) * CELLSIZE, i * CELLSIZE + CELLSIZE / 5 * 2 };
							startPoints[3] = { j * CELLSIZE, i * CELLSIZE + CELLSIZE / 5 * 2 };
							startPoints[4] = { j * CELLSIZE + CELLSIZE / 5 * 4, (i + 1) * CELLSIZE };
							Polygon(hDC, startPoints, 5);
							break;
						}
						SelectObject(hDC, oldBrush);
						DeleteObject(hBrush);
					}
				}
			}
		}
		if (selectedShape != NULL)
		{
			hPen = CreatePen(BS_SOLID, 2, RGB(0, 255, 255));
			oldPen = (HPEN)SelectObject(hDC, hPen);
			hBrush = CreateSolidBrush(baseColors[selectedShape->color]);
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			if (!isShapeByColors)
			{
				switch (selectedShape->shape)
				{
				case 0:
					Ellipse(hDC, selectedShape->x * CELLSIZE, selectedShape->y * CELLSIZE, (selectedShape->x + 1) * CELLSIZE, (selectedShape->y + 1) * CELLSIZE);
					break;
				case 1:
					POINT triPoints[3];
					triPoints[0] = { selectedShape->x * CELLSIZE + CELLSIZE / 2, selectedShape->y * CELLSIZE };
					triPoints[1] = { (selectedShape->x + 1) * CELLSIZE, (selectedShape->y + 1) * CELLSIZE };
					triPoints[2] = { selectedShape->x * CELLSIZE, (selectedShape->y + 1) * CELLSIZE };
					Polygon(hDC, triPoints, 3);
					break;
				case 2:
					Rectangle(hDC, selectedShape->x * CELLSIZE, selectedShape->y * CELLSIZE, (selectedShape->x + 1) * CELLSIZE, (selectedShape->y + 1) * CELLSIZE);
					break;
				}
			}
			else
			{
				switch (selectedShape->color)
				{
				case 0:
					POINT diaPoints[4];
					diaPoints[0] = { selectedShape->x * CELLSIZE + CELLSIZE / 2, selectedShape->y * CELLSIZE };
					diaPoints[1] = { (selectedShape->x + 1) * CELLSIZE, selectedShape->y * CELLSIZE + CELLSIZE / 2 };
					diaPoints[2] = { selectedShape->x * CELLSIZE + CELLSIZE / 2, (selectedShape->y + 1) * CELLSIZE };
					diaPoints[3] = { selectedShape->x * CELLSIZE, selectedShape->y * CELLSIZE + CELLSIZE / 2 };
					Polygon(hDC, diaPoints, 4);
					break;
				case 1:
					POINT triPoints[3];
					triPoints[0] = { selectedShape->x * CELLSIZE + CELLSIZE / 2, (selectedShape->y + 1) * CELLSIZE };
					triPoints[1] = { (selectedShape->x + 1) * CELLSIZE, selectedShape->y * CELLSIZE };
					triPoints[2] = { selectedShape->x * CELLSIZE, selectedShape->y * CELLSIZE };
					Polygon(hDC, triPoints, 3);
					break;
				case 2:
					POINT pentaPoints[5];
					pentaPoints[0] = { selectedShape->x * CELLSIZE + CELLSIZE / 2, selectedShape->y * CELLSIZE };
					pentaPoints[1] = { (selectedShape->x + 1) * CELLSIZE, selectedShape->y * CELLSIZE + CELLSIZE / 5 * 2 };
					pentaPoints[2] = { selectedShape->x * CELLSIZE + CELLSIZE / 5 * 4, (selectedShape->y + 1) * CELLSIZE };
					pentaPoints[3] = { selectedShape->x * CELLSIZE + CELLSIZE / 5, (selectedShape->y + 1) * CELLSIZE };
					pentaPoints[4] = { selectedShape->x * CELLSIZE, selectedShape->y * CELLSIZE + CELLSIZE / 5 * 2 };
					Polygon(hDC, pentaPoints, 5);
					break;
				case 3:
					POINT startPoints[5];
					startPoints[0] = { selectedShape->x * CELLSIZE + CELLSIZE / 2, selectedShape->y * CELLSIZE };
					startPoints[1] = { selectedShape->x * CELLSIZE + CELLSIZE / 5, (selectedShape->y + 1) * CELLSIZE };
					startPoints[2] = { (selectedShape->x + 1) * CELLSIZE, selectedShape->y * CELLSIZE + CELLSIZE / 5 * 2 };
					startPoints[3] = { selectedShape->x * CELLSIZE, selectedShape->y * CELLSIZE + CELLSIZE / 5 * 2 };
					startPoints[4] = { selectedShape->x * CELLSIZE + CELLSIZE / 5 * 4, (selectedShape->y + 1) * CELLSIZE };
					Polygon(hDC, startPoints, 5);
					break;
				}
			}
			SelectObject(hDC, oldPen);
			DeleteObject(hPen);
			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
		}

		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_UP)
		{
			if (selectedShape == NULL)
				break;
			Shape* preShape = &shape[selectedShape->y][selectedShape->x];
			int movedPos = selectedShape->y == 0 ? boardSize - 1 : selectedShape->y - 1;
			if (shape[movedPos][selectedShape->x].shape != -1)
			{
				hidedShape = { shape[movedPos][selectedShape->x].shape, shape[movedPos][selectedShape->x].color, shape[movedPos][selectedShape->x].size, selectedShape->x, movedPos };
			}

			selectedShape = &shape[movedPos][selectedShape->x];

			shape[selectedShape->y][selectedShape->x].shape = preShape->shape;
			shape[selectedShape->y][selectedShape->x].color = preShape->color;
			shape[selectedShape->y][selectedShape->x].size = preShape->size;
			preShape->shape = -1;
			preShape->color = -1;
			preShape->size = 0;

			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == VK_DOWN)
		{
			if (selectedShape == NULL)
				break;
			Shape* preShape = &shape[selectedShape->y][selectedShape->x];
			int movedPos = selectedShape->y == boardSize - 1 ? 0 : selectedShape->y + 1;
			if (shape[movedPos][selectedShape->x].shape != -1)
			{
				hidedShape = { shape[movedPos][selectedShape->x].shape, shape[movedPos][selectedShape->x].color, shape[movedPos][selectedShape->x].size, selectedShape->x, movedPos };
			}

			selectedShape = &shape[movedPos][selectedShape->x];

			shape[selectedShape->y][selectedShape->x].shape = preShape->shape;
			shape[selectedShape->y][selectedShape->x].color = preShape->color;
			shape[selectedShape->y][selectedShape->x].size = preShape->size;
			preShape->shape = -1;
			preShape->color = -1;
			preShape->size = 0;

			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == VK_LEFT)
		{
			if (selectedShape == NULL)
				break;
			Shape* preShape = &shape[selectedShape->y][selectedShape->x];
			int movedPos = selectedShape->x == 0 ? boardSize - 1 : selectedShape->x - 1;
			if (shape[movedPos][selectedShape->x].shape != -1)
			{
				hidedShape = { shape[movedPos][selectedShape->x].shape, shape[movedPos][selectedShape->x].color, shape[movedPos][selectedShape->x].size, movedPos, selectedShape->y};
			}

			selectedShape = &shape[selectedShape->y][movedPos];

			shape[selectedShape->y][selectedShape->x].shape = preShape->shape;
			shape[selectedShape->y][selectedShape->x].color = preShape->color;
			shape[selectedShape->y][selectedShape->x].size = preShape->size;
			preShape->shape = -1;
			preShape->color = -1;
			preShape->size = 0;

			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == VK_RIGHT)
		{
			if (selectedShape == NULL)
				break;
			Shape* preShape = &shape[selectedShape->y][selectedShape->x];
			int movedPos = selectedShape->x == boardSize - 1 ? 0 : selectedShape->x + 1;
			if (shape[movedPos][selectedShape->x].shape != -1)
			{
				hidedShape = { shape[movedPos][selectedShape->x].shape, shape[movedPos][selectedShape->x].color, shape[movedPos][selectedShape->x].size, movedPos, selectedShape->y };
			}

			selectedShape = &shape[selectedShape->y][movedPos];

			shape[selectedShape->y][selectedShape->x].shape = preShape->shape;
			shape[selectedShape->y][selectedShape->x].color = preShape->color;
			shape[selectedShape->y][selectedShape->x].size = preShape->size;
			preShape->shape = -1;
			preShape->color = -1;
			preShape->size = 0;

			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

	case WM_CHAR:
		if (wParam == 's')
		{
			boardSize = 30;

			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'm')
		{
			boardSize = 40;

			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'l')
		{
			boardSize = 50;

			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'e')
		{
			srand((unsigned int)time(NULL));
			int tempX, tempY;
			while (true)
			{
				tempX = rand() % boardSize;
				tempY = rand() % boardSize;
				if (shape[tempY][tempX].shape == -1)
					break;
			}
			shape[tempY][tempX].shape = 0;
			shape[tempY][tempX].color = rand() % 4;
			shape[tempY][tempX].size = 3;

			if (shapeQueueCnt == 10) {
				shapeQueue[0]->shape = -1;
				shapeQueue[0]->color = -1;
				shapeQueue[0]->size = 3;
				for (int i = 0; i < 9; i++)
				{
					shapeQueue[i] = shapeQueue[i + 1];
				}
				shapeQueue[9] = &shape[tempY][tempX];
			}
			else
				shapeQueue[shapeQueueCnt++] = &shape[tempY][tempX];

			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 't')
		{
			srand((unsigned int)time(NULL));
			int tempX, tempY;
			while (true)
			{
				tempX = rand() % boardSize;
				tempY = rand() % boardSize;
				if (shape[tempY][tempX].shape == -1)
					break;
			}
			shape[tempY][tempX].shape = 1;
			shape[tempY][tempX].color = rand() % 4;
			shape[tempY][tempX].size = 3;

			if (shapeQueueCnt == 10) {
				shapeQueue[0]->shape = -1;
				shapeQueue[0]->color = -1;
				shapeQueue[0]->size = 3;
				for (int i = 0; i < 9; i++)
				{
					shapeQueue[i] = shapeQueue[i + 1];
				}
				shapeQueue[9] = &shape[tempY][tempX];
			}
			else
				shapeQueue[shapeQueueCnt++] = &shape[tempY][tempX];

			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'r')
		{
			srand((unsigned int)time(NULL));
			int tempX, tempY;
			while (true)
			{
				tempX = rand() % boardSize;
				tempY = rand() % boardSize;
				if (shape[tempY][tempX].shape == -1)
					break;
			}
			shape[tempY][tempX].shape = 2;
			shape[tempY][tempX].color = rand() % 4;
			shape[tempY][tempX].size = 3;

			if (shapeQueueCnt == 10) {
				shapeQueue[0]->shape = -1;
				shapeQueue[0]->color = -1;
				shapeQueue[0]->size = 3;
				for (int i = 0; i < 9; i++)
				{
					shapeQueue[i] = shapeQueue[i + 1];
				}
				shapeQueue[9] = &shape[tempY][tempX];
			}
			else
				shapeQueue[shapeQueueCnt++] = &shape[tempY][tempX];

			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'c')
		{
			isShapeByColors = !isShapeByColors;

			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'd')
		{
			isShapeByColors = !isShapeByColors;

			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'p')
		{
			for (int i = 0; i < 50; i++)
			{
				for (int j = 0; j < 50; j++)
				{
					shape[i][j] = { -1, -1, 0, j, i };
				}
			}
			shapeQueueCnt = 0;

			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam >= 48 && wParam <= 57)
		{
			int idx = wParam - 48;
			if (idx >= shapeQueueCnt)
				break;
			selectedShape = shapeQueue[idx];

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