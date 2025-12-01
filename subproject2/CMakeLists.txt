#设置 PROJECT_NAME 变量为 subproject2
project(subproject2)

# 查找源文件
file(GLOB SOURCES "src/*.cpp")

# 创建可执行文件 - 会自动输出到根目录的 bin
add_executable(${PROJECT_NAME} ${SOURCES})

# 添加头文件包含路径
target_include_directories(${PROJECT_NAME}
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${SPDLOG_INCLUDE_DIR}
    PUBLIC
        ${COMMON_INCLUDE_DIR}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_COMPILED_LIB)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SPDLOG_LIBRARY})

# 可选：设置可执行文件属性
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "subproject2"  # 确保输出名称一致
)

# 可选：检查库文件是否存在
if(NOT EXISTS ${SPDLOG_LIBRARY})
    message(WARNING "spdlog library not found at: ${SPDLOG_LIBRARY}")
endif()

message(STATUS "${PROJECT_NAME} 配置完成: ${PROJECT_NAME}")