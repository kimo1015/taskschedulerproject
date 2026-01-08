#pragma once 
#include <string> 

class ITask { 
public: 
    virtual ~ITask() = default; 
    virtual std::string GetName() const = 0; // Get task name 
    virtual void Execute() = 0;              // Execute task logic 
};
