#define _CRT_SECURE_NO_WARNINGS
#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include <stdlib.h>
#include <tchar.h>
#include <string.h>
#include <cmath>
#include "resource.h"

#define EDIT1 300
#define RIST1 301
#define BUTTON1 400
#define BUTTON2 401
#define BUTTON3 402
#define BUTTON4 403
#define BUTTON5 404
#define BUTTON6 405

HBRUSH hBrush, oldBrush;


HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"윈플 7-2";
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);         // 프록시저 함수 선언
LRESULT CALLBACK ChildProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);         // 프록시저 함수 선언
#define IDC_BUTTON 100
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
    HWND hwnd;
    MSG Message;
    WNDCLASSEX WndClass;
    g_hInst = hInstance;

    WndClass.cbSize = sizeof(WndClass);
    WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    WndClass.lpfnWndProc = (WNDPROC)WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_HAND);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = L"ParentClass";
    WndClass.hIconSm = LoadIcon(NULL, IDI_QUESTION);
    RegisterClassEx(&WndClass);

    // 버튼 윈도우 클래스 등록
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = (WNDPROC)ChildProc;
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszClassName = L"ChildClass";
    RegisterClassEx(&WndClass);

    hwnd = CreateWindow(L"ParentClass", lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 1536, 768, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return Message.wParam;
}
int list_num, Select_list;       // 리스트 박스의 맵 인덱스 값
bool Select, Move;
int Select_list_arr[5];
int xPos;

TCHAR str[50];
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static HWND child_hWnd;
    static HWND Edit1, Rist1;
    HWND Button[6];
    HBITMAP hBit[2];
    static TCHAR str[30] = L"Select: ";
    static int arr_num;

    switch (iMsg)
    {
    case WM_CREATE:
     /*   hBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(55));
        hBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(55));*/
        child_hWnd = CreateWindow(L"ChildClass", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 100, 768, 512, hwnd, (HMENU)0, g_hInst, NULL);



        Edit1 = CreateWindow(L"edit", str, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_READONLY, 340, 20, 256, 60, hwnd, (HMENU)EDIT1, g_hInst, NULL);

        Rist1 = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_STANDARD | LBS_SORT, 968, 100, 250, 320, hwnd, (HMENU)RIST1, g_hInst, NULL);

        Button[0] = CreateWindow(L"button", L"<-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON , 40, 364, 40, 40, hwnd, (HMENU)BUTTON1, g_hInst, NULL);
        Button[1] = CreateWindow(L"button", L"->", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 888, 364, 40, 40, hwnd, (HMENU)BUTTON2, g_hInst, NULL);
        Button[2] = CreateWindow(L"button", L"Select", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 968, 430, 250, 40, hwnd, (HMENU)BUTTON3, g_hInst, NULL);
        Button[3] = CreateWindow(L"button", L"Save", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 968, 510, 250, 40, hwnd, (HMENU)BUTTON4, g_hInst, NULL);
        Button[4] = CreateWindow(L"button", L"Move", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 968, 590, 250, 40, hwnd, (HMENU)BUTTON5, g_hInst, NULL);
        Button[5] = CreateWindow(L"button", L"Stop", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 968, 670, 250, 40, hwnd, (HMENU)BUTTON6, g_hInst, NULL);

        SendMessage(Rist1, LB_INSERTSTRING, 0, (LPARAM)L"1");
        SendMessage(Rist1, LB_INSERTSTRING, 0, (LPARAM)L"2");
        SendMessage(Rist1, LB_INSERTSTRING, 0, (LPARAM)L"3");
        SendMessage(Rist1, LB_INSERTSTRING, 0, (LPARAM)L"4");
        SendMessage(Rist1, LB_INSERTSTRING, 0, (LPARAM)L"5");

        SendMessage(Button[0], BM_SETIMAGE, 0 /*IMAGE_BITMAP*/, (LPARAM)hBit[0]);
        SendMessage(Button[1], BM_SETIMAGE, 0 /*IMAGE_BITMAP*/, (LPARAM)hBit[1]);

        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case RIST1:
            switch (HIWORD(wParam))
            {
            case LBN_SELCHANGE:
                list_num = SendMessage(Rist1, LB_GETCURSEL, 0, 0);
                break;
            }
            break;

        case BUTTON1:  // <- 화살표
            if (arr_num == 0)
                arr_num = 5;
            Select_list = Select_list_arr[--arr_num];
            break;
        case BUTTON2:  // -> 화살표
            if (arr_num == 4)
                arr_num = -1;
            Select_list = Select_list_arr[++arr_num];
            break;
        case BUTTON3:  // Select
            Select = TRUE;
            Select_list = list_num;
            _tcscpy(str, L"");
            if (Select_list == 0)
            {
                _tcscat(str, L"Select: 1");
            }
            else if (Select_list == 1)
            {
                _tcscat(str, L"Select: 2");
            }
            else if (Select_list == 2)
            {
                _tcscat(str, L"Select: 3");
            }
            else if (Select_list == 3)
            {
                _tcscat(str, L"Select: 4");
            }
            else if (Select_list == 4)
            {
                _tcscat(str, L"Select: 5");
            }
            SetDlgItemText(hwnd, EDIT1, str);
            break;
        case BUTTON4:  // Save
            if (arr_num == 5)
                break;
            Select_list_arr[arr_num++] = list_num;
            break;
        case BUTTON5:  // Move
            Move = TRUE;
            SetTimer(hwnd, 0, 5, NULL);
            break;
        case BUTTON6:  // Stop
            KillTimer(hwnd, 0);
            break;
        }
        InvalidateRect(child_hWnd, NULL, FALSE);
        break;
    case WM_TIMER:
        if (xPos == 768 * 5)
            xPos = 0;
        xPos += 5;
        InvalidateRect(child_hWnd, NULL, FALSE);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}




LRESULT CALLBACK ChildProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc, MemDC, MemDCImage;
    static HBITMAP hBit, OldBit[2];
    static HBITMAP BackGroundBitmap[5];
    PAINTSTRUCT ps;
    static RECT rt;


    switch (uMsg) {
    case WM_CREATE:
        GetClientRect(hwnd, &rt);
        break;
    case WM_TIMER:

        InvalidateRect(hwnd, NULL, TRUE);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        hBit = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
        BackGroundBitmap[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
        BackGroundBitmap[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
        BackGroundBitmap[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
        BackGroundBitmap[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
        BackGroundBitmap[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
        MemDC = CreateCompatibleDC(hdc); //--- 메모리 DC 만들기
        MemDCImage = CreateCompatibleDC(MemDC);
        OldBit[0] = (HBITMAP)SelectObject(MemDC, hBit); //--- MemDC와 hBit 를 호환되게 선택함


        if (Select)
        {
            OldBit[1] = (HBITMAP)SelectObject(MemDCImage, BackGroundBitmap[Select_list]);
            StretchBlt(MemDC, 0, 0, 768, 512, MemDCImage, 0, 0, 768, 512, SRCCOPY);
        }
        if (Move)
        {
            for (int i = 0; i < 5; i++)
            {
                OldBit[1] = (HBITMAP)SelectObject(MemDCImage, BackGroundBitmap[Select_list_arr[i]]);
                StretchBlt(MemDC, i * 768 - xPos, 0, 768, 512, MemDCImage, 0, 0, 768, 512, SRCCOPY);
            }
            OldBit[1] = (HBITMAP)SelectObject(MemDCImage, BackGroundBitmap[Select_list_arr[0]]);
            StretchBlt(MemDC, 768 * 5 - xPos, 0, 768, 512, MemDCImage, 0, 0, 768, 512, SRCCOPY);
        }

        BitBlt(hdc, 0, 0, rt.right, rt.bottom, MemDC, 0, 0, SRCCOPY);
        EndPaint(hwnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}