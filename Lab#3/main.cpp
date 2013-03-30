#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <commctrl.h>
#include <math.h>
#include "resource.h"

#define NUM 1000
#define TWOPI (2 * 3.14159)
/* IDB buttons 1-10 */
#define IDB_BUTTON1 1
#define IDB_BUTTON2 2
#define IDB_BUTTON3 3
#define IDB_BUTTON4 4
#define IDB_BUTTON5 5
#define IDB_BUTTON6 6
#define IDB_BUTTON7 7
#define IDB_BUTTON8 8
#define IDB_BUTTON9 9

/* IDB buttons 11-21 */
#define IDR_RADIOBUTTON1 11
#define IDR_RADIOBUTTON2 12
#define IDR_RADIOBUTTON3 13
#define IDR_RADIOBUTTON4 14
#define IDR_RADIOBUTTON5 15
#define IDR_RADIOBUTTON6 16
#define IDR_RADIOBUTTON7 17
#define IDR_RADIOBUTTON8 18
#define IDR_RADIOBUTTON9 19
/*
//SYSTEm Menus 1-10;
#define IDM_SYS_ABOUT 1
#define IDM_SYS_HELP 2
#define IDM_SYS_REMOVE 3

//SCrollBars 11 - 20;
#define IDC_SCROLL_BAR1 11
#define IDC_SCROLL_BAR2 12
#define IDC_SCROLL_BAR3 13

//Labels 21-30
#define IDC_LABEL1 21
#define IDC_LABEL2 22
#define IDC_LABEL3 23

//changing labels 31-40
#define IDC_VAR_LABEL1 31
#define IDC_VAR_LABEL2 32
#define IDC_VAR_LABEL3 33

//list 41-50
#define IDC_TASK_LIST 41
// imput 51-60
#define IDC_TEXT_INPUT 51
//button 61-70
#define IDC_ADD_BUTTON 61

//rectangles 71-80
#define IDC_RECTANGLE 71-80

*/

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "CodeBlocksWindowsApp";
//BITMAP bitmap1 = NULL;
HBITMAP hbmp;
HINSTANCE hInst ;
int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
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
    /* Use Windows's default colour as the background of the window
        wincl.hbrBackground = CreatePatternBrush((HBITMAP) LoadImage(0,_T("emblema2.bmp"),
                                     IMAGE_BITMAP,0,0,
                                     LR_CREATEDIBSECTION|LR_LOADFROMFILE));
    */
    wincl.hbrBackground = /*(HBRUSH) COLOR_BACKGROUND;(LONG)*/ CreateSolidBrush (RGB (255, 255, 255));

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
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */
void Triangle (HDC hdc, POINT pt[])
{
    SelectObject (hdc, GetStockObject (WHITE_BRUSH)) ;
    Polygon (hdc, pt, 3) ;
    SelectObject (hdc, GetStockObject (WHITE_BRUSH)) ;
}

void Rectangle (HDC hdc, POINT pt[])
{
    SelectObject (hdc, GetStockObject (WHITE_BRUSH)) ;
    Polygon (hdc, pt, 4) ;
    SelectObject (hdc, GetStockObject (WHITE_BRUSH)) ;
}

void Circle(HDC hdc,int x, int y, int rad, bool filled)
{
    if(filled){
        SelectObject (hdc, GetStockObject (BLACK_BRUSH));
    }
    else {
        SelectObject (hdc, GetStockObject (WHITE_BRUSH)) ;
    }
    //SelectObject (hdc, GetStockObject (WHITE_BRUSH)) ;
    Ellipse(hdc, x, y, rad + x, rad + y);
    SelectObject (hdc, GetStockObject (WHITE_BRUSH)) ;
}

void DrawBezier (HDC hdc, POINT apt[])
{
    PolyBezier (hdc, apt, 4) ;
    MoveToEx (hdc, apt[0].x, apt[0].y, NULL) ;
    LineTo (hdc, apt[1].x, apt[1].y) ;
    MoveToEx (hdc, apt[2].x, apt[2].y, NULL) ;
    LineTo (hdc, apt[3].x, apt[3].y) ;
}

void updateColorPreview(HDC hdc, COLORREF rgb, int xLeft, int yTop) {
    HBRUSH hBrush = CreateSolidBrush(rgb);
    SelectObject(hdc, hBrush);
    Rectangle(hdc, xLeft, yTop, xLeft + 75, yTop + 45);
    DeleteObject(hBrush);
}

void DrawBoxOutline (HWND hwnd, POINT ptBeg, POINT ptEnd)
{
    HDC hdc ;
    hdc = GetDC (hwnd) ;
    SetROP2 (hdc, R2_NOT) ;
    SelectObject (hdc, GetStockObject (NULL_BRUSH)) ;
    Rectangle (hdc, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y) ;
    ReleaseDC (hwnd, hdc) ;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndButton1, hwndButton2, hwndButton3, hwndButton4, hwndButton5, hwndButton6, hwndButton7, hwndButton8, hwndButton9;
    static HWND hwndCheckBox1, hwndCheckBox2, hwndCheckBox3, hwndCheckBox4, hwndCheckBox5, hwndCheckBox6, hwndCheckBox7, hwndCheckBox8, hwndCheckBox9;
    static HBITMAP hBitmap ;
    static int cxClient, cyClient, cxSource, cySource ;
    BITMAP bitmap ;
    HDC hdc, hdcMem ;
    HINSTANCE hInstance ;
    int x, y ;
    PAINTSTRUCT ps;
    int i ;
    POINT apt [NUM] ;
    LPDRAWITEMSTRUCT pdis ;
    POINT pt[3] ;
    RECT rc,rect, rectTemp;
    RECT gradient = {25, 342, 133, 353};
    HPEN		hPen;
    HBRUSH hBrush;
    HBRUSH hbr;

    COLORREF colRef = RGB(100,100,100);


    static BOOL fBlocking, fValidBox ;
    static POINT ptBeg, ptEnd, ptBoxBeg, ptBoxEnd ;

    HDC hDC = GetDC(hwnd);;
	static BOOL IsDrawing = FALSE;
	static int StartX, StartY;
	static int EndX, EndY;
	UINT iButtonState;

	static BOOL isLine, isFreeHand, isEllipse, isTriangle, isRectangle, isEraser;



//HBITMAP testImage = NULL;
    switch (message)                                                            /* handle the messages */
    {

        case WM_CREATE:
            /*testImage = (HBITMAP)LoadImage(NULL, TEXT("emblema2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            if(testImage == NULL) {
                MessageBox(NULL, TEXT("NO IMAGE LOADED!"), TEXT("Error!"),  MB_ICONEXCLAMATION | MB_OK);
            }*/
            /*hInstance = ((LPCREATESTRUCT) lParam)->hInstance ;*/
            hBitmap = LoadBitmap (hInst, TEXT ("header")) ;
            GetObject (hBitmap, sizeof (BITMAP), &bitmap) ;
            cxSource = bitmap.bmWidth ;
            cySource = bitmap.bmHeight ;

            hwndCheckBox1 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                          WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                                          20, 60, 13, 13,
                                          hwnd, (HMENU) IDR_RADIOBUTTON1, hInst, NULL) ;
            hwndButton1 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                        50, 60, 30, 30,
                                        hwnd, (HMENU) IDB_BUTTON1, hInst, NULL);
            hwndCheckBox2 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                          WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                                          20, 95, 13, 13,
                                          hwnd, (HMENU) IDR_RADIOBUTTON2, hInst, NULL) ;
            hwndButton2 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                        50, 95, 30, 30,
                                        hwnd, (HMENU) IDB_BUTTON2, hInst, NULL) ;
            hwndCheckBox3 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                          WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                                          20, 130, 13, 13,
                                          hwnd, (HMENU) IDR_RADIOBUTTON3, hInst, NULL) ;
            hwndButton3 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                        50, 130, 30, 30,
                                        hwnd, (HMENU) IDB_BUTTON3, hInst, NULL) ;
            hwndCheckBox4 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                          WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                                          20, 165, 13, 13,
                                          hwnd, (HMENU) IDR_RADIOBUTTON4, hInst, NULL) ;
            hwndButton4 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                        50, 165, 30, 30,
                                        hwnd, (HMENU) IDB_BUTTON4, hInst, NULL) ;
            hwndCheckBox5 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                          WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                                          20, 200, 13, 13,
                                          hwnd, (HMENU) IDR_RADIOBUTTON5, hInst, NULL) ;
            hwndButton5 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                        50, 200, 30, 30,
                                        hwnd, (HMENU) IDB_BUTTON5, hInst, NULL) ;
            hwndCheckBox6 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                          WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                                          20, 235, 13, 13,
                                          hwnd, (HMENU) IDR_RADIOBUTTON6, hInst, NULL) ;
            hwndButton6 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                        50, 235, 30, 30,
                                        hwnd, (HMENU) IDB_BUTTON6, hInst, NULL) ;
            hwndCheckBox7 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                          WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                                          20, 270, 13, 13,
                                          hwnd, (HMENU) IDR_RADIOBUTTON7, hInst, NULL) ;
            hwndButton7 = CreateWindow ( TEXT("BUTTON"), TEXT (""),
                                        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                        50, 270, 30, 30,
                                        hwnd, (HMENU) IDB_BUTTON7, hInst, NULL) ;
            return 0 ;
        case WM_DRAWITEM :
            pdis = (LPDRAWITEMSTRUCT) lParam ;
            FillRect (pdis->hDC, &pdis->rcItem, (HBRUSH) GetStockObject (WHITE_BRUSH)) ;
            FrameRect (pdis->hDC, &pdis->rcItem, (HBRUSH) GetStockObject (BLACK_BRUSH)) ;
            switch (pdis->CtlID)
            {
                case IDB_BUTTON1 :
                    /*pt[0].x = 5 ; pt[0].y = 5 ;
                    pt[1].x = 25 ; pt[1].y = 25 ;
                    pt[2].x = 25 ; pt[2].y = 5 ;
                    Triangle (pdis->hDC, pt) ;*/
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
                    MoveToEx (pdis->hDC, 11, 25, NULL) ;
                    LineTo (pdis->hDC, 25, 11) ;
                    break;
                case IDB_BUTTON2 :
                    pt[0].x = 3 ;  pt[0].y = 3 ;
                    pt[1].x = 3 ; pt[1].y = 27 ;
                    pt[2].x = 27 ; pt[2].y = 15 ;
                    pt[3].x = 27 ; pt[3].y = 3 ;
                    //PolyBezier (pdis->hDC, apt, 2) ;
                    DrawBezier (pdis->hDC, pt) ;
                    break;
                case IDB_BUTTON3 :{
                    //if(message==WM_COMMAND && LOWORD(wParam)==checkbox_id)
                   // {
		//First get handle of checkbox by casting lParam to HWND
                        //HWND checkbox_handle = (HWND)lParam;
		//send message BM_GETCHECK to check box to retrieve its current state
                    int state = SendMessage(hwndCheckBox3, BM_GETCHECK, 0, 0);


                    if(state == BST_CHECKED){ //yes it is checked
                        //MessageBox(hwnd,"It hs checked","Message",0);
                        Circle (pdis->hDC, 3, 3, 24, true) ;
                    }
                   else{
                     Circle (pdis->hDC, 3, 3, 24, false) ;
                   }
                  //  Circle (pdis->hDC, 3, 3, 24) ;
                    }break;
                case IDB_BUTTON4 :
                    hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
                    SelectObject(pdis->hDC, hPen);
                    MoveToEx (pdis->hDC, 8, 21, NULL) ;
                    LineTo (pdis->hDC, 22, 7) ;

                    hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                    SelectObject(pdis->hDC, hPen);
                    MoveToEx (pdis->hDC, 8, 21, NULL) ;
                    LineTo (pdis->hDC, 3, 24) ;
                    break;
                case IDB_BUTTON5 :
                    pt[0].x = 4 ;  pt[0].y = 26 ;
                    pt[1].x = 26 ; pt[1].y = 26 ;
                    pt[2].x = 15 ; pt[2].y = 3 ;
                    Triangle (pdis->hDC, pt);
                    break;
                case IDB_BUTTON6 :
                    pt[0].x = 3 ;  pt[0].y = 3 ;
                    pt[1].x = 3 ; pt[1].y = 26 ;
                    pt[2].x = 26 ; pt[2].y = 26 ;
                    pt[3].x = 26 ;  pt[3].y = 3;
                    Rectangle(pdis->hDC, pt);
                    break;
                case IDB_BUTTON7 :
                    pt[0].x = 3 ;  pt[0].y = 20 ;
                    pt[1].x = 6 ; pt[1].y = 15 ;
                    pt[2].x = 16 ; pt[2].y = 15 ;
                    pt[3].x = 13 ;  pt[3].y = 20;
                    Rectangle(pdis->hDC, pt);
                    hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                    SelectObject(pdis->hDC, hPen);
                    MoveToEx (pdis->hDC, 6, 15, NULL) ;
                    LineTo (pdis->hDC, 15, 11) ;
                    MoveToEx (pdis->hDC, 16, 15, NULL) ;
                    LineTo (pdis->hDC, 25, 11) ;
                    MoveToEx (pdis->hDC, 13, 20, NULL) ;
                    LineTo (pdis->hDC, 22, 16) ;
                    MoveToEx (pdis->hDC, 15, 11, NULL) ;
                    LineTo (pdis->hDC, 25, 11) ;
                    MoveToEx (pdis->hDC, 25, 11, NULL) ;
                    LineTo (pdis->hDC, 21, 17) ;
                    break;
            }

            if (pdis->itemState & ODS_SELECTED)
                InvertRect (pdis->hDC, &pdis->rcItem) ;
// Draw a focus rectangle if the button has the focus
            if (pdis->itemState & ODS_FOCUS)
            {
                /*pdis->rcItem.left += cx / 16 ;
                pdis->rcItem.top += cy / 16 ;
                pdis->rcItem.right -= cx / 16 ;
                pdis->rcItem.bottom -= cy / 16 ;*/
                DrawFocusRect (pdis->hDC, &pdis->rcItem) ;
            }
            return 0;
        /*case IDB_BUTTON1:

            switch (LOWORD(wParam))
            {
                case WM_LBUTTONDOWN:

                return 0 ;
                case WM_MOUSEMOVE:

                    return 0 ;
                case WM_LBUTTONUP:
                    InvalidateRect (hwnd, NULL, FALSE) ;
                    return 0 ;
            }
            break;*/
        /*case WM_LBUTTONDOWN :
            ptBeg.x = ptEnd.x = LOWORD (lParam) ;
            ptBeg.y = ptEnd.y = HIWORD (lParam) ;
            DrawBoxOutline (hwnd, ptBeg, ptEnd) ;
            SetCursor (LoadCursor (NULL, IDC_CROSS)) ;
            fBlocking = TRUE ;
            return 0 ;
        case WM_MOUSEMOVE :
            if (fBlocking)
            {
                SetCursor (LoadCursor (NULL, IDC_CROSS)) ;
                DrawBoxOutline (hwnd, ptBeg, ptEnd) ;
                ptEnd.x = LOWORD (lParam) ;
                ptEnd.y = HIWORD (lParam) ;
                DrawBoxOutline (hwnd, ptBeg, ptEnd) ;
            }
                return 0 ;
        case WM_LBUTTONUP :
            if (fBlocking)
            {
                DrawBoxOutline (hwnd, ptBeg, ptEnd) ;
                ptBoxBeg = ptBeg ;
                ptBoxEnd.x = LOWORD (lParam) ;
                ptBoxEnd.y = HIWORD (lParam) ;
                SetCursor (LoadCursor (NULL, IDC_ARROW)) ;
                fBlocking = FALSE ;
                fValidBox = TRUE ;
                InvalidateRect (hwnd, NULL, TRUE) ;
            }
            return 0 ;*/
            case WM_LBUTTONDOWN:

		// Find out if the Line button is clicked
		//iButtonState = SendMessage(Exo.hWndToolbar, TB_GETSTATE, IDM_DRAW_LINE, 0);

		// If the Line button is down, draw with it

		//iButtonState
		int coll;
		if((LOWORD(lParam) > 15) && (HIWORD(lParam) > 400) &&
            (LOWORD(lParam) < 90) && (HIWORD(lParam) <550) )
        {
            coll = 255* (HIWORD(lParam) - 400)/150;
           // printf("%i", HIWORD(lParam)-400);
            /*colRef = RGB(coll,coll,coll);
            HBRUSH hBrush3 = CreateSolidBrush(colRef);
            SelectObject(hdc, hBrush3);
            Rectangle(hdc, 15, 390, 90, 350);
            DeleteObject(hBrush3);*/
            colRef = RGB(coll, coll, coll);
             updateColorPreview(hDC, colRef, 15, 350);
            //SendMessage(hwnd, WM_PAINT, 0, 0);
        }


/*
rectTemp.top = 400;
            rectTemp.bottom = 550;
            rectTemp.left =15;
            rectTemp.right =90;
*/

		//hBrush
		hDC = GetDC(hwnd);
		/*hPen = CreatePen(PS_SOLID, 1, colRef);
		hbr = CreateSolidBrush(colRef)
        SelectObject(hDC, hPen);
        SelectObject(hDC, hbr);*/
			StartX = LOWORD(lParam);
			StartY = HIWORD(lParam);

			EndX = LOWORD(lParam);
			EndY = HIWORD(lParam);
		 if( isLine )
		{

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
			//ReleaseDC(hwnd, hDC);
		}

		if( isFreeHand )
		{
		    hPen = CreatePen(PS_SOLID, 1, colRef);
		hbr = CreateSolidBrush(colRef);
        SelectObject(hDC, hPen);
        SelectObject(hDC, hbr);
		    StartX = LOWORD(lParam);
			StartY = HIWORD(lParam);

			//SetROP2(hDC, R2_XORPEN);

			//MoveToEx(hDC, StartX, StartY, NULL);
			//LineTo(hDC, EndX, EndY);
			IsDrawing = TRUE;
			//ReleaseDC(hwnd, hDC);
						DeleteObject(hPen);
			DeleteObject(hbr);
		}

		if( isEllipse )
        {
            hPen = CreatePen(PS_SOLID, 1, colRef);
		hbr = CreateSolidBrush(colRef);
        SelectObject(hDC, hPen);
        SelectObject(hDC, hbr);
            SelectObject(hDC, CreateSolidBrush(colRef));
            SetROP2(hDC, R2_XORPEN);

            Ellipse(hDC, StartX, StartY, EndX, EndY);
            ReleaseDC(hwnd, hDC);
            IsDrawing = TRUE;
            			DeleteObject(hPen);
			DeleteObject(hbr);
        }

        if ( isTriangle ){

        hPen = CreatePen(PS_SOLID, 1, colRef);
		hbr = CreateSolidBrush(colRef);
        SelectObject(hDC, hPen);
        SelectObject(hDC, hbr);
            SetROP2(hDC, R2_XORPEN);

            pt[0].x = StartX ;  pt[0].y = EndY ;
            pt[1].x = (EndX + StartX) / 2; pt[1].y = StartY ;
            pt[2].x = EndX ; pt[2].y = EndY ;
            Triangle(hDC, pt);
            ReleaseDC(hwnd, hDC);
            IsDrawing = TRUE;
            			DeleteObject(hPen);
			DeleteObject(hbr);
        }

        if (isRectangle)
        {
            hPen = CreatePen(PS_SOLID, 1, colRef);
		hbr = CreateSolidBrush(colRef);
        SelectObject(hDC, hPen);
        SelectObject(hDC, hbr);
            SetROP2(hDC, R2_XORPEN);

            pt[0].x = StartX ;  pt[0].y = StartY;
            pt[1].x = StartX; pt[1].y = EndY ;
            pt[2].x = EndX ; pt[2].y = EndY ;
            pt[3].x = EndX ; pt[3].y = StartY ;
            Rectangle(hDC, pt);
            ReleaseDC(hwnd, hDC);
            IsDrawing = TRUE;
            			DeleteObject(hPen);
			DeleteObject(hbr);
        }
        if (isEraser)
        {
                    //point = adjustDrawLimits(xMouse, yMouse, drawingArea, stroke_weight);

                    rect.left = StartX - 5;
                    rect.right = StartX + 5;
                    rect.top = StartY - 5;
                    rect.bottom = StartY + 5 ;

                    InvalidateRect(hwnd, &rect, FALSE);
                    SendMessage(hwnd, WM_PAINT, 0, 0);
                    ValidateRect(hwnd, &rect);
        }
            ReleaseDC(hwnd, hDC);
		return 0;

	case WM_MOUSEMOVE:
	    //strokePen = CreatePen(PS_SOLID, stroke_weight, strokeRGB);
          //      fillBrush = (Button_GetCheck(hwndFillCheck) == BST_CHECKED)? CreateSolidBrush(fillRGB) : (HBRUSH)GetStockObject(NULL_BRUSH);
            //    SelectObject(hdc, strokePen);
              //  SelectObject(hdc, fillBrush);


        if( IsDrawing == TRUE )
		{

		hDC = GetDC(hwnd);
		if( isLine == TRUE )
		{
		    hPen = CreatePen(PS_SOLID, 1, colRef);
		hbr = CreateSolidBrush(colRef);
        SelectObject(hDC, hPen);
        SelectObject(hDC, hbr);
		    SetROP2(hDC, R2_NOTXORPEN);//sa faci veer trebuie sa scoti fishka

			MoveToEx(hDC, StartX, StartY, NULL);
			LineTo(hDC, EndX, EndY);

			EndX = LOWORD(lParam);
			EndY = HIWORD(lParam);

			MoveToEx(hDC, StartX, StartY, NULL);
			LineTo(hDC, EndX, EndY);
						DeleteObject(hPen);
			DeleteObject(hbr);
		}
		if( isFreeHand == TRUE )
		{
            hPen = CreatePen(PS_SOLID, 1, colRef);
		hbr = CreateSolidBrush(colRef);
        SelectObject(hDC, hPen);
        SelectObject(hDC, hbr);
			MoveToEx(hDC, EndX, EndY, NULL);
				EndX = LOWORD(lParam);
				EndY = HIWORD(lParam);
				LineTo(hDC, EndX, EndY);
							DeleteObject(hPen);
			DeleteObject(hbr);
		}
		if( isEllipse == TRUE )
		{
		    hPen = CreatePen(PS_SOLID, 1, colRef);
		hbr = CreateSolidBrush(colRef);
        SelectObject(hDC, hPen);
        SelectObject(hDC, hbr);
            SetROP2(hDC, R2_NOTXORPEN);//sa faci veer trebuie sa scoti fishka

			Ellipse(hDC, StartX, StartY, EndX, EndY);

			EndX = LOWORD(lParam);
			EndY = HIWORD(lParam);

			Ellipse(hDC, StartX, StartY, EndX, EndY);
						DeleteObject(hPen);
			DeleteObject(hbr);

		}
		if( isTriangle == TRUE )
		{
		    hPen = CreatePen(PS_SOLID, 1, colRef);
		hbr = CreateSolidBrush(colRef);
        SelectObject(hDC, hPen);
        SelectObject(hDC, hbr);
		    SetROP2(hDC, R2_NOTXORPEN);
		    pt[0].x = StartX ;  pt[0].y = EndY ;
            pt[1].x = (EndX + StartX) / 2; pt[1].y = StartY ;
            pt[2].x = EndX ; pt[2].y = EndY ;

            //sa faci veer trebuie sa scoti fishka

			//Ellipse(hDC, StartX, StartY, EndX, EndY);
            Triangle(hDC, pt);

			EndX = LOWORD(lParam);
			EndY = HIWORD(lParam);
            pt[0].x = StartX ;  pt[0].y = EndY ;
           pt[1].x = (EndX + StartX) / 2; pt[1].y = StartY ;
            pt[2].x = EndX ; pt[2].y = EndY ;
			//Ellipse(hDC, StartX, StartY, EndX, EndY);
			Triangle(hDC, pt);
						DeleteObject(hPen);
			DeleteObject(hbr);

		}
		if (isRectangle == TRUE)
        {
            hPen = CreatePen(PS_SOLID, 1, colRef);
		hbr = CreateSolidBrush(colRef);
        SelectObject(hDC, hPen);
        SelectObject(hDC, hbr);
            SetROP2(hDC, R2_NOTXORPEN);
            pt[0].x = StartX ;  pt[0].y = StartY;
            pt[1].x = StartX; pt[1].y = EndY ;
            pt[2].x = EndX ; pt[2].y = EndY ;
            pt[3].x = EndX ; pt[3].y = StartY ;
            Rectangle(hDC, pt);
            EndX = LOWORD(lParam);
			EndY = HIWORD(lParam);
			pt[0].x = StartX ;  pt[0].y = StartY;
            pt[1].x = StartX; pt[1].y = EndY ;
            pt[2].x = EndX ; pt[2].y = EndY ;
            pt[3].x = EndX ; pt[3].y = StartY ;
			Rectangle(hDC, pt);
			DeleteObject(hPen);
			DeleteObject(hbr);
        }

        if (isEraser)
        {
            //stroke_weight = getWeight(hwndEraserWeight);
                    //point = adjustDrawLimits(xMouse, yMouse, drawingArea, stroke_weight);
                    SetROP2(hDC, R2_NOTXORPEN);
                   // StartX = LOWORD(lParam);
                   // StartY = HIWORD(lParam);
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
            cxClient = LOWORD (lParam) ;
            cyClient = HIWORD (lParam) ;
            return 0;
        case WM_COMMAND :
            switch (LOWORD(wParam))
            {
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
            break;
            GetWindowRect (hwnd, &rc) ;
            return 0;

        case WM_PAINT:
        {
            /*draw the header*/
            hdc = BeginPaint (hwnd, &ps) ;
            hdcMem = CreateCompatibleDC (hdc) ;
            SelectObject (hdcMem, hBitmap) ;
            for (y = 0 ; y < cySource ; y += cySource)
                for (x = 0 ; x < cxSource ; x += cxSource)
                {
                    BitBlt (hdc, x, y, cxSource, cySource, hdcMem, 0, 0, SRCCOPY) ;
                }
            DeleteDC (hdcMem) ;

            /*HBRUSH hBrush2 = CreateSolidBrush(colRef);
            SelectObject(hdc, hBrush2);
            Rectangle(hdc, 15, 390, 90, 350);
            DeleteObject(hBrush2);
*/
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

/*
            void updateColorPreview(HDC hdc, COLORREF rgb, int xLeft, int yTop) {
                HBRUSH hBrush = CreateSolidBrush(rgb);
                SelectObject(hdc, hBrush);
                Rectangle(hdc, xLeft, yTop, xLeft + 45, yTop + 45);
    DeleteObject(hBrush);
}
            /*
            if (fValidBox)
            {
                SelectObject (hdc, GetStockObject (BLACK_BRUSH)) ;
                Rectangle (hdc, ptBoxBeg.x, ptBoxBeg.y, ptBoxEnd.x, ptBoxEnd.y) ;
            }

            if (fBlocking)
            {
                SetROP2 (hdc, R2_NOT) ;
                SelectObject (hdc, GetStockObject (NULL_BRUSH)) ;
                Rectangle (hdc, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y) ;
            }

            //hdc = BeginPaint (hwnd, &ps) ;
            MoveToEx (hdc, 0, cyClient / 2, NULL) ;
            LineTo (hdc, cxClient, cyClient / 2) ;
            for (int i = 0 ; i < NUM ; i++)
            {
                apt[i].x = i * cxClient / NUM ;
                apt[i].y = (int) (cyClient / 2 * (1 - sin (TWOPI * i / NUM))) ;
            }
            Polyline (hdc, apt, NUM) ;*/
            /*draw the left palete*/
            /*hInstance = (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE);
            hwndRect1 = CreateWindow (TEXT ("static"), NULL, WS_CHILD | WS_VISIBLE | SS_WHITERECT,
                                      5, 5, 200, 365, hwnd, (HMENU) IDC_RECTANGLE, hInstance, NULL);*/
            EndPaint (hwnd, &ps) ;
            return 0;
        }
            break;

        case WM_DESTROY:
            DeleteObject (hBitmap) ;
            PostQuitMessage (0);                                                /* send a WM_QUIT to the message queue */
            break;
        default:                                                                /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}


