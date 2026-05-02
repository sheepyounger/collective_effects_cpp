#include<fstream>
#include<algorithm>
#include<stdexcept>
// #include "../../include/wakeimpe/wakefield.hpp"
#include "wakeimpe/wakefield.hpp"

using namespace std;
using namespace boost::math::interpolators;


FileWakefield::FileWakefield(const string& filename)
{
    //  z [m]; w [V/pC]
    auto [z, w] = read_wakefield_data(filename);
    init_parameters(z, w);
}

void FileWakefield::init_parameters(std::vector<double> z, std::vector<double> w)
{
    //  验证输入数据
    check_data(z, w);

    //  记录坐标范围的最大值和最小值
    z_min_.emplace(z.front());
    z_max_.emplace(z.back());
    z_step_.emplace((z.back() - z.front()) / (z.size() - 1));

    //  构造尾场插值器
    wake_interpolate_.emplace(makima<vector<double>>(move(z), move(w)));
}

void FileWakefield::check_data(const vector<double>& z, const vector<double>& w)
{
    if (z.size() != w.size()) {
        throw invalid_argument("Wakefield initialization error: z and w must have same size");
    }
    if (z.empty() or w.empty()) {
        throw invalid_argument("Wakefield initialization error: arrays cannot be empty");
    }
    if (not is_sorted(z.begin(), z.end())) {
        throw invalid_argument("Wakefield initialization error: z must be monotonic increasing");
    }
}

double FileWakefield::operator()(double z_query) const
{
    // 检查插值器是否初始化
    if (not wake_interpolate_.has_value())
    {
        throw runtime_error("Wakefield: interpolators not initialized");
    }
    // 执行插值
    if (z_query < z_min_ or z_query > z_max_)
    {
        return 0.0;     // 边界处理：超出频率范围返回 0
    }
    else
    {
        return (*wake_interpolate_)(z_query);   // 正常插值
    }
}

vector<double> FileWakefield::operator()(const vector<double>& z_queries) const
{
    // 检查插值器是否初始化
    if (not wake_interpolate_.has_value())
    {
        throw runtime_error("Wakefield: interpolators not initialized");
    }

    vector<double> wake;
    wake.reserve(z_queries.size());

    for (double zq : z_queries){
        if (zq < z_min_ or zq > z_max_)
        {
            wake.push_back(0.0);    // 边界处理：超出频率范围返回 0
        }
        else
        {
            wake.push_back((*wake_interpolate_)(zq));   // 正常插值
        }
    }
    return wake;
}

tuple<vector<double>, vector<double>>
FileWakefield::read_wakefield_data(const string& filename)
{
    ifstream file(filename);
    if (not file.is_open()){
        throw runtime_error("Cannot open wakefield data file: " + filename);
    }

    vector<double> z_array, w_array;
    double z, w;

    while (file >> z >> w)
    {
        z_array.push_back(z);
        w_array.push_back(w * 1e12);   // 单位转换 V/pC --> V/C
    }

    if (z_array.empty()){
        throw runtime_error("Wakefield data file is empty: " + filename);
    }

    return {z_array, w_array};
}

