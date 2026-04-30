#include<algorithm>
#include<stdexcept>
#include "../../include/wakeimpe/wakefield.hpp"
// #include "wakeimpe/wakefield.hpp"     // 未来程序整体编译用

using namespace std;
using namespace boost::math::interpolators;

Wakefield::Wakefield(vector<double> z, vector<double> w)
    : z_min_(z.front())
    , z_max_(z.back())
    , interpolate_(create_interpolator(z, w))
{
}

makima<vector<double>> Wakefield::create_interpolator(vector<double> z, vector<double> w){
    // 验证输入数据
    if (z.size() != w.size()){
        throw invalid_argument("Wakefield initialization error: z and w must have same size");
    }
    if (z.empty() or w.empty()){
        throw invalid_argument("Wakefield initialization error: z and w cannot be empty");
    }
    if (not is_sorted(z.begin(), z.end())){
        throw invalid_argument("Wakefield initialization error: z must be monotonic increasing");
    }
    // 构造插值器
    return makima<vector<double>>(move(z), move(w));
}

double Wakefield::operator()(double z_query) const {
    if (z_query<z_min_ or z_query>z_max_)
    {
        return 0.0;     // 边界处理：超出纵向坐标范围返回 0
    }
    else
    {
        return interpolate_(z_query);   // 正常插值
    }
}

vector<double> Wakefield::operator()(const vector<double>& z_queries) const {
    vector<double> wake;
    wake.reserve(z_queries.size());
    for (double zq : z_queries){
        if (zq<z_min_ or zq>z_max_)
        {
            wake.push_back(0.0);    // 边界处理：超出纵向坐标范围返回 0
        }
        else
        {
            wake.push_back(interpolate_(zq));   // 正常插值
        }
    }
    return wake;
}

