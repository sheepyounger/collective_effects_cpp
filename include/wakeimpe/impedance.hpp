#pragma once

#include<complex>
#include<vector>
#include<optional>
#include<boost/math/interpolators/makima.hpp>


class Impedance
{
private:
    //  频率边界，超出边界的频率点插值会返回0.
    double freq_min_;
    double freq_max_;
    //  使用makima插值算法，构造可调用的阻抗对象
    std::optional<boost::math::interpolators::makima<std::vector<double>>> real_interpolate_;
    std::optional<boost::math::interpolators::makima<std::vector<double>>> imag_interpolate_;
    static void check_data(const std::vector<double>& freq, const std::vector<double>& impe_real, const std::vector<double>& impe_imag);

public:
    Impedance(
        std::vector<double> freq,           //  Corresponding frequency of imepdance [Hz].
        std::vector<double> impe_real,      //  Real part of impedance (longitudinal imepdance [Ω]; transverse impedance [Ω/m]).
        std::vector<double> impe_imag       //  Imaginary part of impedance (longitudinal imepdance [Ω]; transverse impedance [Ω/m]).
    );
    ~Impedance() = default;

    // 使用makima插值算法，使得后续对阻抗的调用能与内部数据解耦
    // 对超出原始频率范围的插值点，返回结果为0
    [[nodiscard]] std::complex<double> operator()(double freq_query) const;
    [[nodiscard]] std::vector<std::complex<double>> operator()(const std::vector<double>& freq_queries) const ;

};


