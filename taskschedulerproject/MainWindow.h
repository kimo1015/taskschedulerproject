#pragma once
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include <string>

// UI Control IDs
#define IDC_BTN_TASK_A 101
#define IDC_BTN_TASK_B 102
#define IDC_BTN_TASK_C 103
#define IDC_BTN_TASK_D 104
#define IDC_BTN_TASK_E 105
#define IDC_BTN_STOP   106
#define IDC_LIST_LOG   107

class MainWindow {
public:
    static void Run(HINSTANCE hInstance);
    
private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void CreateControls(HWND hwnd);
    static void AddLogItem(HWND hwnd, const std::string& log);
};
