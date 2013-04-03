#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <commctrl.h>
#include <math.h>
#include "resource.h"

/* IDB buttons 1-10 */
#define IDB_BUTTON1 1
#define IDB_BUTTON2 2
#define IDB_BUTTON3 3
#define IDB_BUTTON4 4
#define IDB_BUTTON5 5
#define IDB_BUTTON6 6
#define IDB_BUTTON7 7

/* IDB buttons 11-21 */
#define IDR_RADIOBUTTON1 11
#define IDR_RADIOBUTTON2 12
#define IDR_RADIOBUTTON3 13
#define IDR_RADIOBUTTON4 14
#define IDR_RADIOBUTTON5 15
#define IDR_RADIOBUTTON6 16
#define IDR_RADIOBUTTON7 17


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "Lab#3";
HBITMAP hbmp;
HINSTANCE hInst ;
int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow) {
    HWND hwnd;                                                                  /* This is the handle for our window */
    MSG messages;                                                               /* Here messages to the application are saved */
    WNDCLASS wincl;                                                             /* Data structure for the windowclass */
    hInst = hThisInstance ;
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */

    wincl.hbrBackground = CreateSolidBrush (RGB (255, 255, 255));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClass (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindow (
                                                                                /* Extended possibilites for variation */
           szClassName,                                                         /* Classname */
           "Code::Blocks Template Windows App",                                 /* Title Text */
           WS_OVERLAPPEDWINDOW,                                                 /* default window */
           CW_USEDEFAULT,                                                       /* Windows decides the position */
           CW_USEDEFAULT,                                                       /* where the window ends up on the screen */
           700,                                                                 /* The programs width */
           650,                                                                 /* and height in pixels */
           HWND_DESKTOP,                                                        /* The window is a child-window to desktop */
           NULL,                                                                /* No menu */
           hThisInstance,                                                       /* Program Instance handler */
           NULL                                                                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */

    ShowWindow (hwnd, nCmdShow);
    UpdateWindow (hwnd) ;
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0)) {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

void Triangle (HDC hdc, POINT pt[]) {
    SelectObject (hdc, GetStockObject (WHITE_BRUSH));
    Polygon (hdc, pt, 3);
    SelectObject (hdc, GetStockObject (WHITE_BRUSH));
}

void Rectangle (HDC hdc, POINT pt[]) {
    SelectObject (hdc, GetStockObject (WHITE_BRUSH));
    Polygon (hdc, pt, 4);
    SelectObject (hdc, GetStockObject (WHITE_BRUSH));
}

void Circle(HDC hdc,int x, int y, int rad, bool filled) {
    if(filled) {
        SelectObject (hdc, GetStockObject (BLACK_BRUSH));
    } else {
        SelectObject (hdc, GetStockObject (WHITE_BRUSH));
    }
    Ellipse(hdc, x, y, rad + x, rad + y);
    SelectObject (hdc, GetStockObject (WHITE_BRUSH));
}

void DrawBezier (HDC hdc, POINT apt[]) {
    PolyBezier (hdc, apt, 4);
    MoveToEx (hdc, apt[0].x, apt[0].y, NULL);
    LineTo (hdc, apt[1].x, apt[1].y);
    MoveToEx (hdc, apt[2].x, apt[2].y, NULL);
    LineTo (hdc, apt[3].x, apt[3].y);
}

void updateColorPreview(HDC hdc, COLORREF rgb, int xLeft, int yTop) {
    HBRUSH hBrush = CreateSolidBrush(rgb);
    SelectObject(hdc, hBrush);
    Rectangle(hdc, xLeft, yTop, xLeft + 75, yTop + 45);
    DeleteObject(hBrush);
}


/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static HWND hwndButton1, hwndButton2, hwndButton3, hwndButton4, hwndButton5, hwndButton6, hwndButton7;
    static HWND hwndCheckBox3, hwndCheckBox5, hwndCheckBox6;
    static HBITMAP hBitmap;
    static int cxClient, cyClient, cxSource, cySource;
    BITMAP bitmap;
    HDC hdc, hdcMem;
    int x, y;
    PAINTSTRUCT ps;
    LPDRAWITEMSTRUCT pdis;
    POINT pt[3];
    RECT rc,rect, rectTemp;
    HPEN hPen;
    HBRUSH hBrush;
    HBRUSH hbr;

    UINT coll = 0;
    COLORREF colRef;

    HDC hDC = GetDC(hwnd);
	static BOOL IsDrawing = FALSE;
	static int StartX, StartY;
	static int EndX, EndY;

	static BOOL isLine, isFreeHand, isEllipse, isTriangle, isRectangle, isEraser;
    int state;

    switch (message) {                                                           /* handle the messages */

        case WM_CREATE:
            hBitmap = LoadBitmap (hInst, TEXT ("header"));
            GetObject (hBitmap, sizeof (BITMAP), &bitmap);
            cxSource = bitmap.bmWidth;
            cySource = bitmap.bmHeight;

            hwndButton1 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                        50, 60, 30, 30,
                                        hwnd, (HMENU) IDB_BUTTON1, hInst, NULL);
            hwndButton2 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                        50, 95, 30, 30,
                                        hwnd, (HMENU) IDB_BUTTON2, hInst, NULL);
            hwndCheckBox3 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                          WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                                          20, 130, 13, 13,
                                          hwnd, (HMENU) IDR_RADIOBUTTON3, hInst, NULL);
            hwndButton3 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                        50, 130, 30, 30,
                                        hwnd, (HMENU) IDB_BUTTON3, hInst, NULL);
            hwndButton4 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                        50, 165, 30, 30,
                                        hwnd, (HMENU) IDB_BUTTON4, hInst, NULL);
            hwndCheckBox5 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                          WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                                          20, 200, 13, 13,
                                          hwnd, (HMENU) IDR_RADIOBUTTON5, hInst, NULL);
            hwndButton5 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                        50, 200, 30, 30,
                                        hwnd, (HMENU) IDB_BUTTON5, hInst, NULL);
            hwndCheckBox6 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                          WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                                          20, 235, 13, 13,
                                          hwnd, (HMENU) IDR_RADIOBUTTON6, hInst, NULL);
            hwndButton6 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                        50, 235, 30, 30,
                                        hwnd, (HMENU) IDB_BUTTON6, hInst, NULL);
            hwndButton7 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                        50, 270, 30, 30,
                                        hwnd, (HMENU) IDB_BUTTON7, hInst, NULL);
            return 0 ;
        case WM_DRAWITEM :
            pdis = (LPDRAWITEMSTRUCT) lParam;
            FillRect (pdis->hDC, &pdis->rcItem, (HBRUSH) GetStockObject (WHITE_BRUSH));
            FrameRect (pdis->hDC, &pdis->rcItem, (HBRUSH) GetStockObject (BLACK_BRUSH));
            switch (pdis->CtlID) {
                case IDB_BUTTON1 :

                    hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                    SelectObject(pdis->hDC, hPen);
                    MoveToEx (pdis->hDC, 5, 18, NULL) ;
                    LineTo (pdis->hDC, 19, 4) ;

                    hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
                    SelectObject(pdis->hDC, hPen);
                    MoveToEx (pdis->hDC, 8, 21, NULL) ;
                    LineTo (pdis->hDC, 22, 7) ;

                    hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
                    SelectObject(pdis->hDC, hPen);
                    MoveToEx (pdis->hDC, 11, 25, NULL);
                    LineTo (pdis->hDC, 25, 11);
                    break;
                case IDB_BUTTON2 :
                    pt[0].x = 3; pt[0].y = 3;
                    pt[1].x = 3; pt[1].y = 27;
                    pt[2].x = 27; pt[2].y = 15;
                    pt[3].x = 27; pt[3].y = 3;
                    DrawBezier (pdis->hDC, pt);
                    break;
                case IDB_BUTTON3 :
                    state = SendMessage(hwndCheckBox3, BM_GETCHECK, 0, 0);
                    if(state == BST_CHECKED) {
                        Circle (pdis->hDC, 3, 3, 24, true);
                    } else {
                        Circle (pdis->hDC, 3, 3, 24, false);
                    }
                    break;
                case IDB_BUTTON4 :

                    hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
                    SelectObject(pdis->hDC, hPen);
                    MoveToEx (pdis->hDC, 8, 21, NULL) ;
                    LineTo (pdis->hDC, 22, 7) ;

                    hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                    SelectObject(pdis->hDC, hPen);
                    MoveToEx (pdis->hDC, 8, 21, NULL);
                    LineTo (pdis->hDC, 3, 24);
                    break;
                case IDB_BUTTON5 :

                    pt[0].x = 4;  pt[0].y = 26;
                    pt[1].x = 26; pt[1].y = 26;
                    pt[2].x = 15; pt[2].y = 3;
                    Triangle (pdis->hDC, pt);
                    break;
                case IDB_BUTTON6 :

                    pt[0].x = 3; pt[0].y = 3;
                    pt[1].x = 3; pt[1].y = 26;
                    pt[2].x = 26; pt[2].y = 26;
                    pt[3].x = 26; pt[3].y = 3;
                    Rectangle(pdis->hDC, pt);
                    break;
                case IDB_BUTTON7 :

                    pt[0].x = 3; pt[0].y = 20;
                    pt[1].x = 6; pt[1].y = 15;
                    pt[2].x = 16; pt[2].y = 15;
                    pt[3].x = 13; pt[3].y = 20;
                    Rectangle(pdis->hDC, pt);

                    hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                    SelectObject(pdis->hDC, hPen);
                    MoveToEx (pdis->hDC, 6, 15, NULL);
                    LineTo (pdis->hDC, 15, 11);
                    MoveToEx (pdis->hDC, 16, 15, NULL);
                    LineTo (pdis->hDC, 25, 11);
                    MoveToEx (pdis->hDC, 13, 20, NULL);
                    LineTo (pdis->hDC, 22, 16);
                    MoveToEx (pdis->hDC, 15, 11, NULL);
                    LineTo (pdis->hDC, 25, 11);
                    MoveToEx (pdis->hDC, 25, 11, NULL);
                    LineTo (pdis->hDC, 21, 17);
                    break;
            }

            if (pdis->itemState & ODS_SELECTED)
                InvertRect (pdis->hDC, &pdis->rcItem);
            if (pdis->itemState & ODS_FOCUS)                            // Draw a focus rectangle if the button has the focus
                DrawFocusRect (pdis->hDC, &pdis->rcItem) ;
            return 0;
        case WM_LBUTTONDOWN:

                if((LOWORD(lParam) > 15) && (HIWORD(lParam) > 400) &&
                   (LOWORD(lParam) < 90) && (HIWORD(lParam) <550) ) {
                    coll = 255* (HIWORD(lParam) - 400)/150;
                    updateColorPreview(hDC, RGB(coll, coll, coll), 15, 350);
                    colRef = RGB(coll, coll, coll);
                    return 0;
                }
                hDC = GetDC(hwnd);
                StartX = LOWORD(lParam);
                StartY = HIWORD(lParam);
                EndX = LOWORD(lParam);
                EndY = HIWORD(lParam);
                if(isLine) {
                    hPen = CreatePen(PS_SOLID, 1, colRef);
                    hbr = CreateSolidBrush(colRef);
                    SelectObject(hDC, hPen);
                    SelectObject(hDC, hbr);
                    SetROP2(hDC, R2_XORPEN);
                    MoveToEx(hDC, StartX, StartY, NULL);
                    LineTo(hDC, EndX, EndY);
                    IsDrawing = TRUE;
					DeleteObject(hPen);
                    DeleteObject(hbr);
                }
                if(isFreeHand) {
                    hPen = CreatePen(PS_SOLID, 1, colRef);
                    hbr = CreateSolidBrush(colRef);
                    SelectObject(hDC, hPen);
                    SelectObject(hDC, hbr);
                    StartX = LOWORD(lParam);
                    StartY = HIWORD(lParam);
                    IsDrawing = TRUE;
					DeleteObject(hPen);
                    DeleteObject(hbr);
                }
                if(isEllipse){
                    state = SendMessage(hwndCheckBox3, BM_GETCHECK, 0, 0);
                    hPen = CreatePen(PS_SOLID, 1, colRef);
                    if(state){
                        hbr = CreateSolidBrush(colRef);
                        SelectObject(hDC, hbr);
                    } else {
                        hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
                        SelectObject(hDC, hbr);
                    }
                    SelectObject(hDC, hPen);
                    SetROP2(hDC, R2_XORPEN);
                    Ellipse(hDC, StartX, StartY, EndX, EndY);
                    ReleaseDC(hwnd, hDC);
                    IsDrawing = TRUE;
            		DeleteObject(hPen);
                    DeleteObject(hbr);
                }
                if (isTriangle) {
                    state = SendMessage(hwndCheckBox5, BM_GETCHECK, 0, 0);
                    hPen = CreatePen(PS_SOLID, 1, colRef);
                    if (state){
                        hbr = CreateSolidBrush(colRef);
                        SelectObject(hDC, hbr);
                    } else {
                        hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
                        SelectObject(hDC, hbr);
                    }
                    SelectObject(hDC, hPen);
                    SetROP2(hDC, R2_XORPEN);

                    pt[0].x = StartX;  pt[0].y = EndY;
                    pt[1].x = (EndX + StartX) / 2; pt[1].y = StartY;
                    pt[2].x = EndX; pt[2].y = EndY;
                    Triangle(hDC, pt);
                    ReleaseDC(hwnd, hDC);
                    IsDrawing = TRUE;
            		DeleteObject(hPen);
                    DeleteObject(hbr);
                }
                if (isRectangle) {
                    hPen = CreatePen(PS_SOLID, 1, colRef);
                    state = SendMessage(hwndCheckBox6, BM_GETCHECK, 0, 0);
                    if (state) {
                        hbr = CreateSolidBrush(colRef);
                    } else {
                        hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
                    }
                    SelectObject(hDC, hbr);
                    SelectObject(hDC, hPen);
                    SetROP2(hDC, R2_XORPEN);

                    pt[0].x = StartX; pt[0].y = StartY;
                    pt[1].x = StartX; pt[1].y = EndY;
                    pt[2].x = EndX; pt[2].y = EndY;
                    pt[3].x = EndX; pt[3].y = StartY;
                    Rectangle(hDC, pt);
                    ReleaseDC(hwnd, hDC);
                    IsDrawing = TRUE;
            		DeleteObject(hPen);
                    DeleteObject(hbr);
                }
                if (isEraser) {
                    rect.left = StartX - 15;
                    rect.right = StartX + 15;
                    rect.top = StartY - 15;
                    rect.bottom = StartY + 15 ;
                    InvalidateRect(hwnd, &rect, FALSE);
                    SendMessage(hwnd, WM_PAINT, 0, 0);
                    ValidateRect(hwnd, &rect);
                }
                ReleaseDC(hwnd, hDC);
                return 0;

        case WM_MOUSEMOVE:

            if(IsDrawing) {
                hDC = GetDC(hwnd);

                if(isLine) {
                    hPen = CreatePen(PS_SOLID, 1, colRef);
                    SelectObject(hDC, hPen);
                    SetROP2(hDC, R2_NOTXORPEN);
                    MoveToEx(hDC, StartX, StartY, NULL);
                    LineTo(hDC, EndX, EndY);
                    EndX = LOWORD(lParam);
                    EndY = HIWORD(lParam);
                    MoveToEx(hDC, StartX, StartY, NULL);
                    LineTo(hDC, EndX, EndY);
					DeleteObject(hPen);
                }
                if(isFreeHand)  {
                    hPen = CreatePen(PS_SOLID, 1, colRef);
                    SelectObject(hDC, hPen);
                    MoveToEx(hDC, EndX, EndY, NULL);
				    EndX = LOWORD(lParam);
				    EndY = HIWORD(lParam);
				    LineTo(hDC, EndX, EndY);
					DeleteObject(hPen);
                }
                if(isEllipse) {
                    hPen = CreatePen(PS_SOLID, 1, colRef);
                    state = SendMessage(hwndCheckBox3, BM_GETCHECK, 0, 0);
                    if(state) {
                        hbr = CreateSolidBrush(colRef);
                    } else {
                        hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
                    }
                    SelectObject(hDC, hbr);
                    SelectObject(hDC, hPen);
                    SetROP2(hDC, R2_NOTXORPEN);
                    Ellipse(hDC, StartX, StartY, EndX, EndY);
                    EndX = LOWORD(lParam);
                    EndY = HIWORD(lParam);
                    Ellipse(hDC, StartX, StartY, EndX, EndY);
                    DeleteObject(hbr);
                    DeleteObject(hPen);
                }
                if(isTriangle) {
                    hPen = CreatePen(PS_SOLID, 1, colRef);
                    state = SendMessage(hwndCheckBox5, BM_GETCHECK, 0, 0);
                    if (state){
                        hbr = CreateSolidBrush(colRef);
                    } else {
                        hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
                    }
                    SelectObject(hDC, hbr);
                    SelectObject(hDC, hPen);
                    SetROP2(hDC, R2_NOTXORPEN);
                    pt[0].x = StartX;  pt[0].y = EndY;
                    pt[1].x = (EndX + StartX) / 2; pt[1].y = StartY;
                    pt[2].x = EndX; pt[2].y = EndY;
                    Triangle(hDC, pt);
                    EndX = LOWORD(lParam);
                    EndY = HIWORD(lParam);
                    pt[0].x = StartX;  pt[0].y = EndY;
                    pt[1].x = (EndX + StartX) / 2; pt[1].y = StartY;
                    pt[2].x = EndX ; pt[2].y = EndY;
                    Triangle(hDC, pt);
                    DeleteObject(hPen);
                    DeleteObject(hbr);
                }
                if (isRectangle) {
                    hPen = CreatePen(PS_SOLID, 1, colRef);
                    state = SendMessage(hwndCheckBox6, BM_GETCHECK, 0, 0);
                    hPen = CreatePen(PS_SOLID, 1, colRef);
                    if(state) {
                        hbr = CreateSolidBrush(colRef);
                        SelectObject(hDC, hbr);
                    } else {
                        hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
                        SelectObject(hDC, hbr);
                    }
                    SelectObject(hDC, hPen);
                    SetROP2(hDC, R2_NOTXORPEN);
                    pt[0].x = StartX ;  pt[0].y = StartY;
                    pt[1].x = StartX; pt[1].y = EndY ;
                    pt[2].x = EndX ; pt[2].y = EndY ;
                    pt[3].x = EndX ; pt[3].y = StartY ;
                    Rectangle(hDC, pt);
                    EndX = LOWORD(lParam);
                    EndY = HIWORD(lParam);
                    pt[0].x = StartX;  pt[0].y = StartY;
                    pt[1].x = StartX; pt[1].y = EndY;
                    pt[2].x = EndX; pt[2].y = EndY;
                    pt[3].x = EndX; pt[3].y = StartY;
                    Rectangle(hDC, pt);
                    DeleteObject(hPen);
                    DeleteObject(hbr);
                }
                if (isEraser) {
                    SetROP2(hDC, R2_NOTXORPEN);
                    rect.left = StartX - 5;
                    rect.right = StartX + 5;
                    rect.top = StartY - 5;
                    rect.bottom = StartY + 5 ;
                    InvalidateRect(hwnd, &rect, FALSE);
                    SendMessage(hwnd, WM_PAINT, 0, 0);
                    ValidateRect(hwnd, &rect);
                    EndX = LOWORD(lParam);
                    EndY = HIWORD(lParam);
                    rect.left = EndX - 5;
                    rect.right = EndX + 5;
                    rect.top = EndY - 5;
                    rect.bottom = EndY + 5 ;
                    InvalidateRect(hwnd, &rect, FALSE);
                    SendMessage(hwnd, WM_PAINT, 0, 0);
                    ValidateRect(hwnd, &rect);

                }
            }
            ReleaseDC(hwnd, hDC);
            break;
        case WM_LBUTTONUP:

            hDC = GetDC(hwnd);
            EndX = LOWORD(lParam);
            EndY = HIWORD(lParam);
            SetROP2(hDC, R2_XORPEN);
            MoveToEx(hDC, StartX, StartY, NULL);
            LineTo(hDC, EndX, EndY);
            IsDrawing = FALSE;
            ReleaseDC(hwnd, hDC);
            break;
        case WM_SIZE:
            cxClient = LOWORD (lParam);
            cyClient = HIWORD (lParam);
            return 0;
        case WM_COMMAND :
            switch (LOWORD(wParam)){
                case IDB_BUTTON1:
                    isEraser = false;
                    isRectangle = false;
                    isLine = true;
                    isFreeHand = false;
                    isEllipse = false;
                    isTriangle = false;
                    break;
                case IDB_BUTTON3:
                    isEraser = false;
                    isRectangle = false;
                    isLine = false;
                    isFreeHand = false;
                    isEllipse = true;
                    isTriangle = false;
                    break;
                case IDB_BUTTON4:
                    isEraser = false;
                    isRectangle = false;
                    isLine = false;
                    isFreeHand = true;
                    isEllipse = false;
                    isTriangle = false;
                    break;
                case IDB_BUTTON5:
                    isEraser = false;
                    isRectangle = false;
                    isTriangle = true;
                    isLine = false;
                    isFreeHand = false;
                    isEllipse = false;
                    break;
                case IDB_BUTTON6:
                    isEraser = false;
                    isRectangle = true;
                    isTriangle = false;
                    isLine = false;
                    isFreeHand = false;
                    isEllipse = false;
                    break;
                case IDB_BUTTON7:
                    isEraser = true;
                    isRectangle = false;
                    isTriangle = false;
                    isLine = false;
                    isFreeHand = false;
                    isEllipse = false;
                    break;
            }
            GetWindowRect (hwnd, &rc);
            return 0;
        case WM_PAINT:
            /*draw the header*/
            hdc = BeginPaint (hwnd, &ps);
            hdcMem = CreateCompatibleDC (hdc);
            SelectObject (hdcMem, hBitmap);
            for (y = 0 ; y < cySource ; y += cySource)
                for (x = 0 ; x < cxSource ; x += cxSource)
                    BitBlt (hdc, x, y, cxSource, cySource, hdcMem, 0, 0, SRCCOPY);
            DeleteDC (hdcMem) ;
            updateColorPreview(hdc, colRef, 15, 350);
            rectTemp.top = 400;
            rectTemp.bottom = 550;
            rectTemp.left =15;
            rectTemp.right =90;

            for(int l = 0; l < 150; l++) {
                int r;
                r = l * 255 / 150;
                rectTemp.top = 400 + l;
                rectTemp.bottom = 400 + l + 1;
                hBrush = CreateSolidBrush(RGB(r, r, r));
                FillRect(hdc, &rectTemp, hBrush);
                DeleteObject(hBrush);
            }

            EndPaint (hwnd, &ps) ;
            return 0;
        case WM_DESTROY:
            DeleteObject (hBitmap) ;
            PostQuitMessage (0);                                                /* send a WM_QUIT to the message queue */
            break;
        default:                                                                /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
