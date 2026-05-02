cmake_minimum_required(VERSION 3.15)
project(collective_effects VERSION 0.0.1 LANGUAGES CXX)

# C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 全局启用位置无关代码（-fPIC）
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 构建类型（默认 Release）
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(pybind11_DIR "/home/panliwei/.pyenv/versions/3.14.0/lib/python3.14/site-packages/pybind11/share/cmake/pybind11")

# 查找依赖
find_package(Boost REQUIRED COMPONENTS headers)
find_package(Python REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 CONFIG REQUIRED)

# 查找 FFTW3
find_path(FFTW3_INCLUDE_DIR NAMES fftw3.h PATH_SUFFIXES include)
find_library(FFTW3_LIBRARY NAMES fftw3)

# 手动设置 FFTW3（不通过 find_package）
set(FFTW3_INCLUDE_DIRS "/usr/include")
set(FFTW3_LIBRARIES "fftw3")  # 或者用绝对路径 -lfftw3

if(NOT FFTW3_INCLUDE_DIR OR NOT FFTW3_LIBRARY)
    message(FATAL_ERROR "FFTW3 not found. Please install libfftw3-dev")
endif()

set(FFTW3_INCLUDE_DIRS ${FFTW3_INCLUDE_DIR})
set(FFTW3_LIBRARIES ${FFTW3_LIBRARY})

# 包含目录
set(INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# 添加子目录
add_subdirectory(src)
add_subdirectory(python/bindings)

# 安装规则
install(DIRECTORY include/ DESTINATION include)