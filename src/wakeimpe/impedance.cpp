#include<algorithm>
#include<stdexcept>
#include "wakeimpe/impedance.hpp"

using namespace std;
using namespace boost::math::interpolators;


void FileImpedance::init_parameters(
    vector<double> freq,
    vector<double> real_impe,
    vector<double> imag_impe
){
    //  freq [Hz]; impe (longitudinal imepdance [Ω]; transverse impedance [Ω/m]

    // 验证输入数据
    check_data(freq, real_impe, imag_impe);

    // 复制一份频率给虚部（因为 freq 会被实部移动）
    vector<double> freq_for_imag = freq;

    // 记录频率最大最小值
    freq_max_.emplace(freq.back());
    freq_min_.emplace(freq.front());
    freq_step_.emplace((freq.back() - freq.front()) / (freq.size() - 1));

    // 构造实部和虚部插值器
    real_interpolate_.emplace(makima<vector<double>>(move(freq), move(real_impe)));
    imag_interpolate_.emplace(makima<vector<double>>(move(freq_for_imag), move(imag_impe)));
}

void FileImpedance::check_data(const vector<double>& freq, const vector<double>& real_impe, const vector<double>& imag_impe)
{
    if (freq.size() != real_impe.size() or freq.size() != imag_impe.size()) {
        throw invalid_argument("Impedance initialization error: freq, real, imag must have same size");
    }
    if (freq.empty() or imag_impe.empty() or real_impe.empty()) {
        throw invalid_argument("Impedance initialization error: arrays cannot be empty");
    }
    if (not is_sorted(freq.begin(), freq.end())) {
        throw invalid_argument("Impedance initialization error: freq must be monotonic increasing");
    }
}


complex<double> FileImpedance::operator()(double freq_query) const
{
    // 检查插值器是否初始化
    if ((not real_interpolate_.has_value()) or (not imag_interpolate_.has_value()))
    {
        throw runtime_error("Impedance: interpolators not initialized");
    }
    // 执行插值
    if (freq_query<freq_min_ or freq_query>freq_max_)
    {
        return complex<double>(0.0, 0.0);   // 边界处理：超出频率范围返回 0
    }
    else
    {
        return complex<double>(     // 正常插值
            (*real_interpolate_)(freq_query),
            (*imag_interpolate_)(freq_query)
        );
    }
}

vector<complex<double>> FileImpedance::operator()(const vector<double>& freq_queries) const
{
    // 检查插值器是否初始化
    if (!real_interpolate_.has_value() or !imag_interpolate_.has_value())
    {
        throw runtime_error("Impedance: interpolators not initialized");
    }

    vector<complex<double>> impe;
    impe.reserve(freq_queries.size());

    for (double freq : freq_queries){
        if (freq<freq_min_ or freq>freq_max_)
        {
            impe.emplace_back(0.0, 0.0);    // 边界处理：超出频率范围返回 0
        }
        else
        {
            impe.emplace_back(
                (*real_interpolate_)(freq), (*imag_interpolate_)(freq)
            );  // 正常插值
        }
    }
    return impe;
}

