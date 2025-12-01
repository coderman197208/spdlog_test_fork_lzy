#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

void stdout_example()
{
    // 创建一个彩色的多线程 logger
    auto console = spdlog::stdout_color_mt("console");    
    spdlog::get("console")->info("日志记录器可以从一个全局注册表中检索/获取...");
}

int main()
{
    stdout_example();
    return 0;
}