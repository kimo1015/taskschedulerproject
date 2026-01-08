#pragma once 
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include <shellapi.h>
#include "ITask.h" 
#include "LogWriter.h" 
#include "TaskFactory.h"
#include <shlobj.h> 
#include <string> 
#include <ctime> 
#include <vector>

#pragma comment(lib, "Shell32.lib")

class TaskA : public ITask { 
public: 
    std::string GetName() const override { return "Task A - File Backup"; } 
    void Execute() override { 
        try { 
            // Generate backup filename: backup_YYYYMMDD.zip 
            char dateBuf[32]; 
            std::time_t t = std::time(nullptr);
            std::tm tm_info;
            localtime_s(&tm_info, &t);
            std::strftime(dateBuf, sizeof(dateBuf), "backup_%Y%m%d.zip", &tm_info); 
            
            // Construct double-null terminated string
            std::string srcPath = "C:\\Data";
            std::string destDir = "D:\\Backup\\";
            std::string destPath = destDir + dateBuf;

            // Ensure directory exists (simple attempt to create)
            CreateDirectoryA("D:\\Backup", NULL);

            // SHFileOperation needs double-null terminated string
            std::vector<char> src(srcPath.begin(), srcPath.end());
            src.push_back('\0');
            src.push_back('\0');

            std::vector<char> dest(destPath.begin(), destPath.end());
            dest.push_back('\0');
            dest.push_back('\0');

            SHFILEOPSTRUCTA fileOp = {0};
            fileOp.wFunc = FO_COPY;
            fileOp.pFrom = src.data();
            fileOp.pTo = dest.data();
            fileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;

            // Call Windows Shell copy
            // Assuming file copy for demonstration as per user request.
            
            int result = SHFileOperationA(&fileOp); 
            
            if (result == 0) {
                 g_logWriter.WriteLog(GetName() + " Success: Backup to " + destPath); 
            } else {
                 g_logWriter.WriteLog(GetName() + " Failed, Error Code: " + std::to_string(result)); 
            }
        } catch (const std::exception& e) { 
            g_logWriter.WriteLog(GetName() + " Exception: " + e.what()); 
        } 
    } 
}; 

// Implement TaskFactory::CreateTaskA 
inline std::shared_ptr<ITask> TaskFactory::CreateTaskA() { 
    return std::make_shared<TaskA>(); 
} 
