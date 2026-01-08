// Include Google Test
#include <gtest/gtest.h>
// Include task headers
#include "ITask.h"
#include "TaskFactory.h"
#include "TaskA.h"
#include "ScheduledTask.h"
#include <chrono>

// Test Suite 1: Basic ITask Interface
TEST(ITaskInterfaceTest, GetNameReturnCorrectValue) {
    // Create TaskA instance
    auto taskA = TaskFactory::CreateTaskA();
    // Assert: Check correct name
    EXPECT_EQ(taskA->GetName(), "Task A - File Backup");
    // Assert: Not null
    EXPECT_NE(taskA, nullptr);
}

// Test Suite 2: TaskA Execution
TEST(TaskATest, ExecuteNoException) {
    auto taskA = TaskFactory::CreateTaskA();
    // Assert: No exception thrown
    EXPECT_NO_THROW(taskA->Execute());
}

// Test Suite 3: ScheduledTask Wrapper
TEST(ScheduledTaskTest, WrapperTaskFunction) {
    auto taskA = TaskFactory::CreateTaskA();
    auto now = std::chrono::system_clock::now();
    ScheduledTask scheduledTask(taskA, now);

    // Assert: Name consistency
    EXPECT_EQ(scheduledTask.GetName(), taskA->GetName());
    // Assert: No exception execution
    EXPECT_NO_THROW(scheduledTask.Execute());
}

// Entry point exposed to main
void RunGoogleTests() {
    int argc = 1;
    char* argv[] = { (char*)"taskscheduler_tests", NULL };
    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    (void)result; // Ignore result
}
