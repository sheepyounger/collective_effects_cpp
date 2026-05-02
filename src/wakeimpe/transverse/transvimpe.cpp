#include<fstream>
#include<algorithm>
#include<stdexcept>
#include<cmath>
#include<fftw3.h>
#include "utils/constants.hpp"
#include "utils/sequence.hpp"
#include "utils/integral.hpp"
#include "wakeimpe/transverse/transvimpe.hpp"
#include "wakeimpe/transverse/transvwake.hpp"

using namespace std;
using namespace constants;

// 前向声明
namespace transvwake {class TransverseWakefield;}

transvimpe::TransverseImpedance::TransverseImpedance(
    const string& filename, double twiss_beta, double scale
) : twiss_beta_(twiss_beta), scale_(scale){
    auto [freq, real_impe, imag_impe] = read_impedance_data(filename);
    init_parameters(move(freq), move(real_impe), move(imag_impe));
}

transvimpe::TransverseImpedance::TransverseImpedance(
    const transvwake::TransverseWakefield& wakefield
) : twiss_beta_(wakefield.twiss_beta_), scale_(wakefield.scale_)
{
    auto [freq, real_impe, imag_impe] = wakefield_to_impedance(wakefield);
    init_parameters(move(freq), move(real_impe), move(imag_impe));
}

tuple<vector<double>, vector<double>, vector<double>>
transvimpe::TransverseImpedance::read_impedance_data(const string& filename)
{
    ifstream file(filename);
    if (not file.is_open()){
        throw runtime_error("Cannot open impedance data file: " + filename);
    }

    // 根据阻抗数据文件获取正频率部分
    vector<double> pos_freq, pos_real, pos_imag;
    double f, pr, pi;

    while (file >> f >> pr >> pi)
    {
        pos_freq.push_back(f);
        pos_real.push_back(pr);
        pos_imag.push_back(pi);
    }

    if (pos_freq.empty()){
        throw runtime_error("Impedance data file is empty: " + filename);
    }

    // 开始构建负频率部分
    size_t n = pos_freq.size();

    // 创建全频率的数组
    vector<double> full_freq(2 * n - 1);
    vector<double> full_real(2 * n - 1);
    vector<double> full_imag(2 * n - 1);
    // 正频率
    for (size_t i = 0; i < n; ++i){
        full_freq[n - 1 + i] = pos_freq[i];
        full_real[n - 1 + i] = pos_real[i] * twiss_beta_ * scale_;
        full_imag[n - 1 + i] = pos_imag[i] * twiss_beta_ * scale_;
    }
    // 负频率
    for (size_t i = 1; i < n; ++i){
        full_freq[n - 1 - i] = - pos_freq[i];
        full_real[n - 1 - i] = - pos_real[i] * twiss_beta_ * scale_;
        full_imag[n - 1 - i] = pos_imag[i] * twiss_beta_ * scale_;
    }
    return {
        move(full_freq),
        move(full_real),
        move(full_imag)
    };
}

tuple<vector<double>, vector<double>, vector<double>>
transvimpe::TransverseImpedance::wakefield_to_impedance(const transvwake::TransverseWakefield& wakefield)
{
    // 获取尾场数据的坐标范围
    double z_min = *wakefield.z_min_;
    double z_max = *wakefield.z_max_;
    double z_step = *wakefield.z_step_;
    cout << "z_min: " << z_min << endl;
    cout << "z_max: " << z_max << endl;
    cout << "z_step: " << z_step << endl;

    // 准备纵向位置数组（等差数列）
    vector<double> z = arange(z_min, z_max, z_step);
    size_t nz = z.size();
    cout << "nz: " << nz << endl;

    // 获取尾场值
    vector<double> wake(nz);
    for (size_t i = 0; i < nz; ++i) {
        wake[i] = wakefield(z[i]);
    }
    cout << "max wake value: " << vec_max(wake) << endl;

    float dfreq = 1e6;         //  设置频率间隔 0.1MHz
    float max_freq = 50e9;    //  最大频率 500GHz
    size_t nfreq = static_cast<size_t>(2 * max_freq / dfreq) + 1;   //  频率点个数

    //  创建频率轴
    vector<double> vec_freq = linspace(-max_freq, max_freq, nfreq);
    //  初始化阻抗组数
    vector<complex<double>> impe(nfreq);
    for (size_t f=0; f<nfreq; ++f) {
        vector<complex<double>> integrand(nz);
        for (size_t i =0; i<nz; ++i) {
            double omega = 2 * pi * vec_freq[f];
            complex<double> kernel = exp(- 1i * omega * z[i] / speed_of_light);
            integrand[i] = kernel * wake[i] / speed_of_light;
        }
        impe[f] = simpson_3_8_dx_complex(integrand, z_step);
    }

    vector<double> real_impe, imag_impe;
    real_impe.reserve(impe.size());
    imag_impe.reserve(impe.size());

    // 分离实部和虚部
    for (const auto& z : impe) {
        real_impe.push_back(-z.imag());
        imag_impe.push_back(z.real());
    }
    return {vec_freq, real_impe, imag_impe};
}