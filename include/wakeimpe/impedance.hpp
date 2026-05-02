#pragma once

#include<complex>
#include<vector>
#include<optional>
#include<string>
#include<tuple>
#include<boost/math/interpolators/makima.hpp>
#include "wakefield.hpp"

// 前向声明
class FileWakefield;

// ============================================================================
// 抽象基类 Impedance
// ============================================================================
class Impedance
{
    public:
    virtual ~Impedance() = default;

    // 禁止拷贝（阻抗数据不需要被复制）
    Impedance(const Impedance&) = delete;
    Impedance& operator=(const Impedance&) = delete;
    // 允许移动（高效转移所有权）
    Impedance(Impedance&&) = default;
    Impedance& operator=(Impedance&&) = default;

    /**
     * @brief 单点阻抗查询
     * @param freq_query 查询频率 [Hz]
     * @return 复数阻抗值
     *
     * 超出原始频率范围时，具体实现应返回 0
     */
    [[nodiscard]] virtual std::complex<double> operator()(double freq_query) const = 0;

    /**
     * @brief 批量阻抗查询
     * @param freq_queries 查询频率数组 [Hz]
     * @return 复数阻抗值数组，与输入顺序对应
     *
     * 超出原始频率范围时，具体实现应返回 0
     */
    [[nodiscard]] virtual std::vector<std::complex<double>>
    operator()(const std::vector<double>& freq_queries) const = 0;

    protected:
    // 允许派生类的构造
    Impedance() = default;
};


// ============================================================================
// 基于数据文件的阻抗 FileImpedance
// ============================================================================
class FileImpedance : public Impedance
{
    public:
        //  频率边界，超出边界的频率点插值会返回0.
        std::optional<double> freq_min_;
        std::optional<double> freq_max_;
        std::optional<double> freq_step_;
    protected:
        FileImpedance() = default;
    public:
        ~FileImpedance() override = default;

        // 使用makima插值算法，使得后续对阻抗模型的调用能与内部数据解耦
        // 对超出原始频率范围的插值点，返回结果为0
        [[nodiscard]] std::complex<double> operator()(double freq_query) const override;
        [[nodiscard]] std::vector<std::complex<double>>
        operator()(const std::vector<double>& freq_queries) const override;

    protected:
        void init_parameters(
            std::vector<double> freq,
            std::vector<double> real_impe,
            std::vector<double> imag_impe
        );
        virtual std::tuple<std::vector<double>, std::vector<double>, std::vector<double>>
        read_impedance_data(const std::string& filename) = 0;
        // 数据验证
        static void check_data(
            const std::vector<double>& freq,
            const std::vector<double>& real_impe,
            const std::vector<double>& imag_impe
        );
        //  使用makima插值算法，构造可调用的阻抗对象
        std::optional<boost::math::interpolators::makima<std::vector<double>>> real_interpolate_;
        std::optional<boost::math::interpolators::makima<std::vector<double>>> imag_interpolate_;

};


// ============================================================================
// 基于数学公式的阻抗 FormulaImpedance
// ============================================================================
class FormulaImpedance : public Impedance
{
    public:
    FormulaImpedance() = default;
    ~FormulaImpedance() override = default;

    // 利用阻抗模型公式，计算不同频率对应的阻抗
    // 支持任意大小的频率
    [[nodiscard]] virtual std::complex<double> operator()(double freq_query) const override = 0;
    [[nodiscard]] virtual std::vector<std::complex<double>>
    operator()(const std::vector<double>& freq_queries) const override = 0;
};