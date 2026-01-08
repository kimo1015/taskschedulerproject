#pragma once 
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include "ITask.h" 
#include "LogWriter.h" 
#include "TaskFactory.h"
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "ws2_32.lib")

class TaskC : public ITask { 
public: 
    std::string GetName() const override { return "Task C - HTTP GET"; } 
    void Execute() override { 
        HINTERNET hInternet = InternetOpenA("TaskSchedulerAgent", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (!hInternet) {
            g_logWriter.WriteLog(GetName() + " InternetOpen failed");
            return;
        }

        HINTERNET hConnect = InternetOpenUrlA(hInternet, "https://www.example.com", NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (hConnect) {
            char buffer[1024];
            DWORD bytesRead;
            std::string content;
            // Read small amount of data as example
            if (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
                buffer[bytesRead] = 0;
                content = buffer;
                // Truncate log if too long
                if (content.length() > 50) content = content.substr(0, 50) + "...";
                g_logWriter.WriteLog(GetName() + " Success: " + content);
            } else {
                g_logWriter.WriteLog(GetName() + " Read failed or empty");
            }
            InternetCloseHandle(hConnect);
        } else {
            g_logWriter.WriteLog(GetName() + " Connection failed");
        }
        InternetCloseHandle(hInternet);
    } 
}; 

inline std::shared_ptr<ITask> TaskFactory::CreateTaskC() { 
    return std::make_shared<TaskC>(); 
} 
