#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include "resource.h"

using namespace std;

HINSTANCE g_hInst;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dialog_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

enum Calc {
	Plus,
	Minus,
	Mul,
	Divide
};

const static TCHAR ONE[2] = L"1";
const static TCHAR TWO[2] = L"2";
const static TCHAR THREE[2] = L"3";
const static TCHAR FOUR[2] = L"4";
const static TCHAR FIVE[2] = L"5";
const static TCHAR SIX[2] = L"6";
const static TCHAR SEVEN[2] = L"7";
const static TCHAR EIGHT[2] = L"8";
const static TCHAR NINE[2] = L"9";
const static TCHAR ZERO[2] = L"0";

const static TCHAR PLUS[4] = L" + ";
const static TCHAR MINUS[4] = L" - ";
const static TCHAR MUL[4] = L" * ";
const static TCHAR DIVIDE[4] = L" / ";

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	HWND hWnd;
	MSG msg;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Main Class");
	RegisterClass(&WndClass);

	hWnd = CreateWindow(_T("Main Class"), _T("Window Program"), WS_OVERLAPPEDWINDOW, 0, 0, 1000, 800, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	switch (iMsg) {
	case WM_CREATE:
		DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)Dialog_Proc);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

BOOL CALLBACK Dialog_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	static TCHAR editText[200];
	static vector<double> nums;
	static vector<Calc> calc;

	switch (iMsg) {
		case WM_INITDIALOG: {
			break;
		}

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDC_BUTTON1: {
					if (nums.size() == calc.size()) {
						nums.push_back(1);
					}
					else {
						nums[nums.size() - 1] = nums[nums.size() - 1] * 10 + 1;
					}

					GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
					lstrcat(editText, ONE);
					SetDlgItemText(hDlg, IDC_EDIT1, editText);
					break;
				}
				case IDC_BUTTON2: {
					if (nums.size() == calc.size()) {
						nums.push_back(2);
					}
					else {
						nums[nums.size() - 1] = nums[nums.size() - 1] * 10 + 2;
					}
					GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
					lstrcat(editText, TWO);
					SetDlgItemText(hDlg, IDC_EDIT1, editText);
					break;
				}
				case IDC_BUTTON3: {
					if (nums.size() == calc.size()) {
						nums.push_back(3);
					}
					else {
						nums[nums.size() - 1] = nums[nums.size() - 1] * 10 + 3;
					}
					GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
					lstrcat(editText, THREE);
					SetDlgItemText(hDlg, IDC_EDIT1, editText);
					break;
				}
				case IDC_BUTTON4: {
					if (nums.size() == calc.size()) {
						nums.push_back(4);
					}
					else {
						nums[nums.size() - 1] = nums[nums.size() - 1] * 10 + 4;
					}
					GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
					lstrcat(editText, FOUR);
					SetDlgItemText(hDlg, IDC_EDIT1, editText);
					break;
				}
				case IDC_BUTTON5: {
					if (nums.size() == calc.size()) {
						nums.push_back(5);
					}
					else {
						nums[nums.size() - 1] = nums[nums.size() - 1] * 10 + 5;
					}
					GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
					lstrcat(editText, FIVE);
					SetDlgItemText(hDlg, IDC_EDIT1, editText);
					break;
				}
				case IDC_BUTTON6: {
					if (nums.size() == calc.size()) {
						nums.push_back(6);
					}
					else {
						nums[nums.size() - 1] = nums[nums.size() - 1] * 10 + 6;
					}
					GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
					lstrcat(editText, SIX);
					SetDlgItemText(hDlg, IDC_EDIT1, editText);
					break;
				}
				case IDC_BUTTON7: {
					if (nums.size() == calc.size()) {
						nums.push_back(7);
					}
					else {
						nums[nums.size() - 1] = nums[nums.size() - 1] * 10 + 7;
					}
					GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
					lstrcat(editText, SEVEN);
					SetDlgItemText(hDlg, IDC_EDIT1, editText);
					break;
				}
				case IDC_BUTTON8: {
					if (nums.size() == calc.size()) {
						nums.push_back(8);
					}
					else {
						nums[nums.size() - 1] = nums[nums.size() - 1] * 10 + 8;
					}
					GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
					lstrcat(editText, EIGHT);
					SetDlgItemText(hDlg, IDC_EDIT1, editText);
					break;
				}
				case IDC_BUTTON9: {
					if (nums.size() == calc.size()) {
						nums.push_back(9);
					}
					else {
						nums[nums.size() - 1] = nums[nums.size() - 1] * 10 + 9;
					}
					GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
					lstrcat(editText, NINE);
					SetDlgItemText(hDlg, IDC_EDIT1, editText);
					break;
				}
				case IDC_BUTTON10: {
					if (nums.size() == calc.size()) {
						nums.push_back(0);
					}
					else {
						nums[nums.size() - 1] = nums[nums.size() - 1] * 10;
						if (nums[nums.size() - 1] == 0)
							break;
					}
					GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
					lstrcat(editText, ZERO);
					SetDlgItemText(hDlg, IDC_EDIT1, editText);
					break;
				}

				case IDC_BUTTON_PLUS: {
					if (nums.size() <= calc.size()) {
						break;
					}
					calc.push_back(Plus);
					GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
					lstrcat(editText, PLUS);
					SetDlgItemText(hDlg, IDC_EDIT1, editText);
					break;
				}
				case IDC_BUTTON_MIN: {
					if (nums.size() <= calc.size()) {
						break;
					}
					calc.push_back(Minus);
					GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
					lstrcat(editText, MINUS);
					SetDlgItemText(hDlg, IDC_EDIT1, editText);
					break;
				}
				case IDC_BUTTON_SLASH: {
					if (nums.size() <= calc.size()) {
						break;
					}
					calc.push_back(Divide);
					GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
					lstrcat(editText, DIVIDE);
					SetDlgItemText(hDlg, IDC_EDIT1, editText);
					break;
				}
				case IDC_BUTTON_MUL: {
					if (nums.size() <= calc.size()) {
						break;
					}
					calc.push_back(Mul);
					GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
					lstrcat(editText, MUL);
					SetDlgItemText(hDlg, IDC_EDIT1, editText);
					break;
				}
				case IDC_BUTTON_EQUAL: {
					if (nums.size() == calc.size())
						calc.pop_back();
					for (int i = 0; i < calc.size(); i++) {
						switch (calc[i]) {
							case Plus: {
								nums[i + 1] = nums[i] + nums[i + 1];
								break;
							}
							case Minus: {
								nums[i + 1] = nums[i] - nums[i + 1];
								break;
							}
							case Mul: {
								nums[i + 1] = nums[i] * nums[i + 1];
								break;
							}
							case Divide: {
								nums[i + 1] = nums[i] / nums[i + 1];
								break;
							}
						}
					}

					double temp = nums[nums.size() - 1];
					nums.clear();
					nums.push_back(temp);
					calc.clear();
					if (temp - (int)temp != 0) {
						swprintf(editText, L"%Lf", temp);
					}
					else {
						swprintf(editText, L"%d", (int)temp);
					}

					SetDlgItemText(hDlg, IDC_EDIT1, editText);
					break;
				}
				case IDC_BUTTON_TENTIMES: {
					GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
					TCHAR temp[100];
					if (nums[nums.size() - 1] - (int)nums[nums.size() - 1] != 0) {
						swprintf(temp, L"%Lf", nums[nums.size() - 1]);
					}
					else {
						swprintf(temp, L"%d", (int)nums[nums.size() - 1]);
					}
					int index = lstrlen(editText) - lstrlen(temp);
					editText[index] = '\0';
					nums[nums.size() - 1] *= 10;
					if (nums[nums.size() - 1] - (int)nums[nums.size() - 1] != 0) {
						swprintf(temp, L"%Lf", nums[nums.size() - 1]);
					}
					else {
						swprintf(temp, L"%d", (int)nums[nums.size() - 1]);
					}
					lstrcat(editText, temp);
					SetDlgItemText(hDlg, IDC_EDIT1, editText);
					break;
				}
				case IDC_BUTTON_ROOT: {
					TCHAR temp[100];
					swprintf(temp, L"%Lf", sqrt(nums[0]));
					lstrcpy(editText, temp);
					SetDlgItemText(hDlg, IDC_EDIT1, editText);
					break;
				}
				case IDC_BUTTON_RESET: {
					for (int i = 0; i < nums.size(); i++) {
						string tempStr = to_string((int)nums[i]);
						for (int j = 0; j < tempStr.size() / 2; j++) {
							char temp = tempStr[j];
							tempStr[j] = tempStr[tempStr.size() - j - 1];
							tempStr[tempStr.size() - j - 1] = temp;
						}
						nums[i] = stod(tempStr);
					}

					GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
					break;
				}
				case IDC_BUTTON_CE: {
					GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
					TCHAR temp[100];
					if (nums[nums.size() - 1] - (int)nums[nums.size() - 1] != 0) {
						swprintf(temp, L"%Lf", nums[nums.size() - 1]);
					}
					else {
						swprintf(temp, L"%d", (int)nums[nums.size() - 1]);
					}
					int index = lstrlen(editText);
					if (nums.size() > calc.size() && nums.size() > 0) {
						index -= lstrlen(temp);
						nums.pop_back();
					}
					if (calc.size() > 0) {
						index -= 3;
						calc.pop_back();
					}
					editText[index] = '\0';
					SetDlgItemText(hDlg, IDC_EDIT1, editText);
					break;
				}
				case IDC_BUTTON_BS: {
					GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
					if (nums.size() <= calc.size()) {
						calc.pop_back();
						GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
						editText[lstrlen(editText) - 3] = '\0';
						SetDlgItemText(hDlg, IDC_EDIT1, editText);
					}
					else if (nums.size() > 0) {
						if (nums[nums.size() - 1] / 10 < 1) {
							GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);

							nums.pop_back();
							if (nums.size() > 0) {
								calc.pop_back();
								editText[lstrlen(editText) - 4] = '\0';
							}
							else {
								editText[lstrlen(editText) - 1] = '\0';
							}
							SetDlgItemText(hDlg, IDC_EDIT1, editText);
						}
						else {
							GetDlgItemText(hDlg, IDC_EDIT1, editText, 200);
							int index = lstrlen(editText);
							editText[index - 1] = '\0';
							nums[nums.size() - 1] = (int)nums[nums.size() - 1] / 10;
							SetDlgItemText(hDlg, IDC_EDIT1, editText);
						}
					}
					SetDlgItemText(hDlg, IDC_EDIT1, editText);
					break;
				}
				case IDC_BUTTON_CLEAR: {
					nums.clear();
					calc.clear();

					SetDlgItemText(hDlg, IDC_EDIT1, L"");
					break;
				}
				case IDC_BUTTON_EXIT: {
					EndDialog(hDlg, 0);
					PostQuitMessage(0);
					break;
				}
			}
			break;
		}

		case WM_CLOSE: {
			EndDialog(hDlg, 0);
			PostQuitMessage(0);
			break;
		}
	}
	return 0;
}