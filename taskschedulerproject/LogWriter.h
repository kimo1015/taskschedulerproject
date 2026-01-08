#pragma once 
#define _CRT_SECURE_NO_WARNINGS // Avoid localtime errors
#include <fstream> 
#include <string> 
#include <chrono> 
#include <iomanip> 
#include <mutex>
#include <functional>

class LogWriter { 
public: 
    using LogCallback = std::function<void(const std::string&)>;

    LogWriter(const std::string& path = "task_log.txt") { 
        m_file.open(path, std::ios::app); 
        if (m_file.is_open()) WriteLog("LogWriter initialized"); 
    } 
    ~LogWriter() { 
        if (m_file.is_open()) { 
            WriteLog("LogWriter closed"); 
            m_file.close(); 
        } 
    } 
    LogWriter(const LogWriter&) = delete; // Disable copy 

    void SetCallback(LogCallback cb) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_callback = cb;
    }

    void WriteLog(const std::string& content) { 
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_file.is_open()) return; 
        
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); 
        struct tm tm_info;
        localtime_s(&tm_info, &now);
        
        std::stringstream ss;
        ss << "[" << std::put_time(&tm_info, "%Y-%m-%d %H:%M:%S") << "] " << content;
        std::string logLine = ss.str();

        m_file << logLine << "\n"; 
        m_file.flush(); // Ensure immediate write

        if (m_callback) {
            m_callback(logLine);
        }
    } 

private: 
    std::ofstream m_file; 
    std::mutex m_mutex;
    LogCallback m_callback;
}; 

inline LogWriter g_logWriter; // Global log instance
