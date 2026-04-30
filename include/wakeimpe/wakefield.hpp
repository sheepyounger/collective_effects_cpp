#pragma once


#include<vector>
#include<boost/math/interpolators/makima.hpp>


class Wakefield
{
private:
    double z_min_;  // 纵向坐标最小值
    double z_max_;  // 纵向坐标最大值
    //  使用修正akima算法插值，构造可调用的尾场对象
    boost::math::interpolators::makima<std::vector<double>> interpolate_;
    static boost::math::interpolators::makima<std::vector<double>> create_interpolator(std::vector<double> z, std::vector<double> w);
public:
    Wakefield(
        std::vector<double> z,   //  Corresponding longitudinal position of wakefield [m].
        std::vector<double> w    //  Amplitude of wakefield [V/pC].
    );
    ~Wakefield() = default;

    // 使用makima插值算法，使得后续对尾场的调用能与内部数据解耦
    // 对超出原始纵向坐标范围的插值点，返回结果为0
    [[nodiscard]] double operator()(double z_query) const;
    [[nodiscard]] std::vector<double> operator()(const std::vector<double>& z_queries) const ;
};

