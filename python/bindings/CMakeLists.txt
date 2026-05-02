# 查找 pybind11
find_package(pybind11 REQUIRED)

# 创建 Python 模块
pybind11_add_module(_wakeimpe_cpp
    wakeimpe_bindings.cpp
)

# 链接 C++ 核心库
target_link_libraries(_wakeimpe_cpp
    PRIVATE
        collective_effects_wakeimpe
        pybind11::module
)

# 设置模块输出目录（直接放到 Python 包中）
set_target_properties(_wakeimpe_cpp
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY
    "${CMAKE_SOURCE_DIR}/python/collective_effects/wakeimpe"
)

# Windows 下需要特殊处理
if(WIN32)
    set_target_properties(_wakeimpe_cpp
        PROPERTIES
        SUFFIX ".pyd"
    )
endif()

# 可选：复制 __init__.py 等文件（确保在 build 目录中也存在）
add_custom_command(TARGET _wakeimpe_cpp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/python/collective_effects/wakeimpe/__init__.py
        ${CMAKE_SOURCE_DIR}/python/collective_effects/wakeimpe/__init__.py
    COMMENT "Ensuring __init__.py exists"
)