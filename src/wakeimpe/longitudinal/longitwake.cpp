#include<fstream>
#include<algorithm>
#include<stdexcept>
#include<cmath>
#include<fftw3.h>
// #include "../../include/utils/constants.hpp"
// #include "../../include/utils/sequence.hpp"
// #include "../../../include/wakeimpe/longitudinal/longitwake.hpp"
// #include "../../../include/wakeimpe/longitudinal/longitimpe.hpp"
#include "utils/constants.hpp"
#include "utils/sequence.hpp"
#include "wakeimpe/longitudinal/longitwake.hpp"
#include "wakeimpe/longitudinal/longitimpe.hpp"
#include "wakeimpe/wakefield.hpp"
#include<iostream>

using namespace std;
using namespace constants;

// 前向声明
namespace longitimpe {class LongitudinalImpedance;}

longitwake::LongitudinalWakefield::LongitudinalWakefield(
    const string& filename,double scale
) : ::FileWakefield(filename), scale_(scale){}

longitwake::LongitudinalWakefield::LongitudinalWakefield(
    const longitimpe::LongitudinalImpedance& impedance
) : scale_(impedance.scale_){
    auto [z, w] = impedance_to_wakefield(impedance);
    init_parameters(z, w);
}

//  eq.(2.72) A. W. Chao, Physics of Collective Beam Instabilities in High Energy Accelerators (Wiley, New York, 1993).
tuple<vector<double>, vector<double>>
longitwake::LongitudinalWakefield::impedance_to_wakefield(const longitimpe::LongitudinalImpedance& impedance)
{
    //  准备频率数组（等差数列）
    vector<double> freq = arange(
        *impedance.freq_min_,
        *impedance.freq_max_,
        *impedance.freq_step_
    );
    //  频率数组长度
    size_t n = static_cast<size_t>(
        llround((*impedance.freq_max_ - *impedance.freq_min_) / *impedance.freq_step_)
    ) + 1;
    //  准备FFT输入数组
    fftw_complex* in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);
    fftw_complex* out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);

    //  创建 FFTW 计划（逆变换，复数→复数）
    fftw_plan plan = fftw_plan_dft_1d(n, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);

    //  填充输入数据
    for (size_t i = 0; i < n; ++i) {
        in[i][0] = impedance.operator()(freq[i]).real();
        in[i][1] = impedance.operator()(freq[i]).imag();
    }

    //  执行逆傅里叶变换
    fftw_execute(plan);

    const double& dfreq = *impedance.freq_step_;
    double dz = 2 * pi * speed_of_light / (n * dfreq);  //  纵向位置步长
    double z0 = - (n - 1) / 2.0 * dz;                       //  起始位置
    double factor = 1.0 / 2.0 / pi * dfreq;

    vector<double> z(n), wake(n);
    for (size_t i=0; i<n; ++i){
        z[i] = z0 + i * dz;

        //  尾场数据应为实数，故丢弃虚部
        wake[i] = factor * out[i][0];
    }

    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    return {move(z), move(wake)};
}
