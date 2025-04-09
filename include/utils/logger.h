#ifndef __LOG__
#define __LOG__

#include <mutex>

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <chrono>

template<typename T>
void append_to_stream(std::ostringstream& ss, const T& value) {
    ss << value << " ";
}
template<typename T, typename... Args>
void append_to_stream(std::ostringstream& ss, const T& value, const Args&... args) {
    ss << value << " ";
    append_to_stream(ss, args...);
}

template <typename... Args>
void log(
    const std::string& file, const int& line, 
    const std::string& pre, Args... args
)
{
    static std::mutex           mu;
    std::lock_guard<std::mutex> lock(mu);

    // timestamp
    auto    now    = std::chrono::system_clock::now();
    auto    now_c  = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_c);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    std::string timestamp = ss.str();

    std::ostringstream log_msg;
    log_msg << "[" << timestamp << "]"
            << "[" << pre << "]"
            << " ";
    // use recursive template func to expand args
    append_to_stream(log_msg, args..., "\n");
    
    // add file and line info
    log_msg << "    " << file << ":" << line;

    std::cerr << log_msg.str() << std::endl;
}

#define LOGD(...) log(__FILE__, __LINE__, "    ", __VA_ARGS__)
#define DBUG(...) log(__FILE__, __LINE__, "DBUG", __VA_ARGS__)
#define INFO(...) log(__FILE__, __LINE__, "INFO", __VA_ARGS__)
#define WARN(...) log(__FILE__, __LINE__, "WARN", __VA_ARGS__)
#define ERRO(...) log(__FILE__, __LINE__, "ERRO", __VA_ARGS__)
#define FATL(...) log(__FILE__, __LINE__, "FATL", __VA_ARGS__)

#endif
