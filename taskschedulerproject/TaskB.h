#pragma once 
#include "ITask.h" 
#include "LogWriter.h" 
#include "TaskFactory.h"
#include <vector>
#include <random>
#include <sstream>

class TaskB : public ITask { 
public: 
    std::string GetName() const override { return "Task B - Matrix Multiplication"; } 
    void Execute() override { 
        try { 
            int size = 100; // 100x100 matrix
            std::vector<std::vector<int>> A(size, std::vector<int>(size, 1));
            std::vector<std::vector<int>> B(size, std::vector<int>(size, 2));
            std::vector<std::vector<int>> C(size, std::vector<int>(size, 0));

            // Simple multiplication
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    for (int k = 0; k < size; ++k) {
                        C[i][j] += A[i][k] * B[k][j];
                    }
                }
            }
            
            g_logWriter.WriteLog(GetName() + " Finished: " + std::to_string(size) + "x" + std::to_string(size) + " calculation");
        } catch (const std::exception& e) { 
            g_logWriter.WriteLog(GetName() + " Exception: " + e.what()); 
        } 
    } 
}; 

inline std::shared_ptr<ITask> TaskFactory::CreateTaskB() { 
    return std::make_shared<TaskB>(); 
} 
