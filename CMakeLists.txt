cmake_minimum_required(VERSION 3.10)
project(spdlog_test)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置构建类型，Debug模式包含调试信息
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# 设置编译选项，添加调试符号
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# 设置可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 设置公共头文件路径变量
set(COMMON_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/common_include)

# 添加所有子项目
add_subdirectory(subproject1)
add_subdirectory(subproject2)
add_subdirectory(subproject3)


# ===== spdlog 路径配置（可在 configure 时覆盖） =====
set(SPDLOG_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/common_include" CACHE PATH "spdlog include directory")
set(SPDLOG_LIB_DIR     "${CMAKE_SOURCE_DIR}/lib" CACHE PATH "spdlog library directory")
# 如果你有静态库libspdlog.a
set(SPDLOG_LIBRARY "${SPDLOG_LIB_DIR}/libspdlog.a" CACHE FILEPATH "spdlog static library path")
# 如果使用动态库（windows 下可能是 spdlog.lib），你可以改为：
# set(SPDLOG_LIBRARY "${SPDLOG_LIB_DIR}/spdlog.lib" CACHE FILEPATH "spdlog import library path")

message(STATUS "spdlog 头文件: ${SPDLOG_INCLUDE_DIR}")
message(STATUS "spdlog 库: ${SPDLOG_LIBRARY}")

message(STATUS "根目录 配置完成")
message(STATUS "构建类型: ${CMAKE_BUILD_TYPE}")
message(STATUS "Build 目录: ${CMAKE_BINARY_DIR}")
message(STATUS "可执行文件输出目录: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")