#pragma once
#include <vector>
#include <stdexcept>
#include <complex>
#include <algorithm>
#include <iterator>

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

//  获取一个 vector<double>对象的最大值
inline double vec_max(const std::vector<double>& vec){
    double max_val = 0.0;
    auto max_iter = std::max_element(vec.begin(), vec.end());
    if (max_iter != vec.end()) {
        max_val = *max_iter;
        // max_val 现在是最大值
    }
    return max_val;
}

//  获取一个 vector<complex<double>> 对象的实部
inline std::vector<double> real(const std::vector<std::complex<double>>& complex_vec) {
    //  使用STL算法
    std::vector<double> real_parts;
    real_parts.reserve(complex_vec.size());

    std::transform(complex_vec.begin(), complex_vec.end(),
                   std::back_inserter(real_parts),
                   [](const std::complex<double>& z) {
                       return z.real();
                   });

    return real_parts;
}

//  获取一个 vector<complex<double>> 对象的虚部
inline std::vector<double> imag(const std::vector<std::complex<double>>& complex_vec) {
    //  使用STL算法
    std::vector<double> imag_parts;
    imag_parts.reserve(complex_vec.size());

    std::transform(complex_vec.begin(), complex_vec.end(),
                   std::back_inserter(imag_parts),
                   [](const std::complex<double>& z) {
                       return z.imag();
                   });

    return imag_parts;
}