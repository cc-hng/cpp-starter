#include "log.h"
#include "pch.h"
#include <algorithm>
#include <string_view>
#include <gsl/gsl>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include <spdlog/async.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

using namespace std::chrono_literals;

static inline spdlog::level::level_enum to_spdlog_level(LogLevel level) {
    switch (level) {
    case LogLevel::DBG: return spdlog::level::debug;
    case LogLevel::INFO: return spdlog::level::info;
    case LogLevel::WARN: return spdlog::level::warn;
    case LogLevel::ERR: return spdlog::level::err;
    default: return spdlog::level::err;
    }
}

namespace _detail {

void log_impl(LogLevel level, std::string_view file, int line, std::string_view msg) {
    int sz = msg.size();
    if (GSL_UNLIKELY(ss::str_ends_with(msg, "\r\n"))) {
        msg = msg.substr(0, sz - 2);
    } else if (GSL_UNLIKELY(ss::str_ends_with(msg, "\n"))) {
        msg = msg.substr(0, sz - 1);
    }

    spdlog::log(spdlog::source_loc{file.data(), line, ""}, to_spdlog_level(level), msg);
}
}  // namespace _detail

const std::string LogBuilder::DEFAULT_PATTERN =
    " %^%L%$ | %Y-%m-%dT%H:%M:%S.%e | %5t | %s:%-3# | %v";

LogBuilder& LogBuilder::add_console_logger(level_t lv, std::string_view pattern) {
    auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console->set_pattern((pattern.empty() ? DEFAULT_PATTERN : std::string(pattern)));
    console->set_level(to_spdlog_level(lv));
    sinks_.push_back(console);
    return *this;
}

LogBuilder& LogBuilder::add_daily_logger(level_t lv, std::string_view filename,
                                         int max_files, std::string_view pattern) {
    auto daily =
        std::make_shared<spdlog::sinks::daily_file_sink_mt>(std::string(filename), 0, 0,
                                                            false, max_files);
    daily->set_pattern((pattern.empty() ? DEFAULT_PATTERN : std::string(pattern)));
    daily->set_level(to_spdlog_level(lv));
    sinks_.push_back(daily);
    return *this;
}

void LogBuilder::build() {
    // // create logger
    // if (async_) {
    //     spdlog::init_thread_pool(8192, 1);
    //
    //     logger_ =
    //         std::make_shared<spdlog::async_logger>(name_, sinks_.begin(), sinks_.end(),
    //                                                spdlog::thread_pool(),
    //                                                spdlog::async_overflow_policy::block);
    // } else {
    //     logger_ = std::make_shared<spdlog::logger>(name_, sinks_.begin(), sinks_.end());
    // }
    //
    logger_ = std::make_shared<spdlog::logger>(name_, sinks_.begin(), sinks_.end());
    logger_->set_level(spdlog::level::trace);
    logger_->set_pattern(DEFAULT_PATTERN);
    logger_->flush_on(spdlog::level::warn);
    spdlog::set_default_logger(logger_);

    // periodically flush all *registered* loggers every 3 seconds:
    // warning: only use if all your loggers are thread safe ("_mt" loggers)
    spdlog::flush_every(3s);
}

LogBuilder::level_t LogBuilder::tolevel(std::string_view sl) {
    static std::unordered_map<std::string, LogLevel> m = {
        {"debug", LogLevel::DBG },
        {"info",  LogLevel::INFO},
        {"warn",  LogLevel::WARN},
        {"error", LogLevel::ERR },
    };

    auto l = ss::str_tolower(sl);
    return m.count(l) ? m.at(l) : LogLevel::INFO;
}
