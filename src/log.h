#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
#include <fmt/format.h>

// fwd decl
namespace spdlog {
class logger;
namespace sinks {
class sink;
}
}  // namespace spdlog

enum class LogLevel { DBG = 1, INFO, WARN, ERR };

namespace _detail {

void log_impl(LogLevel level, std::string_view file, int line, std::string_view message);

template <typename... Args>
std::enable_if_t<(sizeof...(Args) != 0)>
log_impl(LogLevel level, std::string_view file, int line, std::string_view fmt,
         Args&&... args) {
    log_impl(level, file, line,
             fmt::format(fmt::runtime(fmt), std::forward<Args>(args)...));
}
}  // namespace _detail

/// @brief Logger builder
/// @note This is a simple wrapper for spdlog::logger
/// @see https://github.com/gabime/spdlog
class LogBuilder {
public:
    using level_t = LogLevel;

public:
    LogBuilder& set_name(const std::string& name) {
        name_ = name;
        return *this;
    }

    LogBuilder& set_async(bool enable) {
        async_ = enable;
        return *this;
    }

    LogBuilder& add_console_logger(level_t lv, std::string_view pattern = "");

    LogBuilder& add_daily_logger(level_t lv, std::string_view filename, int max_files,
                                 std::string_view pattern = "");

    void build();

public:
    static level_t tolevel(std::string_view);

private:
    std::string name_ = "UNKNOWN";
    std::vector<std::shared_ptr<spdlog::sinks::sink>> sinks_;
    std::shared_ptr<spdlog::logger> logger_;
    bool async_ = true;

    static const std::string DEFAULT_PATTERN;
};

#define __LOG(lv, ...) (_detail::log_impl(lv, __FILE__, __LINE__, __VA_ARGS__))
#define LOGD(...)      (__LOG(LogLevel::DBG, __VA_ARGS__))
#define LOGI(...)      (__LOG(LogLevel::INFO, __VA_ARGS__))
#define LOGW(...)      (__LOG(LogLevel::WARN, __VA_ARGS__))
#define LOGE(...)      (__LOG(LogLevel::ERR, __VA_ARGS__))
