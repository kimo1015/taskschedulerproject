#pragma once 
#include "ITask.h" 
#include "LogWriter.h" 
#include "TaskFactory.h"
#include <vector>
#include <random>
#include <numeric>
#include <cmath>
#include <sstream>

class TaskE : public ITask { 
public: 
    std::string GetName() const override { return "Task E - Random Number Stats"; } 
    void Execute() override { 
        std::vector<int> numbers(100);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);

        for (auto& num : numbers) {
            num = dis(gen);
        }

        double sum = std::accumulate(numbers.begin(), numbers.end(), 0.0);
        double mean = sum / numbers.size();

        double sq_sum = std::inner_product(numbers.begin(), numbers.end(), numbers.begin(), 0.0);
        double stdev = std::sqrt(sq_sum / numbers.size() - mean * mean);

        std::stringstream ss;
        ss << "Mean: " << mean << ", Stdev: " << stdev;
        g_logWriter.WriteLog(GetName() + " Stats: " + ss.str());
    } 
}; 

inline std::shared_ptr<ITask> TaskFactory::CreateTaskE() { 
    return std::make_shared<TaskE>(); 
} 
