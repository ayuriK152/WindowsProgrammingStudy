#define _CRT_SECURE_NO_WARNINGS
#include <windows.h> 
#include <TCHAR.H>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ModelessDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);


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
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 840, 840, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	srand((unsigned int)time(NULL));
	HDC mdc;
	HBITMAP hBitmap;
	RECT rt;
	PAINTSTRUCT ps;
	HDC hdc;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	HWND hDlg = NULL;

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hwnd, 1, 10, NULL);

		break;

	case WM_LBUTTONDOWN:
		hDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, (DLGPROC)&ModelessDlgProc);
		ShowWindow(hDlg, SW_SHOW);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			break;

		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;

	case WM_PAINT:
		GetClientRect(hwnd, &rt);
		hdc = BeginPaint(hwnd, &ps);
		mdc = CreateCompatibleDC(hdc); //--- 메모리 DC 만들기
		hBitmap = CreateCompatibleBitmap(hdc, rt.right, rt.bottom); //--- 메모리 DC와 연결할 비트맵 만들기
		SelectObject(mdc, (HBITMAP)hBitmap);
		Rectangle(mdc, 0, 0, rt.right, rt.bottom);


		BitBlt(hdc, 0, 0, rt.right, rt.bottom, mdc, 0, 0, SRCCOPY);

		DeleteDC(mdc);
		DeleteObject(hBitmap);
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL CALLBACK ModelessDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static int num;
	static int result[10];
	static int resultnum = 0;
	static int resultop[10];
	static int sum = 0;
	static TCHAR resulttext[100] = L"";
	static TCHAR texttemp[10] = L"";
	static double sumtemp = 0;
	static int b18 = 10;

	switch (iMsg) {
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDCANCEL:
			DestroyWindow(hDlg);
			break;

		case IDC_BUTTON1:
			result[resultnum] = result[resultnum] * 10 + 1;
			
			GetDlgItemText(hDlg, IDC_EDIT1, resulttext, 100);
			lstrcat(resulttext, L"1");
			SetDlgItemText(hDlg, IDC_EDIT1, resulttext);
			break;

		case IDC_BUTTON2:
			result[resultnum] = result[resultnum] * 10 + 2;
			
			GetDlgItemText(hDlg, IDC_EDIT1, resulttext, 100);
			lstrcat(resulttext, L"2");
			SetDlgItemText(hDlg, IDC_EDIT1, resulttext);
			break;

		case IDC_BUTTON3:
			result[resultnum] = result[resultnum] * 10 + 3;

			GetDlgItemText(hDlg, IDC_EDIT1, resulttext, 100);
			lstrcat(resulttext, L"3");
			SetDlgItemText(hDlg, IDC_EDIT1, resulttext);
			break;

		case IDC_BUTTON4:
			result[resultnum] = result[resultnum] * 10 + 4;

			GetDlgItemText(hDlg, IDC_EDIT1, resulttext, 100);
			lstrcat(resulttext, L"4");
			SetDlgItemText(hDlg, IDC_EDIT1, resulttext);
			break;

		case IDC_BUTTON5:
			result[resultnum] = result[resultnum] * 10 + 5;

			GetDlgItemText(hDlg, IDC_EDIT1, resulttext, 100);
			lstrcat(resulttext, L"5");
			SetDlgItemText(hDlg, IDC_EDIT1, resulttext);
			break;

		case IDC_BUTTON6:
			result[resultnum] = result[resultnum] * 10 + 6;

			GetDlgItemText(hDlg, IDC_EDIT1, resulttext, 100);
			lstrcat(resulttext, L"6");
			SetDlgItemText(hDlg, IDC_EDIT1, resulttext);
			break;

		case IDC_BUTTON7:
			result[resultnum] = result[resultnum] * 10 + 7;

			GetDlgItemText(hDlg, IDC_EDIT1, resulttext, 100);
			lstrcat(resulttext, L"7");
			SetDlgItemText(hDlg, IDC_EDIT1, resulttext);
			break;

		case IDC_BUTTON8:
			result[resultnum] = result[resultnum] * 10 + 8;
			GetDlgItemText(hDlg, IDC_EDIT1, resulttext, 100);
			lstrcat(resulttext, L"8");
			SetDlgItemText(hDlg, IDC_EDIT1, resulttext);
			break;

		case IDC_BUTTON9:
			result[resultnum] = result[resultnum] * 10 + 9;

			GetDlgItemText(hDlg, IDC_EDIT1, resulttext, 100);
			lstrcat(resulttext, L"9");
			SetDlgItemText(hDlg, IDC_EDIT1, resulttext);
			break;

		case IDC_BUTTON10:
			result[resultnum] = result[resultnum] * 10 + 0;

			GetDlgItemText(hDlg, IDC_EDIT1, resulttext, 100);
			lstrcat(resulttext, L"0");
			SetDlgItemText(hDlg, IDC_EDIT1, resulttext);
			break;

		case IDC_BUTTON11:
			for (int i = 0; result[i] != 0; i++)
			{
				int temp1 = 0;
				int temp2 = 0; 
				int temp3 = 0;
				if (result[i] != 0)
				{
					if (result[i] < 100 && result[i] > 9)
					{
						temp1 = result[i] % 10;
						temp2 = result[i] / 10;
						result[i] = temp1 * 10 + temp2;
					}
					else if (result[i] < 1000 )
					{
						temp1 = result[i] % 10;
						temp2 = (result[i] / 10) % 10;
						temp3 = result[i] / 100;
						result[i] = temp1 * 100 + temp2 * 10 + temp3;
					}
				}
			}
			GetDlgItemText(hDlg, IDC_EDIT1, resulttext, 100);
			lstrcpy(resulttext, L"");
			_itow(result[0], resulttext, 10);
			for (int i = 1; result[i] != 0; i++)
			{
				if(resultop[i] == 1)
					lstrcat(resulttext, L" + ");
				else if (resultop[i] == 2)
					lstrcat(resulttext, L" - ");
				else if (resultop[i] == 3)
					lstrcat(resulttext, L" * ");
				else if (resultop[i] == 4)
					lstrcat(resulttext, L" / ");

				_itow(result[i],texttemp, 10);
				lstrcat(resulttext, texttemp);
			}
			SetDlgItemText(hDlg, IDC_EDIT1, resulttext);

			break;

		case IDC_BUTTON12:
			result[resultnum] = 0;
			resultop[resultnum] = 0;

			GetDlgItemText(hDlg, IDC_EDIT1, resulttext, 100);
			lstrcpy(resulttext, L"");
			_itow(result[0], resulttext, 10);
			for (int i = 1; result[i] != 0; i++)
			{
				if (resultop[i] == 1)
					lstrcat(resulttext, L" + ");
				else if (resultop[i] == 2)
					lstrcat(resulttext, L" - ");
				else if (resultop[i] == 3)
					lstrcat(resulttext, L" * ");
				else if (resultop[i] == 4)
					lstrcat(resulttext, L" / ");

				_itow(result[i], texttemp, 10);
				lstrcat(resulttext, texttemp);
			}
			SetDlgItemText(hDlg, IDC_EDIT1, resulttext);
			break;

		case IDC_BUTTON13:
			for (int i = 0; i < 10; i++)
			{
				result[i] = 0;
				resultop[i] = 0;
			}
			resultnum = 0;
			sum = 0;
			GetDlgItemText(hDlg, IDC_EDIT1, resulttext, 100);
			lstrcpy(resulttext, L"");
			SetDlgItemText(hDlg, IDC_EDIT1, resulttext);
			break;

		case IDC_BUTTON14:
			for (int i = 1; i < 10; i++)
				if (resultop[i] == 2)
					result[i] = -result[i];

			for (int i = 1; i < 10; i++)
				if (resultop[i] == 3)
				{
					result[i] = result[i - 1] * result[i];
					result[i - 1] = 0;
				}

			for (int i = 1; i < 10; i++)
				if (resultop[i] == 4)
				{
					result[i] = result[i - 1] / result[i];
					result[i - 1] = 0;
				}

			for (int i = 0; i < 10; i++)
				sum = sum + result[i];

			for (int i = 0; i < 10; i++)
			{
				result[i] = 0;
				resultop[i] = 0;
			}

			resultnum = 0;

			lstrcpy(resulttext, L"");
			_itow(sum,resulttext , 2);
			SetDlgItemText(hDlg, IDC_EDIT1, resulttext);
			break;

		case IDC_BUTTON15:
			for (int i = 1; i < 10; i++)
				if (resultop[i] == 2)
					result[i] = -result[i];

			for (int i = 1; i < 10; i++)
				if (resultop[i] == 3)
				{
					result[i] = result[i - 1] * result[i];
					result[i - 1] = 0;
				}

			for (int i = 1; i < 10; i++)
				if (resultop[i] == 4)
				{
					result[i] = result[i - 1] / result[i];
					result[i - 1] = 0;
				}

			for (int i = 0; i < 10; i++)
				sum = sum + result[i];

			for (int i = 0; i < 10; i++)
			{
				result[i] = 0;
				resultop[i] = 0;
			}

			resultnum = 0;
			sumtemp = 1 / sum;
			lstrcpy(resulttext, L"");
			SetDlgItemInt(hDlg, IDC_EDIT1, sumtemp, TRUE);
			break;

		case IDC_BUTTON16:
			for (int i = 1; i < 10; i++)
				if (resultop[i] == 2)
					result[i] = -result[i];

			for (int i = 1; i < 10; i++)
				if (resultop[i] == 3)
				{
					result[i] = result[i - 1] * result[i];
					result[i - 1] = 0;
				}

			for (int i = 1; i < 10; i++)
				if (resultop[i] == 4)
				{
					result[i] = result[i - 1] / result[i];
					result[i - 1] = 0;
				}

			for (int i = 0; i < 10; i++)
				sum = sum + result[i];

			for (int i = 0; i < 10; i++)
			{
				result[i] = 0;
				resultop[i] = 0;
			}

			resultnum = 0;
			sum = sum * 10;
			SetDlgItemInt(hDlg, IDC_EDIT1, sum, TRUE);
			break;

		case IDC_BUTTON17:
			if(result[resultnum] > 0)
				result[resultnum] = result[resultnum] / 10;

			GetDlgItemText(hDlg, IDC_EDIT1, resulttext, 100);
			lstrcpy(resulttext, L"");
			_itow(result[0], resulttext, 10);
			for (int i = 1; result[i] != 0; i++)
			{
				if (resultop[i] == 1)
					lstrcat(resulttext, L" + ");
				else if (resultop[i] == 2)
					lstrcat(resulttext, L" - ");
				else if (resultop[i] == 3)
					lstrcat(resulttext, L" * ");
				else if (resultop[i] == 4)
					lstrcat(resulttext, L" / ");

				_itow(result[i], texttemp, 10);
				lstrcat(resulttext, texttemp);
			}
			SetDlgItemText(hDlg, IDC_EDIT1, resulttext);
			break;

		case IDC_BUTTON18:
			for (int i = 1; i < 10; i++)
				if (resultop[i] == 2)
					result[i] = -result[i];

			for (int i = 1; i < 10; i++)
				if (resultop[i] == 3)
				{
					result[i] = result[i - 1] * result[i];
					result[i - 1] = 0;
				}

			for (int i = 1; i < 10; i++)
				if (resultop[i] == 4)
				{
					result[i] = result[i - 1] / result[i];
					result[i - 1] = 0;
				}

			for (int i = 0; i < 10; i++)
				sum = sum + result[i];

			for (int i = 0; i < 10; i++)
			{
				result[i] = 0;
				resultop[i] = 0;
			}

			resultnum = 0;
			for (int i = 0; i < sum; i++)
				b18 = b18 * 10;
			b18 = b18 / 10;
			SetDlgItemInt(hDlg, IDC_EDIT1, b18, TRUE);
			break;


		case IDC_BUTTON19:
			resultnum++;
			resultop[resultnum] = 1;

			GetDlgItemText(hDlg, IDC_EDIT1, resulttext, 100);
			lstrcat(resulttext, L" + ");
			SetDlgItemText(hDlg, IDC_EDIT1, resulttext);
			break;

		case IDC_BUTTON20:
			resultnum++;
			resultop[resultnum] = 2;

			GetDlgItemText(hDlg, IDC_EDIT1, resulttext, 100);
			lstrcat(resulttext, L" - ");
			SetDlgItemText(hDlg, IDC_EDIT1, resulttext);
			break;

		case IDC_BUTTON21:
			resultnum++;
			resultop[resultnum] = 3;

			GetDlgItemText(hDlg, IDC_EDIT1, resulttext, 100);
			lstrcat(resulttext, L" * ");
			SetDlgItemText(hDlg, IDC_EDIT1, resulttext);
			break;

		case IDC_BUTTON22:
			resultnum++;
			resultop[resultnum] = 4;

			GetDlgItemText(hDlg, IDC_EDIT1, resulttext, 100);
			lstrcat(resulttext, L" / ");
			SetDlgItemText(hDlg, IDC_EDIT1, resulttext);
			break;

		case IDC_BUTTON23:
			for (int i = 1; i < 10; i++)
				if (resultop[i] == 2)
					result[i] = -result[i];

			for (int i = 1; i < 10; i++)
				if (resultop[i] == 3)
				{
					result[i] = result[i-1] * result[i];
					result[i - 1] = 0;
				}
					
			for (int i = 1; i < 10; i++)
				if (resultop[i] == 4)
				{
					result[i] = result[i - 1] / result[i];
					result[i - 1] = 0;
				}
			
			for (int i = 0; i < 10; i++)
				sum = sum + result[i];

			for (int i = 0; i < 10; i++)
			{
				result[i] = 0;
				resultop[i] = 0;
			}

			resultnum = 0;
			SetDlgItemInt(hDlg, IDC_EDIT1, sum, TRUE);
			sum = 0;
			break;
		
		}
		break;

	case WM_CLOSE:
		EndDialog(hDlg, 0);
		break;
	}
	return 0;
}