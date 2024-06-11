#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include<stdio.h>
#include "resource.h"
#include<cmath>
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dlalog_Proc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);//
//HINSTANCE g_hInst;//
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
int curvenum;
int sx;
int sy;
double movex[2];
double movey;
int movexs;
int moveys;
int cirmove;
int col[3];

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc, mdc;
    HBITMAP hBitmap;
    HBRUSH hbr, oldbr;
    HPEN hPen, oldPen;
    double PI = 3.14159;
    RECT rect;
    static int clientWidth = 0;
    static int clientHeight = 0;
    static int ex, ey;
    static int sprx = 0;
    static double mx1 = 0;

    //--- 메시지 처리하기
    switch (uMsg) {
    case WM_CREATE:
        cirmove = 0;
        //curvenum = 1;
        //movey=100;
        for (int i = 0; i < 3; i++) {
            col[i] = 100;
        }
        SetTimer(hWnd, 1, 10, NULL);
        SetTimer(hWnd, 2, 20, NULL);
        break;
    case WM_PAINT:
        DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)&Dlalog_Proc);
        hdc = BeginPaint(hWnd, &ps);
        GetClientRect(hWnd, &rect);
        clientWidth = rect.right - rect.left;
        clientHeight = rect.bottom - rect.top;
        //더블버퍼링
        mdc = CreateCompatibleDC(hdc); //--- 메모리 DC 만들기
        hBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom); //--- 메모리 DC와 연결할 비트맵 만들기
        SelectObject(mdc, (HBITMAP)hBitmap);
        //흰색 배경
        hbr = CreateSolidBrush(RGB(255, 255, 255));
        oldbr = (HBRUSH)SelectObject(mdc, hbr);
        FillRect(mdc, &rect, hbr);
        SelectObject(mdc, oldbr);
        DeleteObject(hbr);
        //+
        hPen = CreatePen(PS_DOT, 2, RGB(0, 0, 0)); // GDI: 펜 만들기
        oldPen = (HPEN)SelectObject(mdc, hPen); // 새로운 펜 선택하기
        MoveToEx(mdc, 800 / 2, 0, NULL);
        LineTo(mdc, 800 / 2, clientHeight);
        MoveToEx(mdc, 0, clientHeight / 2, NULL);
        LineTo(mdc, clientWidth, clientHeight / 2);
        SelectObject(mdc, oldPen); // 이전의 펜으로 돌아감
        DeleteObject(hPen);

        // sin 함수 그리기
        if (curvenum == 0) {
            hPen = CreatePen(PS_DOT, 2, RGB(col[0], col[1], col[2])); // GDI: 펜 만들기
            oldPen = (HPEN)SelectObject(mdc, hPen); // 새로운 펜 선택하기
            MoveToEx(mdc, movex[0], clientHeight / 2, NULL);
            for (int x = 0; x <= 1080; x++)
            {
                double radian = x * 3.14159265 / 180.0;
                int y = clientHeight / 2 + (movey * sin(radian));
                LineTo(mdc, x + movex[0], y);
            }
            MoveToEx(mdc, movex[1], clientHeight / 2, NULL);
            for (int x = 0; x <= 1080; x++)
            {
                double radian = x * 3.14159265 / 180.0;
                int y = clientHeight / 2 + (movey * sin(radian));
                LineTo(mdc, x + movex[1], y);
            }
            SelectObject(mdc, oldPen); // 이전의 펜으로 돌아감
            DeleteObject(hPen);
            if (cirmove == 1) {
                double radian = ex * 3.14159265 / 180.0;
                ey = clientHeight / 2 + (movey * sin(radian));
                if (movexs == 0) {
                    Ellipse(mdc, ex + movex[0] - 5, ey - 5, ex + movex[0] + 5, ey + 5);
                }
                else if (movexs == 1) {
                    Ellipse(mdc, ex + movex[1] - 5, ey - 5, ex + movex[1] + 5, ey + 5);
                }
            }

        }
        else if (curvenum == 1) {
            // 원 그리기
            //int radius = 100;
            double mx = 0;
            hPen = CreatePen(PS_DOT, 2, RGB(col[0], col[1], col[2])); // GDI: 펜 만들기
            oldPen = (HPEN)SelectObject(mdc, hPen); // 새로운 펜 선택하기
            MoveToEx(mdc, 100 + movex[0], clientHeight / 2, NULL);
            for (int angle = 0; angle < 1080; angle++)
            {
                double radian = angle * 3.14159265 / 180.0;
                double x = mx + movey * cos(radian);
                double y = clientHeight / 2 + movey * sin(radian);
                LineTo(mdc, x + movex[0], y);
                mx += 0.7;
            }
            mx = 0;
            MoveToEx(mdc, 100 + movex[1], clientHeight / 2, NULL);
            for (int angle = 0; angle < 1080; angle++)
            {
                double radian = angle * 3.14159265 / 180.0;
                double x = mx + movey * cos(radian);
                double y = clientHeight / 2 + movey * sin(radian);
                LineTo(mdc, x + movex[1], y);
                mx += 0.7;
            }
            SelectObject(mdc, oldPen); // 이전의 펜으로 돌아감
            DeleteObject(hPen);

            if (cirmove == 1) {
                double radian = sprx * 3.14159265 / 180.0;
                ex = mx1 + movey * cos(radian);
                ey = clientHeight / 2 + movey * sin(radian);
                if (movexs == 0) {
                    Ellipse(mdc, 250 + ex + movex[0] - 5, ey - 5, 250 + ex + movex[0] + 5, ey + 5);
                }
                else if (movexs == 1) {
                    Ellipse(mdc, 250 + ex + movex[1] - 5, ey - 5, 250 + ex + movex[1] + 5, ey + 5);
                }


            }
        }
        else if (curvenum == 2) {
            int y;
            hPen = CreatePen(PS_DOT, 2, RGB(col[0], col[1], col[2])); // GDI: 펜 만들기
            oldPen = (HPEN)SelectObject(mdc, hPen); // 새로운 펜 선택하기
            MoveToEx(mdc, movex[0], clientHeight / 2, NULL);
            for (int x = 0; x <= 800; x++) {
                if (0 <= x && x < 100) {
                    y = movey / 100 * x + clientHeight / 2;
                }
                if (100 <= x && x < 300) {
                    y = -movey / 100 * (x - 200) + clientHeight / 2;
                }
                if (300 <= x && x < 500) {
                    y = movey / 100 * (x - 400) + clientHeight / 2;
                }
                if (500 <= x && x < 700) {
                    y = -movey / 100 * (x - 600) + clientHeight / 2;
                }
                if (700 <= x && x <= 800) {
                    y = movey / 100 * (x - 800) + clientHeight / 2;
                }
                LineTo(mdc, x + movex[0], y);
            }
            MoveToEx(mdc, movex[1], clientHeight / 2, NULL);
            for (int x = 0; x <= 800; x++) {
                if (0 <= x && x < 100) {
                    y = movey / 100 * x + clientHeight / 2;
                }
                if (100 <= x && x < 300) {
                    y = -movey / 100 * (x - 200) + clientHeight / 2;
                }
                if (300 <= x && x < 500) {
                    y = movey / 100 * (x - 400) + clientHeight / 2;
                }
                if (500 <= x && x < 700) {
                    y = -movey / 100 * (x - 600) + clientHeight / 2;
                }
                if (700 <= x && x <= 800) {
                    y = movey / 100 * (x - 800) + clientHeight / 2;
                }
                LineTo(mdc, x + movex[1], y);
                SelectObject(mdc, oldPen); // 이전의 펜으로 돌아감
                DeleteObject(hPen);
            }
            if (cirmove == 1) {
                if (0 <= ex && ex < 100) {
                    ey = movey / 100 * ex + clientHeight / 2;
                }
                if (100 <= ex && ex < 300) {
                    ey = -movey / 100 * (ex - 200) + clientHeight / 2;
                }
                if (300 <= ex && ex < 500) {
                    ey = movey / 100 * (ex - 400) + clientHeight / 2;
                }
                if (500 <= ex && ex < 700) {
                    ey = -movey / 100 * (ex - 600) + clientHeight / 2;
                }
                if (700 <= ex && ex <= 800) {
                    ey = movey / 100 * (ex - 800) + clientHeight / 2;
                }
                if (movexs == 0) {
                    Ellipse(mdc, ex + movex[0] - 5, ey - 5, ex + movex[0] + 5, ey + 5);
                }
                else if (movexs == 1) {
                    Ellipse(mdc, ex + movex[1] - 5, ey - 5, ex + movex[1] + 5, ey + 5);
                }
            }
        }
        else if (curvenum == 3) {
            int y;
            hPen = CreatePen(PS_DOT, 2, RGB(col[0], col[1], col[2])); // GDI: 펜 만들기
            oldPen = (HPEN)SelectObject(mdc, hPen); // 새로운 펜 선택하기
            MoveToEx(mdc, movex[0], clientHeight / 2, NULL);
            for (int x = 0; x <= 800; x++) {
                if (0 <= x && x < 200) {
                    y = clientHeight / 2 + movey;
                }
                if (200 <= x && x < 400) {
                    y = clientHeight / 2 - movey;
                }
                if (400 <= x && x < 600) {
                    y = clientHeight / 2 + movey;
                }
                if (600 <= x && x < 800) {
                    y = clientHeight / 2 - movey;
                }
                if (x >= 800) {
                    y = clientHeight / 2 + movey;
                }
                LineTo(mdc, x + movex[0], y);
            }
            MoveToEx(mdc, movex[1], clientHeight / 2, NULL);
            for (int x = 0; x <= 800; x++) {
                if (0 <= x && x < 200) {
                    y = clientHeight / 2 + movey;
                }
                if (200 <= x && x < 400) {
                    y = clientHeight / 2 - movey;
                }
                if (400 <= x && x < 600) {
                    y = clientHeight / 2 + movey;
                }
                if (600 <= x && x < 800) {
                    y = clientHeight / 2 - movey;
                }
                if (x >= 800) {
                    y = clientHeight / 2 + movey;
                }
                LineTo(mdc, x + movex[1], y);
            }
            SelectObject(mdc, oldPen); // 이전의 펜으로 돌아감
            DeleteObject(hPen);
            if (cirmove == 1) {
                if (0 <= ex && ex < 200) {
                    ey = clientHeight / 2 + movey;
                }
                if (200 <= ex && ex < 400) {
                    ey = clientHeight / 2 - movey;
                }
                if (400 <= ex && ex < 600) {
                    ey = clientHeight / 2 + movey;
                }
                if (600 <= ex && ex < 800) {
                    ey = clientHeight / 2 - movey;
                }
                if (ex >= 800) {
                    ey = clientHeight / 2 + movey;
                }
                if (movexs == 0) {
                    Ellipse(mdc, ex + movex[0] - 5, ey - 5, ex + movex[0] + 5, ey + 5);
                }
                else if (movexs == 1) {
                    Ellipse(mdc, ex + movex[1] - 5, ey - 5, ex + movex[1] + 5, ey + 5);
                }
            }
        }

        BitBlt(hdc, 0, 0, rect.right, rect.bottom, mdc, 0, 0, SRCCOPY);
        EndPaint(hWnd, &ps);
        break;
    case WM_TIMER:
        switch (wParam) {
        case 1:
            if (sx == 1) {
                if (curvenum == 0) {
                    if (movexs == 0) {
                        movex[0] += 5;
                        movex[1] += 5;
                        if (movex[0] >= 1080) {
                            movexs = 1;
                            movex[0] = -1080;
                        }
                    }
                    else if (movexs == 1) {
                        movex[0] += 5;
                        movex[1] += 5;
                        if (movex[1] >= 1080) {
                            movexs = 0;
                            movex[1] = -1080;
                        }
                    }
                }
                if (curvenum == 1) {
                    if (movexs == 0) {
                        movex[0] += 5;
                        movex[1] += 5;
                        if (movex[0] >= 755) {
                            movexs = 1;
                            movex[0] = -755;
                        }
                    }
                    else if (movexs == 1) {
                        movex[0] += 5;
                        movex[1] += 5;
                        if (movex[1] >= 755) {
                            movexs = 0;
                            movex[1] = -755;
                        }
                    }
                }
                if (curvenum == 2 || curvenum == 3) {
                    if (movexs == 0) {
                        movex[0] += 5;
                        movex[1] += 5;
                        if (movex[0] >= 800) {
                            movexs = 1;
                            movex[0] = -800;
                        }
                    }
                    else if (movexs == 1) {
                        movex[0] += 5;
                        movex[1] += 5;
                        if (movex[1] >= 800) {
                            movexs = 0;
                            movex[1] = -800;
                        }
                    }
                }
            }
            break;
        case 2:
            if (sy == 1) {
                if (moveys == 0) {
                    movey -= 5;
                    if (movey <= 0) {
                        moveys = 1;
                    }
                }
                else if (moveys == 1) {
                    movey += 5;
                    if (movey >= clientHeight / 2) {
                        moveys = 0;
                    }
                }
            }
            if (cirmove == 1) {

                if (curvenum == 0) {
                    ex += 5;
                    if (ex >= 1080) {
                        ex = 0;
                        cirmove = 0;
                    }
                }
                if (curvenum == 1) {
                    mx1 += 0.7;
                    sprx++;
                    if (sprx > 1080) {
                        sprx = 0;
                        mx1 = 0;
                        cirmove = 0;
                    }
                }
                if (curvenum == 2 || curvenum == 3) {
                    ex += 5;
                    if (ex >= 800) {
                        ex = 0;
                        cirmove = 0;
                    }
                }
            }
            break;
        }
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}
BOOL CALLBACK Dlalog_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HWND hbutton;
    static int one = 0;
    switch (iMsg) {
    case WM_INITDIALOG:
        //hbutton = GetDlgItem(hDlg, ID_TEST); //--- ID_TEST 라는 id의 버튼 핸들 가져오기
        //EnableWindow(hbutton, FALSE);
        CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO4, IDC_RADIO1);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_CHECK1:
            if (one == 0) {
                col[0] = 0;
                col[1] = 0;
                col[2] = 0;
                one = 1;
            }
            if (col[0] == 255) {
                col[0] = 0;
            }
            else {
                col[0] = 255;
            }
            break;
        case IDC_CHECK2:
            if (one == 0) {
                col[0] = 0;
                col[1] = 0;
                col[2] = 0;
                one = 1;
            }
            if (col[1] == 255) {
                col[1] = 0;
            }
            else {
                col[1] = 255;
            }
            break;
        case IDC_CHECK3:
            if (one == 0) {
                col[0] = 0;
                col[1] = 0;
                col[2] = 0;
                one = 1;
            }
            if (col[2] == 255) {
                col[2] = 0;
            }
            else {
                col[2] = 255;
            }
            break;
        case IDC_CHECK4:
            col[0] = 255 - col[0];
            col[1] = 255 - col[1];
            col[2] = 255 - col[2];
            break;
        case IDC_BUTTON5:
            cirmove = 1;
            break;
        case IDC_RADIO1:
            cirmove = 0;
            movexs = 0;
            moveys = 0;
            sx = 0;
            sy = 0;
            movex[0] = 0;
            movex[1] = -1080;
            movey = 100;
            curvenum = 0;
            break;
        case IDC_RADIO3:
            cirmove = 0;
            movexs = 0;
            moveys = 0;
            sx = 0;
            sy = 0;
            movex[0] = -100;
            movex[1] = -855;
            movey = 100;
            curvenum = 1;
            break;
        case IDC_RADIO2:
            cirmove = 0;
            movexs = 0;
            moveys = 0;
            sx = 0;
            sy = 0;
            movex[0] = 0;
            movex[1] = -800;
            movey = 100;
            curvenum = 2;
            break;
        case IDC_RADIO4:
            cirmove = 0;
            movexs = 0;
            moveys = 0;
            sx = 0;
            sy = 0;
            movex[0] = 0;
            movex[1] = -800;
            movey = 100;
            curvenum = 3;
            break;
        case IDC_BUTTON1:
            if (sx == 0) {
                sx = 1;
            }
            else if (sx == 1) {
                sx = 0;
            }
            break;
        case IDC_BUTTON2:
            if (sy == 0) {
                sy = 1;
            }
            else if (sy == 1) {
                sy = 0;
            }
            break;
        case IDC_BUTTON3:
            sx = 0;
            sy = 0;
            break;
        case IDC_BUTTON4:
            movexs = 0;
            moveys = 0;
            sx = 0;
            sy = 0;
            curvenum = 5;
            cirmove = 0;
            break;
        case IDOK: //--- 버튼
            MessageBox(hDlg, L"Reset", L"Reset1", MB_OK);
            movexs = 0;
            moveys = 0;
            sx = 0;
            sy = 0;
            curvenum = 5;
            cirmove = 0;
            break;
        case IDCANCEL: //--- 버튼
            PostQuitMessage(0);
            //EndDialog(hDlg, 0);
            break;
        }
        InvalidateRect(GetParent(hDlg), NULL, TRUE);
        break;
    case WM_CLOSE:
        EndDialog(hDlg, 0);
        break;
    }
    return 0;
}