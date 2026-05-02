#pragma once

#include<tuple>
#include<vector>
#include<optional>
#include<string>
#include<complex>
#include<boost/math/interpolators/makima.hpp>
#include "impedance.hpp"

// 前向声明
class FileImpedance;

// ============================================================================
// 抽象基类 Wakefield
// ============================================================================
class Wakefield
{
    public:
    virtual ~Wakefield() = default;

    // 禁止拷贝（尾场数据不需要被复制）
    Wakefield(const Wakefield&) = delete;
    Wakefield& operator=(const Wakefield&) = delete;
    // 允许移动（高效转移所有权
    Wakefield(Wakefield&&) = default;
    Wakefield& operator=(Wakefield&&) = default;

    // 单点尾场查询
    [[nodiscard]] virtual double operator()(double z_query) const = 0;
    // 批量尾场查询
    [[nodiscard]] virtual std::vector<double> operator()(const std::vector<double>& z_queries) const = 0;

    protected:
    // 允许派生类的构造
    Wakefield() = default;
};


// ============================================================================
// 基于数据文件的尾场 FileWakefield
// ============================================================================
class FileWakefield : public Wakefield
{
    public:
        //  坐标边界，超出边界的坐标点插值会返回0.
        std::optional<double> z_min_;
        std::optional<double> z_max_;
        std::optional<double> z_step_;
        //  从数据文件构造尾场对象
        explicit FileWakefield(const std::string& filename);
    protected:
        FileWakefield() = default;
    public:
        ~FileWakefield() override = default;

        // 使用makima插值算法，使得后续对尾场模型的调用能与内部数据解耦
        // 对超出原始坐标范围的插值点，返回结果为0
        [[nodiscard]] double operator()(double z_query) const override;
        [[nodiscard]] std::vector<double>
        operator()(const std::vector<double>& z_queries) const override;

    protected:
        void init_parameters(std::vector<double> z, std::vector<double> w);
        std::tuple<std::vector<double>, std::vector<double>>
        read_wakefield_data(const std::string& filename);
        // 数据验证
        static void check_data(
            const std::vector<double>& z,
            const std::vector<double>& w
        );
        //  使用makima插值算法，构造可调用的尾场对象.
        std::optional<boost::math::interpolators::makima<std::vector<double>>> wake_interpolate_;


};


// ============================================================================
// 基于数学公式的尾场 FormulaWakefield
// ============================================================================
class FormulaWakefield : public Wakefield
{
    public:
    FormulaWakefield() = default;
    ~FormulaWakefield() override = default;

    //  利用尾场模型公式，计算不同纵向坐标对应的阻抗
    //  支持任意大小的纵向坐标（过大将非物理）
    [[nodiscard]] virtual double operator()(double z_query) const override = 0;
    [[nodiscard]] virtual std::vector<double>
    operator()(const std::vector<double>& z_queries) const override = 0;
};

