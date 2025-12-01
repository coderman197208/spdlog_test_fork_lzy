//基础文件日志测试
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <iostream>

void test_basic_file_log() {
    try {
        // 1. 创建 Logger 对象
        // 文件路径相对于程序执行目录
        auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt");
        
        // 可选：设置日志级别，确保能输出 info 级别
        logger->set_level(spdlog::level::info);

        // 2. 使用 Logger 记录日志
        logger->info("Test message 1: Logger创建成功.");
        logger->warn("Test message 2: 发生了一个重要的警告.");
        
        // 3. 强制刷新，确保写入磁盘
        logger->flush();

        std::cout << "日志消息已发送到记录器。请检查文件 'logs/basic-log.txt'。" << std::endl;

    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "日志初始化失败: " << ex.what() << std::endl;
    }
}

// 轮转文件日志测试
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <iostream>
#include <chrono>   // 用于时间点和持续时间
#include <thread>   // 用于睡眠函数

// 定义一个便于观察的 Log 函数
void test_rotating_log_with_sleep() {
    try {
        // --- 1. 配置参数 (1 KB, 3 个备份文件) ---
        auto max_size_test = 1024;        // 1 KB (1024 字节)
        auto max_files_test = 3;          // 保留 3 个备份文件 (.0, .1, .2)
        int rotation_count_target = 5;    // 目标是触发 5 次轮换

        // 创建 Rotating Logger
        auto logger = spdlog::rotating_logger_mt(
            "test_rotating_logger",
            "logs/rotating_test.txt", // 文件路径
            max_size_test,
            max_files_test
        );

        logger->set_level(spdlog::level::info);
        // 设置简洁的日志模式，方便估算大小
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] %v");
        
        // --- 2. 准备日志消息和循环 ---
        
        // 假设每条日志消息包含时间戳和序号后大约占用 50 字节
        // 1 KB / 50 字节 ≈ 20 条日志触发一次轮换
        int lines_per_rotation = 20; 
        
        // 为了触发 5 次轮换 (需要 6 个文件大小的数据)，总共记录：
        int total_lines_to_log = lines_per_rotation * (max_files_test + rotation_count_target); // 20 * 8 = 160 行
        
        std::cout << "--- 开始轮转测试 (1KB max size) ---" << std::endl;
        std::cout << "目标轮转次数: " << rotation_count_target << std::endl;
        std::cout << "总共要写入的日志行数: " << total_lines_to_log << std::endl;

        // --- 3. 循环输出日志并睡眠 ---
        for (int i = 0; i < total_lines_to_log; ++i) {
            
            // 记录日志
            logger->info("日志行号: {} - 该行应在第 {} 行附近触发轮换", i, lines_per_rotation * (i / lines_per_rotation + 1));
            
            // 确保日志立即写入磁盘，方便观察
            logger->flush(); 

            // 睡眠 200 毫秒，减缓速度
            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            // 每当接近轮换点时，提示用户观察文件变化
            if (i > 0 && i % lines_per_rotation == 0) {
                 std::cout << "--- 轮换应在此点附近触发 (行号: " << i << ") ---" << std::endl;
            }
        }
        
        std::cout << "--- 日志写入完成。请检查 'logs/rotating_test.txt' 及其备份文件。 ---" << std::endl;
        
    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "日志初始化失败: " << ex.what() << std::endl;
    }
}

// 每日日志文件测试
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

void test_daily_log() {
    try {
        // 定义固定的轮换时间点
        const int rotation_hour = 15;
        const int rotation_minute = 30;

        // 强制 spdlog 在遇到任何内部错误时，将错误信息打印到控制台
        spdlog::set_error_handler([](const std::string& msg) {
            std::cerr << "\n[!!! SPDLOG ERROR !!!] 轮换或文件操作失败: " << msg << std::endl;
        });

        // 创建 Daily Logger
        auto logger = spdlog::daily_logger_mt(
            "daily_logger", 
            "logs/daily_test.txt", // 目标文件
            rotation_hour, 
            rotation_minute
        );

        logger->set_level(spdlog::level::info);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] %v");
        
        std::cout << "✅ 日志已配置为在今天 " 
                  << rotation_hour << ":" 
                  << rotation_minute << " 轮换。" << std::endl;
        std::cout << "-----------------------------------------------" << std::endl;

        // 2. 记录初始日志 (文件生成)
        for (int i = 0; i < 10; ++i) { 

            logger->info("初始日志 (轮换前) - 运行次数: {}", i);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));; 
        }
        logger->flush();
        
        std::cout << "\n[文件状态] logs/daily_fixed_test.txt 已创建并包含初始日志." << std::endl;
        std::cout << "=========================================================" << std::endl;
        std::cout << "🚨 请手动进行以下操作:" << std::endl;
        std::cout << "   1. 将系统时间调整到" << rotation_hour << ":" << rotation_minute + 1 << " AM (例如 02:31 AM)。" << std::endl;
        std::cout << "   2. 完成后，请按 ENTER 键继续记录日志..." << std::endl;
        std::cout << "=========================================================\n" << std::endl;

        // 暂停等待用户输入
        std::cin.get();

        // 3. 记录轮换后的日志
        logger->warn("轮换后的第一条日志");
        logger->warn("确认：这行消息应该出现在新的日志文件中。");

         //强制刷新，确保新文件被创建和写入
        logger->flush();
        
        std::cout << "-----------------------------------------------" << std::endl;
        std::cout << "✅ 程序执行完毕，请检查 logs 目录下的文件。" << std::endl;

    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "日志初始化失败: " << ex.what() << std::endl;
    }
}

int main() {
    test_daily_log();
    return 0;
}

