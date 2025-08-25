
#include "pch.h"

void init_logger() {
    LogBuilder builder;
    builder.add_console_logger(LogLevel::INFO);
    // builder.add_daily_logger(LogLevel::INFO, "./log/test.log", 30);
    builder.set_async(false).build();
    LOGD("Logger init ok.");
}

net::task<void> async_main();

int main() {
    init_logger();
    CO_SPAWN("main", async_main());
    g_asp.run();
    return 0;
}

net::task<void>  //
async_main() {
    for (int i = 0; i < 6; i++) {
        LOGI("Hello {}", i + 1);
        co_await cc::timeout(100);
    }
}
