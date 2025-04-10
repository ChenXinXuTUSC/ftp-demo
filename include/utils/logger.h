#ifndef __LOG__
#define __LOG__

#include <mutex>

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <chrono>

enum loglvl
{
    FATL = 0,
    ERRO = 1,
    WARN = 2,
    INFO = 3,
    DBUG = 4,
    NONE = 5
};

// 预定义的日志等级颜色
const std::string lvl_clr_list[] 
{
    "\033[35m", // magenta
    "\033[31m", // red
    "\033[33m", // yellow
    "\033[32m", // green
    "\033[34m", // blue
    "\033[0m",  // none
};
const std::string reset_clr = "\033[0m"; // reset color
// 预定义的日志等级前缀
const std::string lvl_pre_list[] 
{
    "FATL",
    "ERRO",
    "WARN",
    "INFO",
    "DBUG",
    "NONE"
};

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
    loglvl lvl,
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
            << lvl_clr_list[lvl] << "[" << lvl_pre_list[lvl] << "]" << reset_clr
            << "[" << pre << "]"
            << " ";
    // use recursive template func to expand args
    append_to_stream(log_msg, args..., "\n");
    
    // add file and line info
    log_msg << "    " << file << ":" << line;

    std::cerr << log_msg.str() << std::endl;
}

#define FATL(...) log(__FILE__, __LINE__, loglvl::FATL, "", __VA_ARGS__);
#define ERRO(...) log(__FILE__, __LINE__, loglvl::ERRO, "", __VA_ARGS__);
#define WARN(...) log(__FILE__, __LINE__, loglvl::WARN, "", __VA_ARGS__);
#define INFO(...) log(__FILE__, __LINE__, loglvl::INFO, "", __VA_ARGS__);
#define DBUG(...) log(__FILE__, __LINE__, loglvl::DBUG, "", __VA_ARGS__);
#define LOGD(...) log(__FILE__, __LINE__, loglvl::NONE, "", __VA_ARGS__);

#endif
