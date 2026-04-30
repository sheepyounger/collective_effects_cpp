#include<algorithm>
#include<stdexcept>
#include "../../include/wakeimpe/impedance.hpp"
// #include "wakeimpe/impedance.hpp"     // 未来程序整体编译用

using namespace std;
using namespace boost::math::interpolators;

Impedance::Impedance(vector<double> freq, vector<double> impe_real, vector<double> impe_imag)
    : freq_min_(freq.front())
    , freq_max_(freq.back())
{
    // 验证输入数据
    check_data(freq, impe_real, impe_imag);

    // 复制一份频率给虚部（因为 freq 会被实部移动）
    vector<double> freq_for_imag = freq;

    // 构造实部和虚部插值器
    real_interpolate_.emplace(makima<vector<double>>(move(freq), move(impe_real)));
    imag_interpolate_.emplace(makima<vector<double>>(move(freq_for_imag), move(impe_imag)));
}

void Impedance::check_data(const vector<double>& freq, const vector<double>& impe_real, const vector<double>& impe_imag)
{
    if (freq.size() != impe_real.size() or freq.size() != impe_imag.size()) {
        throw invalid_argument("Impedance initialization error: freq, real, imag must have same size");
    }
    if (freq.empty() or impe_imag.empty() or impe_real.empty()) {
        throw invalid_argument("Impedance initialization error: arrays cannot be empty");
    }
    if (not is_sorted(freq.begin(), freq.end())) {
        throw invalid_argument("Impedance initialization error: freq must be monotonic increasing");
    }
}


complex<double> Impedance::operator()(double freq_query) const
{
    // 检查插值器是否初始化
    if ((not real_interpolate_.has_value()) or (not imag_interpolate_.has_value()))
    {
        throw runtime_error("Impedance: interpolators not initialized");
    }

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

vector<complex<double>> Impedance::operator()(const vector<double>& freq_queries) const
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