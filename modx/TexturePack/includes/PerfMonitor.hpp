#pragma once
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <mutex>
#include <functional>
#include <dirent.h>
#include <sys/sysinfo.h>
#include <android/log.h>
#include <malloc.h>
#include <cinttypes>
#include <memory>
#include <atomic>

class AndroidPerfMonitor {
public:


    enum class MemoryUnit {
        KB,
        MB,
        GB,
        AUTO
    };

    enum class TimeUnit {
        MS,
        SEC,
        MIN,
        AUTO
    };

    // 输出模式枚举
    enum class OutputMode {
        LOG,     // 只输出到logcat
        FILE,    // 只输出到文件
        BOTH     // 同时输出到logcat和文件
    };

    struct IoStats {
        int64_t readBytes;
        int64_t writeBytes;

        IoStats operator-(const IoStats& other) const {
            return {
                    readBytes - other.readBytes,
                    writeBytes - other.writeBytes
            };
        }
    };

    struct PerformanceStats {
        int64_t memoryKb;
        float cpuUsage;
        int threadCount;
        IoStats io;
    };

    // 回调函数类型
    using MonitorCallback = std::function<void()>;

    // 构造函数，可以指定输出文件路径
    explicit AndroidPerfMonitor(const std::string& outputFile = "")
            : m_outputFile(outputFile), m_outputMode(OutputMode::LOG) {}

    // 设置内存显示单位
    void SetMemoryUnit(MemoryUnit unit) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_memoryUnit = unit;
    }

    // 设置时间显示单位
    void SetTimeUnit(TimeUnit unit) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_timeUnit = unit;
    }

    // 设置输出模式
    void SetOutputMode(OutputMode mode) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_outputMode = mode;
    }

    void SetCallback(MonitorCallback callback) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_callback = std::move(callback);
    }

    void Begin(const std::string& taskName) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_taskName = taskName;
        m_startTime = std::chrono::steady_clock::now();
        m_startStats = GetCurrentStats();
        m_isMonitoring.store(true);

        if (m_callback) {
            m_callback();
        }

        OutputToTarget("开始监测，任务: " + taskName);
    }

    void End() {
        auto endStats = GetCurrentStats();
        auto duration = std::chrono::steady_clock::now() - m_startTime;
        m_isMonitoring.store(false);

        std::lock_guard<std::mutex> lock(m_mutex);
        std::string report = GenerateReport(duration, m_startStats, endStats);

        if (m_callback) {
            m_callback();
        }

        OutputToTarget(report);
        OutputToTarget("监测结束");
    }

    PerformanceStats GetCurrentStatsSnapshot() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return GetCurrentStats();
    }

private:

    std::string FormatMemory(int64_t kb) const {
        double value;
        const char* unit;

        if (m_memoryUnit == MemoryUnit::AUTO) {
            if (kb >= 1024 * 1024) {
                value = kb / (1024.0 * 1024.0);
                unit = "GB";
            } else if (kb >= 1024) {
                value = kb / 1024.0;
                unit = "MB";
            } else {
                value = kb;
                unit = "KB";
            }
        } else {
            switch (m_memoryUnit) {
                case MemoryUnit::GB:
                    value = kb / (1024.0 * 1024.0);
                    unit = "GB";
                    break;
                case MemoryUnit::MB:
                    value = kb / 1024.0;
                    unit = "MB";
                    break;
                case MemoryUnit::KB:
                default:
                    value = kb;
                    unit = "KB";
                    break;
            }
        }

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << value << " " << unit;
        return ss.str();
    }

    // 格式化时间
    std::string FormatTime(int64_t ms) const {
        double value;
        const char* unit;

        if (m_timeUnit == TimeUnit::AUTO) {
            if (ms >= 60000) {
                value = ms / 60000.0;
                unit = "min";
            } else if (ms >= 1000) {
                value = ms / 1000.0;
                unit = "s";
            } else {
                value = ms;
                unit = "ms";
            }
        } else {
            switch (m_timeUnit) {
                case TimeUnit::MIN:
                    value = ms / 60000.0;
                    unit = "min";
                    break;
                case TimeUnit::SEC:
                    value = ms / 1000.0;
                    unit = "s";
                    break;
                case TimeUnit::MS:
                default:
                    value = ms;
                    unit = "ms";
                    break;
            }
        }

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << value << " " << unit;
        return ss.str();
    }

    // 生成报告
    std::string GenerateReport(auto duration, const PerformanceStats& start, const PerformanceStats& end) {
        int64_t durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        int64_t memoryDelta = end.memoryKb - start.memoryKb;

        std::string memoryStr = FormatMemory(end.memoryKb);
        std::string deltaMemoryStr = FormatMemory(abs(memoryDelta));
        deltaMemoryStr = (memoryDelta >= 0) ? "+" + deltaMemoryStr : "-" + deltaMemoryStr;

        std::string timeStr = FormatTime(durationMs);

        std::stringstream ss;
        ss << "\n┌───────────────────────────────────────┐";
        ss << "\n│           性能分析报告               │";
        ss << "\n├───────────────────────────────────────┤";
        ss << "\n│ 任务: " << std::setw(28) << std::left << m_taskName << "│";
        ss << "\n├───────────────────────────────────────┤";
        ss << "\n│ 耗时: " << std::setw(10) << timeStr << "           │";
        ss << "\n│ 内存: " << std::setw(10) << memoryStr << " (" << deltaMemoryStr << " Δ) │";
        ss << "\n│ CPU: " << std::setw(8) << std::fixed << std::setprecision(1)
           << end.cpuUsage << "% (" << (end.cpuUsage - start.cpuUsage) << " Δ) │";
        ss << "\n│ 线程: " << std::setw(4) << end.threadCount << " ("
           << (end.threadCount - start.threadCount) << " Δ)        │";

        // 格式化IO数据
        int64_t readDelta = end.io.readBytes - start.io.readBytes;
        int64_t writeDelta = end.io.writeBytes - start.io.writeBytes;

        auto formatIo = [](int64_t bytes) {
            if (bytes >= 1024 * 1024) {
                return std::to_string(bytes / (1024 * 1024)) + " MB";
            } else if (bytes >= 1024) {
                return std::to_string(bytes / 1024) + " KB";
            }
            return std::to_string(bytes) + " B";
        };

        ss << "\n│ 读IO: " << std::setw(10) << formatIo(readDelta) << "           │";
        ss << "\n│ 写IO: " << std::setw(10) << formatIo(writeDelta) << "           │";
        ss << "\n└───────────────────────────────────────┘";

        return ss.str();
    }

    // 输出到指定目标
    void OutputToTarget(const std::string& message) {
        switch (m_outputMode) {
            case OutputMode::LOG:
                __android_log_print(ANDROID_LOG_INFO, "AndroidPerfMonitor", "%s", message.c_str());
                break;
            case OutputMode::FILE:
                if (!m_outputFile.empty()) {
                    std::ofstream outFile(m_outputFile, std::ios::app);
                    if (outFile.is_open()) {
                        outFile << message << std::endl;
                    }
                }
                break;
            case OutputMode::BOTH:
                __android_log_print(ANDROID_LOG_INFO, "AndroidPerfMonitor", "%s", message.c_str());
                if (!m_outputFile.empty()) {
                    std::ofstream outFile(m_outputFile, std::ios::app);
                    if (outFile.is_open()) {
                        outFile << message << std::endl;
                    }
                }
                break;
        }
    }

    // 获取当前性能数据
    PerformanceStats GetCurrentStats() {
        return {
                .memoryKb = GetMemoryUsage(),
                .cpuUsage = GetCpuUsage(),
                .threadCount = GetThreadCount(),
                .io = GetIoStats()
        };
    }

    // 获取内存使用量
    int64_t GetMemoryUsage() {
        std::ifstream status("/proc/self/status");
        std::string line;
        while (std::getline(status, line)) {
            if (line.find("VmRSS:") == 0) {
                int64_t kb;
                sscanf(line.c_str() + 6, "%" SCNd64, &kb);
                return kb;
            }
        }
        return 0;
    }

    // 获取CPU使用率
    float GetCpuUsage() {
        static int64_t lastTotal = 0, lastProcess = 0;

        std::ifstream stat("/proc/stat");
        std::string line;
        std::getline(stat, line);
        int64_t user, nice, sys, idle;
        sscanf(line.c_str() + 5, "%" SCNd64 " %" SCNd64 " %" SCNd64 " %" SCNd64,
               &user, &nice, &sys, &idle);
        int64_t total = user + nice + sys + idle;

        std::ifstream proc("/proc/self/stat");
        for (int i = 0; i < 13; ++i) proc.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
        int64_t utime, stime;
        proc >> utime >> stime;
        int64_t process = utime + stime;

        float usage = 0.0f;
        if (lastTotal > 0) {
            usage = 100.0f * (process - lastProcess) / (total - lastTotal);
        }

        lastTotal = total;
        lastProcess = process;
        return usage;
    }

    // 获取线程数
    int GetThreadCount() {
        int count = 0;
        if (auto dir = opendir("/proc/self/task")) {
            while (auto entry = readdir(dir)) {
                if (entry->d_name[0] != '.') count++;
            }
            closedir(dir);
        }
        return count;
    }

    // 获取IO状态
    IoStats GetIoStats() {
        std::ifstream io("/proc/self/io");
        IoStats stats{0, 0};
        std::string line;
        while (std::getline(io, line)) {
            if (line.find("read_bytes:") == 0) {
                sscanf(line.c_str() + 11, "%" SCNd64, &stats.readBytes);
            } else if (line.find("write_bytes:") == 0) {
                sscanf(line.c_str() + 12, "%" SCNd64, &stats.writeBytes);
            }
        }
        return stats;
    }

    std::string m_taskName;
    std::chrono::steady_clock::time_point m_startTime;
    PerformanceStats m_startStats;
    std::mutex m_mutex;
    MemoryUnit m_memoryUnit = MemoryUnit::AUTO;
    TimeUnit m_timeUnit = TimeUnit::AUTO;
    OutputMode m_outputMode;
    std::string m_outputFile;
    MonitorCallback m_callback;
    std::atomic<bool> m_isMonitoring{false};
};