#pragma once
#include <vector>
#include <stdexcept>


// Python风格的linspace函数
inline std::vector<double> linspace(const double& start, const double& stop,
                                   const size_t& num = 50, bool endpoint = true) {
    if (num == 0) {
        return {};
    }
    if (num == 1) {
        return {start};
    }

    std::vector<double> result(num);
    double step = endpoint ? (stop - start) / (num - 1)
                           : (stop - start) / num;

    for (size_t i = 0; i < num; ++i) {
        result[i] = start + i * step;
    }

    return result;
}

// Python风格的arange函数
inline std::vector<double> arange(const double& start, const double& stop,
                                 const double& step = 1.0) {
    if (step == 0.0) {
        throw std::invalid_argument("步长不能为0");
    }

    size_t n = static_cast<size_t>((stop - start) / step);
    std::vector<double> result;
    result.reserve(n);

    for (double val = start; val < stop; val += step) {
        result.push_back(val);
    }

    return result;
}

inline double vec_max(std::vector<double> vec){
    double max_val = 0.0;
    auto max_iter = std::max_element(vec.begin(), vec.end());
    if (max_iter != vec.end()) {
        max_val = *max_iter;
        // max_val 现在是最大值
    }
    return max_val;
}