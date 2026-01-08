#pragma once 
#include <memory> 
#include <chrono> 
#include "ITask.h" 

class ScheduledTask { 
public: 
    using TimePoint = std::chrono::system_clock::time_point; 

    ScheduledTask(std::shared_ptr<ITask> task, TimePoint runTime, bool isPeriodic = false, std::chrono::seconds interval = std::chrono::seconds(0)) 
        : m_task(std::move(task)), m_runTime(runTime), m_isPeriodic(isPeriodic), m_interval(interval) {} 

    void Execute() { if (m_task) m_task->Execute(); } // Execute wrapped task 
    std::string GetName() const { return m_task ? m_task->GetName() : "Unknown Task"; } 
    TimePoint GetRunTime() const { return m_runTime; } 
    bool IsPeriodic() const { return m_isPeriodic; } 
    std::chrono::seconds GetInterval() const { return m_interval; } 
    void UpdateNextRunTime() { m_runTime += m_interval; } // Update run time for periodic tasks 

    // Priority queue sorting rule: earlier time first 
    bool operator>(const ScheduledTask& other) const { 
        return m_runTime > other.m_runTime; 
    } 

private: 
    std::shared_ptr<ITask> m_task; 
    TimePoint m_runTime; 
    bool m_isPeriodic; 
    std::chrono::seconds m_interval; 
};
