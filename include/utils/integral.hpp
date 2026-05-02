#pragma once
#include <vector>
#include <complex>



// Simpson 3/8 积分
double simpson_3_8(const std::vector<double>& x, const std::vector<double>& y);

// 等距版本的 Simpson 3/8 积分
double simpson_3_8_dx(const std::vector<double>& y, double dx);

// 复数版本的 Simpson 3/8 积分
std::complex<double> simpson_3_8_complex(
    const std::vector<double>& x,
    const std::vector<std::complex<double>>& y);

// 复数版本的 Simpson 3/8 积分
std::complex<double> simpson_3_8_dx_complex(
    const std::vector<std::complex<double>>& y,
    double dx);