#include<fstream>
#include<algorithm>
#include<stdexcept>
#include<cmath>
#include<fftw3.h>
// #include "../../include/utils/constants.hpp"
// #include "../../include/utils/sequence.hpp"
// #include "../../../include/wakeimpe/transverse/transvwake.hpp"
// #include "../../../include/wakeimpe/transverse/transvimpe.hpp"
#include "utils/constants.hpp"
#include "utils/sequence.hpp"
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
        //  尾场数据应为实数，故丢弃虚部.注意系数实际上带虚部。
        wake[i] = factor * out[i][1];
    }

    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    return {z, wake};
}