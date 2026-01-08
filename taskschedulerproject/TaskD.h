#pragma once 
#define WIN32_LEAN_AND_MEAN
#include "ITask.h" 
#include "LogWriter.h" 
#include "TaskFactory.h"
#include <windows.h>

class TaskD : public ITask { 
public: 
    std::string GetName() const override { return "Task D - Class Reminder"; } 
    void Execute() override { 
        g_logWriter.WriteLog(GetName() + " Reminder: It is class time!");
        // MessageBoxA(NULL, "It is class time!", "Reminder", MB_OK | MB_ICONINFORMATION);
    } 
}; 

inline std::shared_ptr<ITask> TaskFactory::CreateTaskD() { 
    return std::make_shared<TaskD>(); 
} 
