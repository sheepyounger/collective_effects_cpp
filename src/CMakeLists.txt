# 收集各模块源文件
set(WAKEIMPE_SOURCES
    wakeimpe/wake_base.cpp
    # wakeimpe/internal_utils.cpp  # 等创建
)

# 创建 C++ 静态库（内部使用）
add_library(collective_effects_cpp STATIC ${WAKEIMPE_SOURCES})

# 设置头文件路径
target_include_directories(collective_effects_cpp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# 链接外部库
target_link_libraries(collective_effects_cpp
    PUBLIC
        Boost::math
    PRIVATE
        ${Python_LIBRARIES}
)

# 可选：设置库输出目录
set_target_properties(collective_effects_cpp
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)