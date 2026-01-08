#include "UnitTest.h"
#include "TaskScheduler.h"
#include "TaskFactory.h"
#include "LogWriter.h"
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

void RunUnitTests() {
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Running Unit Tests..." << std::endl;
    g_logWriter.WriteLog("TEST: Starting Unit Tests...");

    // Test 1: TaskFactory Creation
    try {
        auto taskA = TaskFactory::CreateTaskA();
        assert(taskA != nullptr);
        assert(taskA->GetName() == "Task A - File Backup");
        
        auto taskB = TaskFactory::CreateTaskB();
        assert(taskB != nullptr);

        std::cout << "[PASS] TaskFactory Creation" << std::endl;
        g_logWriter.WriteLog("TEST: TaskFactory Creation Passed");
    } catch (const std::exception& e) {
        std::cerr << "[FAIL] TaskFactory Creation: " << e.what() << std::endl;
        g_logWriter.WriteLog("TEST: TaskFactory Creation Failed");
    }

    // Test 2: ScheduledTask Priority
    try {
        auto now = std::chrono::system_clock::now();
        auto taskPtr = TaskFactory::CreateTaskA();
        
        // Task 1: Runs in 10s
        ScheduledTask t1(taskPtr, now + std::chrono::seconds(10));
        // Task 2: Runs in 5s
        ScheduledTask t2(taskPtr, now + std::chrono::seconds(5));

        // Operator > returns true if lhs time > rhs time
        // t1 (10s) > t2 (5s) should be true
        assert(t1 > t2);

        std::cout << "[PASS] ScheduledTask Priority Sorting" << std::endl;
        g_logWriter.WriteLog("TEST: ScheduledTask Priority Sorting Passed");
    } catch (const std::exception& e) {
        std::cerr << "[FAIL] ScheduledTask Priority: " << e.what() << std::endl;
    }

    // Test 3: TaskScheduler Singleton
    {
        TaskScheduler& s1 = TaskScheduler::GetInstance();
        TaskScheduler& s2 = TaskScheduler::GetInstance();
        assert(&s1 == &s2);
        std::cout << "[PASS] TaskScheduler Singleton Identity" << std::endl;
    }

    std::cout << "Unit Tests Completed." << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
}
