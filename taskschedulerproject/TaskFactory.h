#pragma once 
#include <memory> 
#include "ITask.h" 

class TaskFactory { 
public: 
    TaskFactory() = delete; // Static factory, disable construction 
    static std::shared_ptr<ITask> CreateTaskA(); // File Backup 
    static std::shared_ptr<ITask> CreateTaskB(); // Matrix Multiplication 
    static std::shared_ptr<ITask> CreateTaskC(); // HTTP GET 
    static std::shared_ptr<ITask> CreateTaskD(); // Class Reminder 
    static std::shared_ptr<ITask> CreateTaskE(); // Random Number Stats 
};
