#pragma once 
#include <queue> 
#include <vector> 
#include <mutex> 
#include <condition_variable> 
#include <thread> 
#include <memory> 
#include <algorithm>
#include "ScheduledTask.h" 

// Observer Interface
class ITaskObserver {
public:
    virtual void OnTaskComplete(const std::string& taskName, const std::string& result) = 0;
};

class TaskScheduler { 
public: 
    static TaskScheduler& GetInstance() { // Singleton instance 
        static TaskScheduler instance; 
        return instance; 
    } 

    // Disable copy/move 
    TaskScheduler(const TaskScheduler&) = delete; 
    TaskScheduler& operator=(const TaskScheduler&) = delete; 

    // Add task interfaces 
    void AddOneTimeTask(std::shared_ptr<ITask> task) { 
        auto now = std::chrono::system_clock::now(); 
        AddTask(std::make_shared<ScheduledTask>(std::move(task), now, false)); 
    } 

    // Alias for compatibility
    void AddImmediateTask(std::shared_ptr<ITask> task) {
        AddOneTimeTask(std::move(task));
    }

    void AddDelayedTask(std::shared_ptr<ITask> task, std::chrono::seconds delay) { 
        auto runTime = std::chrono::system_clock::now() + delay; 
        AddTask(std::make_shared<ScheduledTask>(std::move(task), runTime, false)); 
    } 
    void AddPeriodicTask(std::shared_ptr<ITask> task, std::chrono::seconds interval, std::chrono::seconds initialDelay = std::chrono::seconds(0)) { 
        auto runTime = std::chrono::system_clock::now() + initialDelay; 
        AddTask(std::make_shared<ScheduledTask>(std::move(task), runTime, true, interval)); 
    } 

    // Observer methods
    void AddObserver(ITaskObserver* observer) {
        std::lock_guard<std::mutex> lock(m_observerMutex);
        m_observers.push_back(observer);
    }

    void RemoveObserver(ITaskObserver* observer) {
        std::lock_guard<std::mutex> lock(m_observerMutex);
        m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer), m_observers.end());
    }

    void NotifyObservers(const std::string& taskName, const std::string& result) {
        std::lock_guard<std::mutex> lock(m_observerMutex);
        for (auto observer : m_observers) {
            observer->OnTaskComplete(taskName, result);
        }
    }

    void Start() {
        if (!m_workerThread.joinable()) {
             m_stop = false;
             m_workerThread = std::thread(&TaskScheduler::WorkerLoop, this);
        }
    }

    void Stop() { // Stop scheduler 
        { 
            std::lock_guard<std::mutex> lock(m_mutex); 
            m_stop = true; 
        } 
        m_cv.notify_all(); 
        if (m_workerThread.joinable()) m_workerThread.join(); 
    } 

private: 
    TaskScheduler() : m_workerThread(&TaskScheduler::WorkerLoop, this) {} 
    ~TaskScheduler() { Stop(); } 

    void WorkerLoop() { // Worker thread logic 
        while (true) { 
            std::unique_lock<std::mutex> lock(m_mutex); 
            // Wait for task or stop signal 
            if (m_taskQueue.empty()) {
                m_cv.wait(lock, [this]() { return m_stop || !m_taskQueue.empty(); });
            } else {
                m_cv.wait_until(lock, m_taskQueue.top()->GetRunTime(), 
                    [this]() { return m_stop || (!m_taskQueue.empty() && m_taskQueue.top()->GetRunTime() <= std::chrono::system_clock::now()); }); 
            }

            if (m_stop) break; 

            if (!m_taskQueue.empty() && m_taskQueue.top()->GetRunTime() <= std::chrono::system_clock::now()) { 
                auto task = m_taskQueue.top(); // Copy shared_ptr
                m_taskQueue.pop(); 
                lock.unlock(); 

                try {
                    task->Execute(); // Execute task 
                    NotifyObservers(task->GetName(), "Success");
                } catch (const std::exception& e) {
                    NotifyObservers(task->GetName(), std::string("Failed: ") + e.what());
                } catch (...) {
                    NotifyObservers(task->GetName(), "Failed: Unknown Exception");
                }

                // Re-add periodic task 
                if (task->IsPeriodic()) { 
                    task->UpdateNextRunTime(); 
                    AddTask(std::move(task)); 
                } 
            } 
        } 
    } 

    void AddTask(std::shared_ptr<ScheduledTask> task) { // Thread-safe add task 
        std::lock_guard<std::mutex> lock(m_mutex); 
        m_taskQueue.push(std::move(task)); 
        m_cv.notify_one(); 
    } 

    struct CompareTask {
        bool operator()(const std::shared_ptr<ScheduledTask>& a, const std::shared_ptr<ScheduledTask>& b) {
            return *a > *b;
        }
    };

    std::priority_queue<std::shared_ptr<ScheduledTask>, std::vector<std::shared_ptr<ScheduledTask>>, CompareTask> m_taskQueue; 
    std::vector<ITaskObserver*> m_observers;
    std::mutex m_mutex; 
    std::mutex m_observerMutex;
    std::condition_variable m_cv; 
    std::thread m_workerThread; 
    bool m_stop = false; 
};
