cmake_minimum_required(VERSION 3.15)
project(collective_effects VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 可选：开启调试符号（便于开发）
set(CMAKE_VERBOSE_MAKEFILE OFF)

# 查找依赖
find_package(Boost REQUIRED COMPONENTS math)
find_package(Python REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 CONFIG REQUIRED)

# 添加子目录
add_subdirectory(src)
add_subdirectory(python/bindings)

# 可选：添加 Python 模块安装
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install" CACHE PATH "..." FORCE)
endif()