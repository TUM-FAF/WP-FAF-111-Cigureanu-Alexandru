#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <commctrl.h>
#include "resource.h"

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
#define IDC_RECTANGLE 71


LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ScrollProc (HWND, UINT, WPARAM, LPARAM);

int idFocus ;
WNDPROC OldScroll1 ;
WNDPROC OldScroll2 ;
WNDPROC OldScroll3 ;
int focused = 0;
int windowWidth = 600;
int windowHeight = 400;
static TCHAR szAppName[] = TEXT ("Lab#2 Cigureanu Alexandru, FAF-111") ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {

    HWND hwnd;
    MSG msg;
    HMENU hMenu;

    WNDCLASS wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon  = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = CreateSolidBrush (0);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    if (!RegisterClass (&wndclass)) {
        MessageBox (NULL, TEXT ("This program requires Windows NT!"), szAppName, MB_ICONERROR);
        return 0 ;
    }

    hwnd = CreateWindow (szAppName, TEXT ("Lab#2 Cigureanu Alexandru, FAF-111"), WS_OVERLAPPEDWINDOW,
                         0, 0, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);
    hMenu = GetSystemMenu (hwnd, FALSE);
        AppendMenu (hMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu (hMenu, MF_STRING, IDM_SYS_ABOUT, TEXT ("About"));
        AppendMenu (hMenu, MF_STRING, IDM_SYS_HELP, TEXT ("Help"));
        AppendMenu (hMenu, MF_STRING, IDM_SYS_REMOVE, TEXT ("Remove Add System Menus"));

    ShowWindow (hwnd, iCmdShow);
    UpdateWindow (hwnd);
    while (GetMessage (&msg, NULL, 0, 0)) {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static COLORREF crPrim = RGB (0,0,0);
    static HBRUSH hBrush, hBrushStatic;
    static HWND hwndScroll1, hwndScroll2, hwndScroll3;
    static HWND hwndLabel1, hwndLabel2, hwndLabel3;
    static HWND hwndValue1, hwndValue2, hwndValue3;
    static HWND hwndRect1, hwndList, hwndTextInput, hwndAddButton;
    static int color;
    static RECT rcColor ;
    static TCHAR * szColorLabel1 = TEXT ("Border");
    static TCHAR * szColorLabel2 = TEXT ("Width");
    static TCHAR * szColorLabel3 = TEXT ("Height");
    HINSTANCE hInstance;
    int i, cxClient, cyClient;
    TCHAR szBuffer1[10];
    TCHAR szBuffer2[10];
    TCHAR szBuffer3[10];
    LRESULT textSize;
    char * msg = new char[100];
    char * placeholder = new char[12];
    placeholder = "Add element";
    int index;
    HDC hdc;
    HICON hIcon;
    HCURSOR hCursor;

    switch (message) {

        case WM_SYSCOMMAND :

            switch (LOWORD (wParam)) {

                case IDM_SYS_ABOUT :

                    MessageBox (hwnd, TEXT ("Alexandru Cigureanu\nst. gr. FAF-111"),
                                szAppName, MB_OK | MB_ICONINFORMATION);
                    return 0;
                case IDM_SYS_HELP :

                    MessageBox (hwnd, TEXT ("JOS ALIANTA!!!"),
                                szAppName, MB_OK | MB_ICONEXCLAMATION);
                    return 0;
                case IDM_SYS_REMOVE :

                    GetSystemMenu (hwnd, TRUE);
                    return 0 ;
            }
            break ;
        case WM_CREATE :

            hInstance = (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE);
            hwndRect1 = CreateWindow (TEXT ("static"), NULL, WS_CHILD | WS_VISIBLE | SS_WHITERECT,
                                      5, 5, 200, 365, hwnd, (HMENU) IDC_RECTANGLE, hInstance, NULL);
//////////////////////////scroll1
            hwndScroll1 = CreateWindow (TEXT ("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | SBS_VERT,
                                              10, 30, 20, 300, hwnd, (HMENU) IDC_SCROLL_BAR1, hInstance, NULL);
                                              SetScrollRange (hwndScroll1, SB_CTL, 0, 255, FALSE);
                                              SetScrollPos (hwndScroll1, SB_CTL, 0, FALSE);
            hwndLabel1  = CreateWindow (TEXT ("static"), szColorLabel1, WS_CHILD | WS_VISIBLE | SS_CENTER,
                                              5, 5, 45, 18, hwnd, (HMENU) IDC_LABEL1, hInstance, NULL);
            hwndValue1  = CreateWindow (TEXT ("static"), TEXT ("0"),WS_CHILD | WS_VISIBLE | SS_CENTER,
                                              5, 340, 30, 30, hwnd, (HMENU) IDC_VAR_LABEL1, hInstance, NULL);
//////////////////////////scroll2
            hwndScroll2 = CreateWindow (TEXT ("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | SBS_VERT,
                                              65, 30, 20, 300, hwnd, (HMENU) IDC_SCROLL_BAR2, hInstance, NULL);
                                              SetScrollRange (hwndScroll2, SB_CTL, 600, 700, FALSE);
                                              SetScrollPos (hwndScroll2, SB_CTL, 0, FALSE);
            hwndLabel2  = CreateWindow (TEXT ("static"), szColorLabel2, WS_CHILD | WS_VISIBLE | SS_CENTER,
                                              55, 5, 45, 18, hwnd, (HMENU) IDC_LABEL2, hInstance, NULL);
            hwndValue2  = CreateWindow (TEXT ("static"), TEXT ("600"), WS_CHILD | WS_VISIBLE | SS_CENTER,
                                              60, 340, 30, 30, hwnd, (HMENU) IDC_VAR_LABEL2, hInstance, NULL);
//////////////////////////scrol3
            hwndScroll3 = CreateWindow (TEXT ("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | SBS_VERT,
                                              115, 30, 20, 300, hwnd, (HMENU) IDC_SCROLL_BAR3, hInstance, NULL);
                                              SetScrollRange (hwndScroll3, SB_CTL, 400, 500, FALSE);
                                              SetScrollPos (hwndScroll3, SB_CTL, 0, FALSE);
            hwndLabel3  = CreateWindow (TEXT ("static"), szColorLabel3, WS_CHILD | WS_VISIBLE | SS_CENTER,
                                              105, 5, 45, 18, hwnd, (HMENU) IDC_LABEL3, hInstance, NULL);
            hwndValue3  = CreateWindow (TEXT ("static"), TEXT ("400"), WS_CHILD | WS_VISIBLE | SS_CENTER,
                                              110, 340, 30, 30, hwnd, (HMENU) IDC_VAR_LABEL3, hInstance, NULL);

            hwndList = CreateWindow(TEXT("listbox"), "", WS_CHILD | WS_VISIBLE | LBS_STANDARD | ES_AUTOVSCROLL | WS_BORDER,
                                    215, 33, 315, 340, hwnd, (HMENU)IDC_TASK_LIST, hInstance, NULL);
            hwndTextInput = CreateWindow(TEXT("edit"), placeholder, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                                         215, 5, 373, 20, hwnd, (HMENU)IDC_TEXT_INPUT, hInstance, NULL);
            hwndAddButton = CreateWindow(TEXT("button"), TEXT("Add"), WS_CHILD |WS_VISIBLE | BS_PUSHBUTTON,
                                        540, 34, 47, 336, hwnd, (HMENU)IDC_ADD_BUTTON, hInstance, NULL);

            OldScroll1 = (WNDPROC) SetWindowLong (hwndScroll1, GWL_WNDPROC, (LONG) ScrollProc);
            OldScroll2 = (WNDPROC) SetWindowLong (hwndScroll2, GWL_WNDPROC, (LONG) ScrollProc);
            OldScroll3 = (WNDPROC) SetWindowLong (hwndScroll3, GWL_WNDPROC, (LONG) ScrollProc);

            hBrush = CreateSolidBrush (crPrim);

            hBrushStatic = CreateSolidBrush (GetSysColor (COLOR_BTNHIGHLIGHT));
            return 0;
      case WM_SETCURSOR :

            hInstance = (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE);
            hCursor = LoadCursor (hInstance, MAKEINTRESOURCE (IDC_CURSOR));
            if (GetDlgCtrlID((HWND)wParam) == IDC_TASK_LIST) {
                SetCursor(hCursor);
                return TRUE;
            }
            break;
        case WM_COMMAND:

            switch (LOWORD (wParam)) {

                case IDC_ADD_BUTTON :

                    textSize = SendMessage(hwndTextInput, WM_GETTEXT, 100, (LPARAM)msg);
                    msg[textSize] = _T('\0');
                    if (strlen(msg) && strcmp(msg, placeholder)) {
                        char *item = new char[200];
                        strcpy(item, " ---- "); // Managing the new string
                        strcat(item, msg);
                        SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)item);
                        delete [] item; // Managing the memory

                        SendMessage (hwndTextInput, WM_SETTEXT,
                                    TRUE, (LPARAM)placeholder); // Recovering the placeholder
                        focused = 0;
                    }
                    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                    break;
                case IDC_TEXT_INPUT :

                    if (HIWORD(wParam) == EN_SETFOCUS) {
                        textSize = SendMessage(hwndTextInput, WM_GETTEXT, 100, (LPARAM) msg);
                        msg[textSize] = _T('\0');
                        if (!strcmp(msg, placeholder)) {
                            SendMessage(hwndTextInput, WM_SETTEXT, TRUE, (LPARAM) "");// Clearing the text input
                            focused = 1;
                            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
                        }
                    } else if (HIWORD(wParam) == EN_KILLFOCUS) {
                        textSize = SendMessage(hwndTextInput, WM_GETTEXT, 100, (LPARAM) msg);
                        msg[textSize] = _T('\0');
                        if (!strcmp(msg, "")) {
                            SendMessage (hwndTextInput, WM_SETTEXT, TRUE, (LPARAM) placeholder); // Recovering the placeholder
                            focused = 0;
                            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
                        }
                    }
                    break;
                case IDC_TASK_LIST :

                    switch (HIWORD(wParam)){

                        case LBN_SELCHANGE :

                            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                            break;
                        case LBN_DBLCLK :

                            index = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
                            SendMessage(hwndList, LB_DELETESTRING, (WPARAM) index, 0);
                            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                            break;
                    }
                    break;
            }
            break;
        case WM_KEYDOWN :

            switch (wParam) {

                case VK_SPACE :

                    if (HIBYTE(GetAsyncKeyState(VK_LCONTROL))) {
                        if (MessageBox(hwnd, "Do you want to exit?", "EXIT", MB_OKCANCEL) == IDOK) {
                            DestroyWindow(hwnd);
                        }
                        return 0;
                    }
                    break ;
            }
            break;
        case WM_GETMINMAXINFO: {

            DefWindowProc(hwnd, message, wParam, lParam);
            MINMAXINFO* mmi = (MINMAXINFO*) lParam;
            mmi->ptMaxTrackSize.x = windowWidth;
            mmi->ptMaxTrackSize.y = windowHeight;
            return 0;
            }
        case WM_SIZE :
            cxClient = LOWORD (lParam) ;
            cyClient = HIWORD (lParam) ;
            SetRect (&rcColor, 0, 0, cxClient, cyClient) ;
            SetFocus(hwnd);
            return 0 ;
        case WM_SETFOCUS :
            if (idFocus == IDC_SCROLL_BAR1) {
                SetFocus (hwndScroll1) ;
            } else if (idFocus == IDC_SCROLL_BAR2) {
                SetFocus (hwndScroll2);
            } else if (idFocus == IDC_SCROLL_BAR3) {
                SetFocus (hwndScroll3);
            }
            return 0 ;
        case WM_VSCROLL :

            i = GetWindowLong ((HWND) lParam, GWL_ID) ;

            switch (LOWORD (wParam)) {

                case SB_PAGEDOWN :

                    if (i == IDC_SCROLL_BAR1) {
                        color += 15;
                    } else if (i == IDC_SCROLL_BAR2) {
                        windowWidth += 15;
                    } else if (i == IDC_SCROLL_BAR3) {
                        windowHeight += 15;
                    }
// fall through
                case SB_LINEDOWN :

                    if (i == IDC_SCROLL_BAR1) {
                        color = min (255, color + 1) ;
                    } else if (i == IDC_SCROLL_BAR2) {
                        windowWidth = min (700, windowWidth + 1) ;
                    } else if (i == IDC_SCROLL_BAR3) {
                        windowHeight = min (700, windowHeight + 1) ;
                    }
                    break ;
                case SB_PAGEUP :

                    if (i == IDC_SCROLL_BAR1) {
                        color -= 15;
                    } else if (i == IDC_SCROLL_BAR2) {
                        windowWidth -= 15;
                    } else if (i == IDC_SCROLL_BAR3) {
                        windowHeight -= 15;
                    }
// fall through
                case SB_LINEUP :

                    if (i == IDC_SCROLL_BAR1) {
                        color = max (0, color - 1);
                    } else if (i == IDC_SCROLL_BAR2) {
                        windowWidth = min (600, windowWidth - 1);
                    } else if (i == IDC_SCROLL_BAR3) {
                        windowHeight = min (400, windowHeight - 1);
                    }
                    break ;
                case SB_TOP :

                    if (i == IDC_SCROLL_BAR1){
                        color = 0 ;
                    } else if (i == IDC_SCROLL_BAR2){
                        windowWidth = 600;
                    } else if (i == IDC_SCROLL_BAR3){
                        windowHeight = 600;
                    }
                    break ;
                case SB_BOTTOM :

                    if (i == IDC_SCROLL_BAR1) {
                        color = 255;
                    } else if (i == IDC_SCROLL_BAR2) {
                        windowWidth = 700;
                    } else if (i == IDC_SCROLL_BAR3) {
                        windowHeight = 500;
                    }
                    break ;
                case SB_THUMBPOSITION :

                case SB_THUMBTRACK :

                    if (i == IDC_SCROLL_BAR1) {
                        color = HIWORD (wParam);
                    } else if (i == IDC_SCROLL_BAR2) {
                        windowWidth = HIWORD (wParam);
                    } else if (i == IDC_SCROLL_BAR3){
                        windowHeight = HIWORD (wParam);
                    }
                    break ;
                default :

                    break ;
            }

            if (i == IDC_SCROLL_BAR1) {

                SetScrollPos (hwndScroll1, SB_CTL, color, TRUE) ;
                wsprintf (szBuffer1, TEXT ("%i"), color) ;
                SetWindowText (hwndValue1, szBuffer1) ;
                DeleteObject ((HBRUSH) SetClassLong (hwnd, GCL_HBRBACKGROUND,
                              (LONG) CreateSolidBrush (RGB (color, color, color)))) ;
            } else if (i == IDC_SCROLL_BAR2) {

                SetScrollPos (hwndScroll2, SB_CTL, windowWidth, TRUE) ;
                wsprintf (szBuffer2, TEXT ("%i"), windowWidth) ;
                SetWindowText (hwndValue2, szBuffer2) ;
            } else if (i == IDC_SCROLL_BAR3) {

                SetScrollPos (hwndScroll3, SB_CTL, windowHeight, TRUE) ;
                wsprintf (szBuffer3, TEXT ("%i"), windowHeight) ;
                SetWindowText (hwndValue3, szBuffer3) ;
            }

            InvalidateRect (hwnd, &rcColor, TRUE) ;
            return 0 ;
        case WM_CTLCOLORSCROLLBAR :

            i = GetWindowLong ((HWND) lParam, GWL_ID);
            return (LRESULT) hBrush;
        case WM_CTLCOLORSTATIC :

            i = GetWindowLong ((HWND) lParam, GWL_ID) ;
            if (i == IDC_LABEL1 || i == IDC_VAR_LABEL1 ||
                i == IDC_LABEL2 || i == IDC_VAR_LABEL2 ||
                i == IDC_LABEL3 || i == IDC_VAR_LABEL3) {  // static text controls
                    SetTextColor ((HDC) wParam, crPrim);
                    SetBkColor ((HDC) wParam, GetSysColor (COLOR_BTNHIGHLIGHT));
                return (LRESULT) hBrushStatic ;
            }
            break ;
        case WM_SYSCOLORCHANGE :

            DeleteObject (hBrushStatic);
            hBrushStatic = CreateSolidBrush (GetSysColor (COLOR_BTNHIGHLIGHT));
            return 0 ;
        case WM_CLOSE:

            if (MessageBox(hwnd, "Do you want to exit?", "EXIT", MB_OKCANCEL) == IDOK) {
                DestroyWindow(hwnd);
            }
            return 0;
            break;
        case WM_DESTROY :
            DeleteObject ((HBRUSH) SetClassLong (hwnd, GCL_HBRBACKGROUND,
                         (LONG) GetStockObject (WHITE_BRUSH))) ;
            DeleteObject (hBrush) ;
            DeleteObject (hBrushStatic) ;
            PostQuitMessage (0) ;
            return 0 ;
    }
    return DefWindowProc (hwnd, message, wParam, lParam) ;
}

LRESULT CALLBACK ScrollProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int id = GetWindowLong (hwnd, GWL_ID) ;

    switch (message) {

        case WM_SETFOCUS :

            idFocus = id;
            break ;
    }

    if (id == IDC_SCROLL_BAR1) {
        return CallWindowProc (OldScroll1, hwnd, message, wParam, lParam);
    } else if (id == IDC_SCROLL_BAR2) {
        return CallWindowProc (OldScroll2, hwnd, message, wParam, lParam);
    } else if(id == IDC_SCROLL_BAR3) {
        return CallWindowProc (OldScroll3, hwnd, message, wParam, lParam) ;
    }
}

