#include "MainWindow.h"
#include "TaskScheduler.h"
#include "TaskFactory.h"
#include "LogWriter.h"
#include "TaskA.h"
#include "TaskB.h"
#include "TaskC.h"
#include "TaskD.h"
#include "TaskE.h"
#include <commctrl.h> // For improved controls (optional, but good for styling)

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")

// Global handle for the listbox to update from thread
HWND g_hListBox = NULL;

void MainWindow::Run(HINSTANCE hInstance) {
    // Register the window class
    const wchar_t CLASS_NAME[] = L"Task Scheduler UI Class";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        L"Task Scheduler Control Panel", // Window text
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME, // Window style (fixed size)
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 500, // Size and position
        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL) return;

    ShowWindow(hwnd, SW_SHOW);

    // Message loop
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK MainWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        CreateControls(hwnd);
        // Setup log callback
        g_logWriter.SetCallback([hwnd](const std::string& log) {
            // Post message to UI thread to update ListBox
            std::string* pLog = new std::string(log);
            PostMessage(hwnd, WM_USER + 1, (WPARAM)pLog, 0);
        });
        return 0;

    case WM_USER + 1: // Custom message for logging
    {
        std::string* pLog = (std::string*)wParam;
        if (pLog && g_hListBox) {
            // Convert ANSI/UTF-8 string to Wide String for ListBox
            int size_needed = MultiByteToWideChar(CP_ACP, 0, pLog->c_str(), -1, NULL, 0);
            if (size_needed > 0) {
                std::vector<wchar_t> wLog(size_needed);
                MultiByteToWideChar(CP_ACP, 0, pLog->c_str(), -1, &wLog[0], size_needed);
                SendMessage(g_hListBox, LB_ADDSTRING, 0, (LPARAM)wLog.data());
                SendMessage(g_hListBox, LB_SETTOPINDEX, SendMessage(g_hListBox, LB_GETCOUNT, 0, 0) - 1, 0); // Auto scroll
            }
            delete pLog;
        }
        return 0;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_BTN_TASK_A:
            TaskScheduler::GetInstance().AddDelayedTask(TaskFactory::CreateTaskA(), std::chrono::seconds(10));
            g_logWriter.WriteLog("System: Task A scheduled (10s delay)");
            break;
        case IDC_BTN_TASK_B:
            TaskScheduler::GetInstance().AddPeriodicTask(TaskFactory::CreateTaskB(), std::chrono::seconds(5));
            g_logWriter.WriteLog("System: Task B scheduled (Periodic 5s)");
            break;
        case IDC_BTN_TASK_C:
            TaskScheduler::GetInstance().AddOneTimeTask(TaskFactory::CreateTaskC());
            g_logWriter.WriteLog("System: Task C scheduled (Immediate)");
            break;
        case IDC_BTN_TASK_D:
            TaskScheduler::GetInstance().AddPeriodicTask(TaskFactory::CreateTaskD(), std::chrono::seconds(60));
            g_logWriter.WriteLog("System: Task D scheduled (Periodic 1min)");
            break;
        case IDC_BTN_TASK_E:
            TaskScheduler::GetInstance().AddDelayedTask(TaskFactory::CreateTaskE(), std::chrono::seconds(10));
            g_logWriter.WriteLog("System: Task E scheduled (10s delay)");
            break;
        case IDC_BTN_STOP:
            TaskScheduler::GetInstance().Stop();
            g_logWriter.WriteLog("System: Scheduler stopped");
            MessageBox(hwnd, L"Scheduler stopped. Application will exit when window closes.", L"Info", MB_OK);
            break;
        }
        return 0;

    case WM_DESTROY:
        TaskScheduler::GetInstance().Stop(); // Ensure stop on close
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void MainWindow::CreateControls(HWND hwnd) {
    int x = 20;
    int y = 20;
    int w = 260;
    int h = 30;
    int gap = 10;

    CreateWindow(L"BUTTON", L"Start Task A (Backup, Delay 10s)", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x, y, w, h, hwnd, (HMENU)IDC_BTN_TASK_A, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
    
    y += h + gap;
    CreateWindow(L"BUTTON", L"Start Task B (Matrix, Period 5s)", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x, y, w, h, hwnd, (HMENU)IDC_BTN_TASK_B, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

    y += h + gap;
    CreateWindow(L"BUTTON", L"Start Task C (HTTP, Immediate)", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x, y, w, h, hwnd, (HMENU)IDC_BTN_TASK_C, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

    y += h + gap; // Second column start
    int col2_x = x + w + gap;
    int col2_y = 20;

    CreateWindow(L"BUTTON", L"Start Task D (Reminder, Period 60s)", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        col2_x, col2_y, w, h, hwnd, (HMENU)IDC_BTN_TASK_D, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

    col2_y += h + gap;
    CreateWindow(L"BUTTON", L"Start Task E (Stats, Delay 10s)", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        col2_x, col2_y, w, h, hwnd, (HMENU)IDC_BTN_TASK_E, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

    col2_y += h + gap;
    CreateWindow(L"BUTTON", L"Stop Scheduler", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        col2_x, col2_y, w, h, hwnd, (HMENU)IDC_BTN_STOP, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

    // Log ListBox
    y = max(y, col2_y) + h + gap * 2;
    CreateWindow(L"STATIC", L"Task Log:", WS_VISIBLE | WS_CHILD, x, y, 100, 20, hwnd, NULL, NULL, NULL);
    
    y += 20;
    g_hListBox = CreateWindow(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT,
        x, y, 540, 300, hwnd, (HMENU)IDC_LIST_LOG, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
}
