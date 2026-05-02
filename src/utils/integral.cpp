#include <vector>
#include <complex>
#include "utils/integral.hpp"

using namespace std;

// Simpson 3/8 积分
double simpson_3_8(const vector<double>& x, const vector<double>& y) {
    if (x.size() != y.size() || x.size() < 4) {
        return 0.0;
    }

    int n = x.size() - 1;
    double sum = 0.0;

    for (int i = 0; i <= n - 3; i += 3) {
        double h = x[i+3] - x[i];
        sum += 3.0 * h / 8.0 * (y[i] + 3*y[i+1] + 3*y[i+2] + y[i+3]);
    }

    // 处理剩余点
    int remainder = n % 3;
    if (remainder == 1) {
        // 梯形法则
        double h = x[n] - x[n-1];
        sum += (y[n-1] + y[n]) * h / 2.0;
    } else if (remainder == 2) {
        // Simpson 1/3
        double h = x[n] - x[n-2];
        sum += h / 6.0 * (y[n-2] + 4*y[n-1] + y[n]);
    }

    return sum;
}

// 等距版本的 Simpson 3/8 积分
double simpson_3_8_dx(const vector<double>& y, double dx) {
    int n = y.size() - 1;  // 区间数

    if (n < 3) {
        // 如果少于3个区间，使用梯形法则
        if (n == 1) return (y[0] + y[1]) * dx / 2.0;
        if (n == 2) return (y[0] + 4*y[1] + y[2]) * dx / 3.0;  // Simpson 1/3
        return 0.0;
    }

    double sum = 0.0;

    // 每3个区间一组，使用 Simpson 3/8 法则
    for (int i = 0; i <= n - 3; i += 3) {
        sum += 3.0 * dx / 8.0 * (y[i] + 3*y[i+1] + 3*y[i+2] + y[i+3]);
    }

    // 处理剩余的区间（如果 n 不是 3 的倍数）
    int remainder = n % 3;
    if (remainder == 1) {
        // 剩下1个区间，用梯形法则
        sum += (y[n-1] + y[n]) * dx / 2.0;
    } else if (remainder == 2) {
        // 剩下2个区间，用 Simpson 1/3
        sum += (y[n-2] + 4*y[n-1] + y[n]) * dx / 3.0;
    }

    return sum;
}

// 复数版本的 Simpson 3/8 积分
complex<double> simpson_3_8_complex(const vector<double>& x,const vector<complex<double>>& y) {

    if (x.size() != y.size() || x.size() < 4) {
        return complex<double>(0.0, 0.0);
    }

    int n = x.size() - 1;
    complex<double> sum(0.0, 0.0);

    // 每3个区间一组，使用 Simpson 3/8 法则
    for (int i = 0; i <= n - 3; i += 3) {
        double h = x[i+3] - x[i];
        complex<double> term = y[i] + 3.0*y[i+1] + 3.0*y[i+2] + y[i+3];
        sum += (3.0 * h / 8.0) * term;
    }

    // 处理剩余的区间
    int remainder = n % 3;
    if (remainder == 1) {
        // 剩下1个区间，梯形法则
        double h = x[n] - x[n-1];
        sum += (h / 2.0) * (y[n-1] + y[n]);
    } else if (remainder == 2) {
        // 剩下2个区间，Simpson 1/3
        double h = x[n] - x[n-2];
        sum += (h / 6.0) * (y[n-2] + 4.0*y[n-1] + y[n]);
    }

    return sum;
}

// 等距+复数版本的 Simpson 3/8 积分
complex<double> simpson_3_8_dx_complex(const vector<complex<double>>& y, double dx) {

    if (y.size() < 4) {
        return complex<double>(0.0, 0.0);
    }

    int n = y.size() - 1;
    complex<double> sum(0.0, 0.0);

    // 每3个点一组
    for (int i = 0; i <= n - 3; i += 3) {
        sum += 3.0 * dx / 8.0 * (y[i] + 3.0*y[i+1] + 3.0*y[i+2] + y[i+3]);
    }

    // 处理剩余点
    int remainder = n % 3;
    if (remainder == 1) {
        // 梯形法则
        sum += dx / 2.0 * (y[n-1] + y[n]);
    } else if (remainder == 2) {
        // Simpson 1/3
        sum += dx / 3.0 * (y[n-2] + 4.0*y[n-1] + y[n]);
    }

    return sum;
}