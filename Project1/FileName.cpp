#include <windows.h>


// Global variables
NOTIFYICONDATA nid;

// Window procedure function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        // Create a button
        CreateWindowEx(
            0,                     // Optional window styles
            L"BUTTON",             // Window class name
            L"Click Me",           // Button text
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Button style
            10, 10,                // x, y position
            100, 30,               // Width, height
            hwnd,                  // Parent window
            (HMENU)1,              // Button ID
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), // Instance handle
            NULL                   // Additional application data
        );

        // Add tray icon
        memset(&nid, 0, sizeof(nid));
        nid.cbSize = sizeof(nid);
        nid.hWnd = hwnd;
        nid.uID = 1;
        nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        nid.uCallbackMessage = WM_USER + 1; // Custom message for tray icon
        nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wcscpy_s(nid.szTip, 128, L"Simple Win32 App");
        Shell_NotifyIcon(NIM_ADD, &nid);

        return 0;
    }
    case WM_USER + 1: { // Custom message for tray icon
        switch (lParam) {
        case WM_RBUTTONDOWN: {
            HMENU hMenu = CreatePopupMenu();
            AppendMenu(hMenu, MF_STRING, 100, L"Exit");
            AppendMenu(hMenu, MF_STRING, 200, L"Show Window"); // Add "Show Window"
            POINT pt;
            GetCursorPos(&pt);
            SetForegroundWindow(hwnd);
            TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
            DestroyMenu(hMenu);
            break;
        }
        }
        return 0;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) { // Button ID
            MessageBox(hwnd, L"Button clicked!", L"Information", MB_OK);
        }
        else if (LOWORD(wParam) == 100) { // Exit menu item
            Shell_NotifyIcon(NIM_DELETE, &nid); // Remove tray icon
            PostQuitMessage(0);
        }
        else if (LOWORD(wParam) == 200) { // Show Window menu item
            ShowWindow(hwnd, SW_SHOW);
        }
        return 0;
    case WM_DESTROY:
        Shell_NotifyIcon(NIM_DELETE, &nid); // Remove tray icon
        PostQuitMessage(0);
        return 0;

    case WM_SYSCOMMAND:
        if (wParam == SC_CLOSE) {
            ShowWindow(hwnd, SW_HIDE); // Minimize to tray instead of closing
            return 0;
        }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
    int nCmdShow) {
    // Register the window class
    const wchar_t CLASS_NAME[] = L"MyWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0, // Optional window styles
        CLASS_NAME, // Window class name
        L"Simple Win32 App", // Window text
        WS_OVERLAPPEDWINDOW, // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL, // Parent window
        NULL, // Menu
        hInstance, // Instance handle
        NULL // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}