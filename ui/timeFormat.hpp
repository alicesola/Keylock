#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

// 把当前时间转成 ISO-8601 字符串（秒级）
inline std::string currentISO8601()
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm tm{};                       // 清零
#if defined(_WIN32)
    localtime_s(&tm, &t);               // Windows 安全版本
#else
    localtime_r(&t, &tm);               // POSIX
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S");
    return oss.str();                   // 例：2023-06-22T15:30:45
}