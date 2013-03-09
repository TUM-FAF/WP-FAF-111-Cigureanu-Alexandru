#include "windows.h"
#include "TCHAR.H"
#include "cstdlib"

#define CLEARTYPE_QUALITY       1
enum { ID_LABEL = 1,ID_EDIT, ID_BUTTON0,ID_BUTTON1};
LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HINSTANCE g_hInst;
TCHAR	szAppName[]	 = TEXT("Alexandru Cigureanu");
TCHAR	szWinClass[] = TEXT("WIN_CLASS");

//static field
HWND static_label1;
//input labels
HWND input_text1;
HWND input_text2;
//number
HWND button0;
HWND button1;

void CreateText(HDC &hdc,
                HFONT &hFont,
                RECT &rect,
                LPCSTR lpcwString,
                int nHeight,
                int nWidth,
                int fnWeight,
                int x,
                int y,
                COLORREF color){
	hFont = CreateFont(
		nHeight,
		nWidth,
		0,
		0,
		fnWeight,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY,
		VARIABLE_PITCH,
		TEXT("Times New Roman")
	);
	SelectObject(hdc, hFont);
	SetRect(&rect, x,y,0,0);
	SetTextColor(hdc, color);
	DrawText(hdc, lpcwString, -1,&rect, DT_NOCLIP);
}

int WINAPI WinMain(HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		PSTR lpCmdLine,
		int nCmdShow){

	MSG			msg;
	WNDCLASS	wc;
	HWND		hwnd;
	g_hInst = hInstance;

	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance		= hInstance;
	wc.lpfnWndProc		= WinProc;
	wc.lpszClassName	= szWinClass;
	wc.lpszMenuName		= NULL;
	wc.style			= CS_HREDRAW | CS_VREDRAW;

 	if( !RegisterClass(&wc)) {
		MessageBox(NULL, TEXT("Error registering class"), TEXT("ERROR"), MB_OK);
		return 0;
	}

	hwnd = CreateWindow(szWinClass,
						szAppName,
						WS_OVERLAPPED|WS_BORDER|WS_SYSMENU,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						400,
						200,
						(HWND) NULL,
						(HMENU) NULL,
						(HINSTANCE) hInstance,
						(void *) NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, (HWND) NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	PAINTSTRUCT	ps;
	HDC			hdc;
	HFONT       hf;

	switch(msg) {
		case WM_CREATE: {
			static_label1 = CreateWindow("Static", "Click the button -->", WS_CHILD | WS_VISIBLE ,60,30,125,18,hwnd,0, g_hInst,0);
			input_text1   = CreateWindow("Edit",  NULL, WS_BORDER | NULL | WS_CHILD | WS_VISIBLE | NULL | NULL ,35,75,175,20,hwnd,(HMENU)ID_EDIT,g_hInst,0);
            input_text2   = CreateWindow("Edit",  NULL,WS_BORDER | NULL | WS_CHILD | WS_VISIBLE | NULL | NULL ,35,105,175,20,hwnd,(HMENU)ID_EDIT,g_hInst,0);
			button0       = CreateWindow("Button","0",BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE ,220,75,25,50,hwnd,(HMENU)ID_BUTTON0,g_hInst,0);
			button1       = CreateWindow("Button","Rand Advice",BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE ,200,30,100,18,hwnd,(HMENU)ID_BUTTON1,g_hInst,0);
		}break;
		case WM_PAINT: {

			HFONT hFont;
			RECT rect;
            LPCSTR a = ("Get a free advice");

            hdc = BeginPaint(hwnd, &ps);
                CreateText(hdc, hFont, rect, a, 20, 15, 100, 50, 0, RGB(123,123,123));
			EndPaint(hwnd, &ps);
        }break;
		case WM_COMMAND: //Command from Child windows and menus are under this message
            switch(wParam) {
                case ID_BUTTON0: {
						SetWindowText(input_text1,"SEE YA");
						SetWindowText(input_text2, "ON NEXT LAB");
						break;
					}
				case ID_BUTTON1:  {
                        int v1 = rand() % 7;
                        switch(v1) {
                            case 0:{SetWindowText(static_label1, "Be patient");}break;
                            case 1:{SetWindowText(static_label1, "Be perseverent");}break;
                            case 2:{SetWindowText(static_label1, "Listen to others");}break;
                            case 3:{SetWindowText(static_label1, "Do your best");}break;
                            case 4:{SetWindowText(static_label1, "Give free Hugs");}break;
                            case 5:{SetWindowText(static_label1, "Love your family");}break;
                            case 6:{SetWindowText(static_label1, "Never stop");}break;
                        }
						break;
					}
				}//switch.
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break; // pass to DefWindowProc(...) as well
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
	}return DefWindowProc(hwnd, msg, wParam, lParam);}
