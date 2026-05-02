# 收集源文件
set(WAKEIMPE_SOURCES
    wakeimpe/wakefield.cpp
    wakeimpe/impedance.cpp
    wakeimpe/longitudinal/longitimpe.cpp
    wakeimpe/longitudinal/longitwake.cpp
    wakeimpe/transverse/transvimpe.cpp
    wakeimpe/transverse/transvwake.cpp
    utils/integral.cpp
)

# 创建 C++ 静态库（内部使用）
add_library(collective_effects_wakeimpe STATIC ${WAKEIMPE_SOURCES})
set_target_properties(collective_effects_wakeimpe
    PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)


# 包含头文件目录
target_include_directories(collective_effects_wakeimpe
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        ${FFTW3_INCLUDE_DIRS}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# 链接外部库
target_link_libraries(collective_effects_wakeimpe
    PUBLIC
        Boost::headers      # ← 改成 Boost::headers
        ${FFTW3_LIBRARIES}
)

# 可选：设置库输出目录
set_target_properties(collective_effects_wakeimpe
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)