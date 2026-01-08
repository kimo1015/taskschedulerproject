#include "TaskScheduler.h" 
#include "TaskFactory.h" 
#include "TaskA.h"
#include "TaskB.h"
#include "TaskC.h"
#include "TaskD.h"
#include "TaskE.h"
#include "MainWindow.h"
#include "UnitTest.h"
#include <iostream> 

int main() { 
    // Initialize Scheduler Singleton
    auto& scheduler = TaskScheduler::GetInstance(); 
    
    // Run Unit Tests
    RunUnitTests();
    
    // Run Google Tests
    RunGoogleTests();

    // Hide Console Window
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    // Run GUI
    MainWindow::Run(GetModuleHandle(NULL));

    return 0; 
}
