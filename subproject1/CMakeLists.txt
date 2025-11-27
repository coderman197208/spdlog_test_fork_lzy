#设置 PROJECT_NAME 变量为 SubProject1
project(subproject1)

# 查找源文件
file(GLOB SOURCES "src/*.cpp")

# 创建可执行文件 - 会自动输出到根目录的 bin
add_executable(${PROJECT_NAME} ${SOURCES})

# 添加头文件包含路径
target_include_directories(${PROJECT_NAME}
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include
    PUBLIC
        ${COMMON_INCLUDE_DIR}
)

# 可选：设置可执行文件属性
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "subproject1"  # 确保输出名称一致
)

message(STATUS "${PROJECT_NAME} 配置完成: ${PROJECT_NAME}")