/*----------------------------------------
COLORS1.C -- Colors Using Scroll Bars
(c) Charles Petzold, 1998
----------------------------------------*/
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <commctrl.h>

#define IDM_SYS_ABOUT 1
#define IDM_SYS_HELP 2
#define IDM_SYS_REMOVE 3
#define IDC_TASK_LIST 15
#define IDC_TEXT_INPUT 999
#define IDC_ADD_BUTTON 17
#define IDC_TASK_LIST 998


LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ScrollProc (HWND, UINT, WPARAM, LPARAM);

int idFocus ;
WNDPROC OldScroll ;
int focused = 0;

static TCHAR szAppName[] = TEXT ("Lab#2 Cigureanu Alexandru, FAF-111") ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {

    HWND hwnd ;
    MSG msg ;
    HMENU hMenu ;

    WNDCLASS wndclass ;
    wndclass.style = CS_HREDRAW | CS_VREDRAW ;
    wndclass.lpfnWndProc = WndProc ;
    wndclass.cbClsExtra = 0 ;
    wndclass.cbWndExtra = 0 ;
    wndclass.hInstance = hInstance ;
    wndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION) ;
    wndclass.hCursor = LoadCursor (NULL, IDC_ARROW) ;
    wndclass.hbrBackground = CreateSolidBrush (0) ;
    wndclass.lpszMenuName = NULL ;
    wndclass.lpszClassName = szAppName ;
    if (!RegisterClass (&wndclass)) {
        MessageBox (NULL, TEXT ("This program requires Windows NT!"), szAppName, MB_ICONERROR) ;
        return 0 ;
    }

    hwnd = CreateWindow (szAppName, TEXT ("Lab#2 Cigureanu Alexandru, FAF-111"), WS_OVERLAPPEDWINDOW,
                        0, 0,
                        600, 400,
                      NULL, NULL, hInstance, NULL);
    hMenu = GetSystemMenu (hwnd, FALSE) ;
        AppendMenu (hMenu, MF_SEPARATOR, 0, NULL) ;
        AppendMenu (hMenu, MF_STRING, IDM_SYS_ABOUT, TEXT ("About")) ;
        AppendMenu (hMenu, MF_STRING, IDM_SYS_HELP, TEXT ("Help")) ;
        AppendMenu (hMenu, MF_STRING, IDM_SYS_REMOVE, TEXT ("Remove Add System Menus"));

    ShowWindow (hwnd, iCmdShow);
    UpdateWindow (hwnd) ;
    while (GetMessage (&msg, NULL, 0, 0)) {
        TranslateMessage (&msg) ;
        DispatchMessage (&msg) ;
    }
    return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static COLORREF crPrim = RGB (255,0,0);
    static HBRUSH hBrush, hBrushStatic;
    static HWND hwndScroll, hwndLabel, hwndValue, hwndRect1, hwndRect2, hwndList, hwndTextInput, hwndAddButton;
    static int color, cyChar;
    static RECT rcColor ;
    static TCHAR * szColorLabel = TEXT ("White");
    HINSTANCE hInstance;
    int i, cxClient, cyClient;
    TCHAR szBuffer[10];
    LRESULT textSize;
    char * msg = new char[100];
    char * placeholder = new char[26];
    placeholder = "Type here the new task...";
    int index;
    HDC hdc;


    switch (message) {
        case WM_SYSCOMMAND:

            switch (LOWORD (wParam)) {
                case IDM_SYS_ABOUT:
                    MessageBox (hwnd, TEXT ("Alexandru Cigureanu\n")
                            TEXT ("st. gr. FAF-111"),
                            szAppName, MB_OK | MB_ICONINFORMATION) ;
                    return 0 ;
                case IDM_SYS_HELP:
                    MessageBox (hwnd, TEXT ("JOS ALIANTA!!!"),
                    szAppName, MB_OK | MB_ICONEXCLAMATION) ;
                    return 0 ;
                case IDM_SYS_REMOVE:
                    GetSystemMenu (hwnd, TRUE) ;
                    return 0 ;
            }
            break ;
        case WM_CREATE :
            hInstance = (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE) ;
            hwndRect1 = CreateWindow (TEXT ("static"), NULL,
                                     WS_CHILD | WS_VISIBLE | SS_WHITERECT,
                                     5, 5, 200, 365,
                                     hwnd, (HMENU) 9, hInstance, NULL) ;
            hwndRect2 = CreateWindow (TEXT ("static"), NULL,
                                     WS_CHILD | WS_VISIBLE | SS_WHITERECT,
                                     210, 5, 378, 365,
                                     hwnd, (HMENU) 9, hInstance, NULL) ;



            hwndScroll = CreateWindow (TEXT ("scrollbar"), NULL,
                                              WS_CHILD | WS_VISIBLE | WS_TABSTOP | SBS_VERT,
                                              0, 30, 20, 300,
                                              hwnd, (HMENU) 0, hInstance, NULL) ;
                SetScrollRange (hwndScroll, SB_CTL, 0, 255, FALSE) ;
                SetScrollPos (hwndScroll, SB_CTL, 0, FALSE) ;

            hwndLabel  = CreateWindow (TEXT ("static"), szColorLabel,
                                              WS_CHILD | WS_VISIBLE | SS_CENTER,
                                              0, 0, 50, 20,
                                              hwnd, (HMENU) 3,
                                              hInstance, NULL) ;
            hwndValue  = CreateWindow (TEXT ("static"), TEXT ("0"),
                                              WS_CHILD | WS_VISIBLE | SS_CENTER,
                                              0, 340, 30, 30,
                                              hwnd, (HMENU) 6,
                                              hInstance, NULL) ;
            hwndList = CreateWindow( TEXT("listbox"), "",
                                    WS_CHILD | WS_VISIBLE | LBS_STANDARD | ES_AUTOVSCROLL | WS_BORDER,
                                    215, 40, 320, 335,
                                    hwnd, (HMENU)IDC_TASK_LIST,
                                    hInstance, NULL);

            hwndTextInput = CreateWindow( TEXT("edit"), "",
                                        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                                        215, 10, 368, 25,
                                        hwnd, (HMENU)IDC_TEXT_INPUT,
                                        hInstance, NULL);
            hwndAddButton = CreateWindow(TEXT("button"), TEXT("Add"),
                                        WS_CHILD |WS_VISIBLE | BS_PUSHBUTTON,
                                        540, 40, 43, 325,
                                        hwnd, (HMENU)IDC_ADD_BUTTON,
                                        hInstance, NULL);

            OldScroll = (WNDPROC) SetWindowLong (hwndScroll, GWL_WNDPROC, (LONG) ScrollProc) ;
            hBrush = CreateSolidBrush (crPrim) ;

            hBrushStatic = CreateSolidBrush (GetSysColor (COLOR_BTNHIGHLIGHT)) ;
            cyChar = HIWORD (GetDialogBaseUnits ()) ;
            return 0 ;

        case WM_COMMAND:
            switch (LOWORD(wParam)){
                case IDC_ADD_BUTTON:
                    textSize = SendMessage(hwndTextInput, WM_GETTEXT, 100, (LPARAM)msg);
                    msg[textSize] = _T('\0');

                    if(strlen(msg) && strcmp(msg, placeholder))
                    {
                        char *item = new char[200];
                        strcpy(item, " - "); // Managing the new string
                        strcat(item, msg);
                        SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)item);
                        delete [] item; // Managing the memory

                        SendMessage(
                            hwndTextInput,
                            WM_SETTEXT,
                            TRUE,
                            (LPARAM)placeholder); // Recovering the placeholder
                        focused = 0;
                    }
                    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                    break;
                case IDC_TEXT_INPUT:
                    if(HIWORD(wParam) == EN_SETFOCUS)
                    {
                        textSize = SendMessage(hwndTextInput, WM_GETTEXT, 100, (LPARAM)msg);
                        msg[textSize] = _T('\0');
                        if(!strcmp(msg, placeholder))
                        {
                            SendMessage(hwndTextInput, WM_SETTEXT, TRUE,(LPARAM)"");// Clearing the text input
                            focused = 1;
                            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
                        }
                    }
                    else if(HIWORD(wParam) == EN_KILLFOCUS)
                    {
                        textSize = SendMessage(hwndTextInput, WM_GETTEXT, 100, (LPARAM)msg);
                        msg[textSize] = _T('\0');
                        if(!strcmp(msg, ""))
                        {
                            SendMessage(
                                hwndTextInput,
                                WM_SETTEXT,
                                TRUE,
                                (LPARAM)placeholder); // Recovering the placeholder
                            focused = 0;
                            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
                        }
                    }
                    break;
                case IDC_TASK_LIST:
                    switch(HIWORD(wParam))
                    {
                        case LBN_SELCHANGE:
                            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                            break;

                        case LBN_DBLCLK:
                            index = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
                            SendMessage(hwndList, LB_DELETESTRING, (WPARAM)index, 0);
                            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                            break;
                    }
                    break;
            }break;
        /*case WM_CTLCOLOREDIT:
            switch(GetDlgCtrlID((HWND)lParam))
            {
                case IDC_TEXT_INPUT:
                {
                    hdc = (HDC)wParam; //Get handles
                    if(focused)
                    {
                        color = CreateSolidBrush(RGB(255, 255, 255));
                        SetTextColor(hdc, RGB(0, 0, 0)); // Text color
                        SetBkMode(hdc, TRANSPARENT); // EditBox Backround Mode
                        SetBkColor(hdc,(LONG)color); // Backround color for EditBox
                    }
                    else
                    {
                        color = CreateSolidBrush(RGB(255, 255, 255));
                        SetTextColor(hdc, RGB(150, 150, 150)); // Text color
                        SetBkMode(hdc, TRANSPARENT); // EditBox Backround Mode
                        SetBkColor(hdc,(LONG)color); // Backround color for EditBox
                    }
                    return (LONG)color; // Paint it
                }
            }
            break;*/
        case WM_SIZE :
            cxClient = LOWORD (lParam) ;
            cyClient = HIWORD (lParam) ;
            SetRect (&rcColor, 0, 0, cxClient, cyClient) ;
            SetFocus(hwnd);
            return 0 ;
        case WM_SETFOCUS :
            //SetFocus (hwndScroll) ;
            return 0 ;
        case WM_VSCROLL :
            i = GetWindowLong ((HWND) lParam, GWL_ID) ;
            switch (LOWORD (wParam)) {
                case SB_PAGEDOWN :
                    color += 15 ;
// fall through
                case SB_LINEDOWN :
                    color = min (255, color + 1) ;
                    break ;
                case SB_PAGEUP :
                    color -= 15 ;
// fall through
                case SB_LINEUP :
                    color = max (0, color - 1) ;
                    break ;
                case SB_TOP :
                    color = 0 ;
                    break ;
                case SB_BOTTOM :
                    color = 255 ;
                    break ;
                case SB_THUMBPOSITION :
                case SB_THUMBTRACK :
                    color = HIWORD (wParam) ;
                    break ;
                default :
                    break ;
            }
            SetScrollPos (hwndScroll, SB_CTL, color, TRUE) ;
            wsprintf (szBuffer, TEXT ("%i"), color) ;
            SetWindowText (hwndValue, szBuffer) ;
            DeleteObject ((HBRUSH)
                SetClassLong (hwnd, GCL_HBRBACKGROUND, (LONG)
                    CreateSolidBrush (RGB (color, color, color)))) ;
            InvalidateRect (hwnd, &rcColor, TRUE) ;
            return 0 ;
        case WM_CTLCOLORSCROLLBAR :
            i = GetWindowLong ((HWND) lParam, GWL_ID) ;
            return (LRESULT) hBrush;
        case WM_CTLCOLORSTATIC :
            i = GetWindowLong ((HWND) lParam, GWL_ID) ;
            if (i >= 3 && i <= 8) // static text controls
            {
                SetTextColor ((HDC) wParam, crPrim) ;
                SetBkColor ((HDC) wParam, GetSysColor (COLOR_BTNHIGHLIGHT));
                return (LRESULT) hBrushStatic ;
            }
            break ;
        case WM_SYSCOLORCHANGE :
            DeleteObject (hBrushStatic) ;
            hBrushStatic = CreateSolidBrush (GetSysColor (COLOR_BTNHIGHLIGHT)) ;
            return 0 ;
        case WM_CLOSE:
            if(MessageBox(hwnd, "Do you want to exit?", "EXIT", MB_OKCANCEL) == IDOK)
                DestroyWindow(hwnd);
            break;
        case WM_DESTROY :
            DeleteObject ((HBRUSH)
                SetClassLong (hwnd, GCL_HBRBACKGROUND, (LONG)
                    GetStockObject (WHITE_BRUSH))) ;
            //for (i = 0 ; i < 3 ; i++)
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
        case WM_KEYDOWN :
            if (wParam == VK_TAB)
                SetFocus (GetDlgItem (GetParent (hwnd), (id + (GetKeyState (VK_SHIFT) < 0 ? 2 : 1)) % 3)) ;
            break ;
        case WM_SETFOCUS :
            idFocus = id ;
            break ;
    }
    return CallWindowProc (OldScroll, hwnd, message, wParam, lParam) ;
}

