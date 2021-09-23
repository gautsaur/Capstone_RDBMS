#include <windows.h>
#include <winuser.h>

#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
#define ID_TABLEVIEW  100 
#define ID_EDITOR 101 
#define ID_RESULTVIEW 102 
HMENU hmenu;

void AddMenu(HWND);
void CreateChildWindows(HWND hwnd, HINSTANCE hinst);
BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam);

/*
*	Author: N/A
*	Date:   Sept. 15, 2021
*	
*	Created when a GUI project is created in DEV-C++. 
*	Handles events when the windows behavior is changed.
*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	RECT rcClient; 
	
	//HINSTANCE hinst = GetModuleHandle(NULL);
	
	switch (Message)
	{

		case WM_COMMAND:
			switch (wParam)
			{
				case FILE_MENU_EXIT:
					DestroyWindow(hwnd);
					break;
	
				case FILE_MENU_NEW:
					MessageBeep(MB_ICONINFORMATION);
					break;
				
			}
			
		case WM_SIZE: {
            GetClientRect(hwnd, &rcClient); 
            EnumChildWindows(hwnd, EnumChildProc, (LPARAM) &rcClient);            
            break;
    	}
	
		case WM_CREATE:
			AddMenu(hwnd);
			//CreateChildWindows(hwnd, hinst);
			break;
	
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
}

/*
*	Author: Andrew Nunez
*	Date:   Sept. 23, 2021
*	
*	Creates a collection of child windows in the main window.
*/
void CreateChildWindows(HWND hwnd, HINSTANCE hinst)
{
	int i;
		
	for (i = 0; i < 3; i++){
		CreateWindowEx(
			0, 
			"ChildWClass", 
			(LPCTSTR) NULL, 
			WS_CHILD | WS_BORDER, 
			0,0,0,0, 
			hwnd, 
			(HMENU) (int) (ID_TABLEVIEW + i), 
			hinst, 
			NULL
		); 
		
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
				
	}
	
}

/*
*	Author: Andrew Nunez
*	Date:   Sept. 19, 2021
*	
*	Enumerates a list of child windows in the main window
*/
BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam) 
{ 
    LPRECT rcParent; 
    int i, idChild; 
 
    // Retrieve the child-window identifier. Use it to set the 
    // position of the child window. 
 
    idChild = GetWindowLong(hwndChild, GWL_ID); 
 
    if (idChild == ID_TABLEVIEW) 
        i = 0; 
    else if (idChild == ID_EDITOR) 
        i = 1; 
    else 
        i = 2; 
 
    // Size and position the child window.  
 
    rcParent = (LPRECT) lParam; 
    MoveWindow(hwndChild, 
               (rcParent->right / 3) * i, 
               0, 
               rcParent->right / 3, 
               rcParent->bottom, 
               TRUE); 
 
    // Make sure the child window is visible. 
 
 	MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
 	
    ShowWindow(hwndChild, SW_SHOW); 
 
    return TRUE;
}

/*
*	Author: N/A
*	Date:   Sept. 15, 2021
*	
*	Created when a GUI project is created in DEV-C++. 
*	Runs when the application starts. This initializes properties of the window.
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd;	   /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg;	   /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc; /* This is where we will send messages to */
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);	  /* Load a standard icon */
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "WindowClass", "ISU DBMS", WS_VISIBLE | WS_OVERLAPPEDWINDOW,
						  CW_USEDEFAULT, /* x */
						  CW_USEDEFAULT, /* y */
						  1280,			 /* width */
						  720,			 /* height */
						  NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	CreateChildWindows(hwnd, hInstance);

	/*
		This is the heart of our program where all input is processed and
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{							/* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg);	/* Send it to WndProc */
	}
	return msg.wParam;
}

void AddMenu(HWND hWnd)
{
	hmenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();

	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_NEW, "New");
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_OPEN, "Open");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, "NULL");
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, "Exit");

	AppendMenu(hmenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
	AppendMenu(hmenu, MF_STRING, NULL, "Help");

	SetMenu(hWnd, hmenu);
}
