#include<fstream>
#include<algorithm>
#include<stdexcept>
#include<cmath>
#include<fftw3.h>
#include "utils/constants.hpp"
#include "utils/sequence.hpp"
#include "utils/integral.hpp"
#include "wakeimpe/transverse/transvwake.hpp"
#include "wakeimpe/transverse/transvimpe.hpp"
#include "wakeimpe/wakefield.hpp"

using namespace std;
using namespace constants;

// 前向声明
namespace transvimpe {class TransverseImpedance;}

transvwake::TransverseWakefield::TransverseWakefield(
    const string& filename, double twiss_beta, double scale
) : ::FileWakefield(filename), twiss_beta_(twiss_beta), scale_(scale)
{}

transvwake::TransverseWakefield::TransverseWakefield(
    const transvimpe::TransverseImpedance& impedance
) : twiss_beta_(impedance.twiss_beta_), scale_(impedance.scale_){
    auto [z, w] = impedance_to_wakefield(impedance);
    init_parameters(z, w);
}

//  eq.(2.72) A. W. Chao, Physics of Collective Beam Instabilities in High Energy Accelerators (Wiley, New York, 1993).
tuple<vector<double>, vector<double>>
transvwake::TransverseWakefield::impedance_to_wakefield(const transvimpe::TransverseImpedance& impedance)
{
    // 获取阻抗数据的频率范围
    double freq_max = *impedance.freq_max_;
    double freq_min = *impedance.freq_min_;
    double freq_step = *impedance.freq_step_;
    cout << "freq_max: " << freq_max << endl;
    cout << "freq_min: " << freq_min << endl;
    cout << "freq_step: " << freq_step << endl;

    // 准备频率数组（等差数列）
    vector<double> freq = arange(freq_min, freq_max, freq_step);
    size_t nf = freq.size();
    cout << "nf: " << nf << endl;

    // 获取阻抗值
    vector<complex<double>> impe(nf);
    for (size_t i=0; i < nf; ++i) {
        impe[i] = impedance(freq[i]);
    }
    cout << "max real value: " << vec_max(real(impe)) << endl;
    cout << "max imag value: " << vec_max(imag(impe)) << endl;

    //  设置尾场数据
    float dz = 1e-5;    //  设置纵向坐标间距
    float max_z = 0.5;  //  设置最大纵向坐标
    size_t nz = static_cast<size_t>(2 * max_z / dz) + 1;    //  纵向坐标点个数

    //  创建纵向坐标轴
    vector<double> vec_z = linspace(-max_z, max_z, nz);
    //  初始化尾场组数
    vector<double> wake(nz);
    for (size_t j=0; j < nz; ++j){
        vector<complex<double>> integrand(nf);
        for (size_t k=0; k<nf; ++k) {
            double omega = 2 * pi * freq[k] / speed_of_light;
            complex<double> kernel = - 1i * exp(1i * omega * vec_z[j]);
            integrand[k] = kernel * impe[k];
        }
        wake[j] = real(simpson_3_8_dx_complex(integrand, freq_step));
    }

    return {vec_z, wake};
}
