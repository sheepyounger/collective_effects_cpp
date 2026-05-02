#pragma once
#include<string>
#include<tuple>
#include<vector>
#include "../wakefield.hpp"
#include "../impedance.hpp"
#include "transvwake.hpp"

// 前向声明
namespace transvwake {class TransverseWakefield;}

namespace transvimpe {
    class TransverseImpedance : public ::FileImpedance
    {
        public:
        double twiss_beta_;
        double scale_;
        //  从数据文件构造阻抗对象
        TransverseImpedance(const std::string& filename, double twiss_beta = 1.0, double scale = 1.0);
        //  从尾场对象构造阻抗对象
        TransverseImpedance(const transvwake::TransverseWakefield& wakefield);
        ~TransverseImpedance() = default;

        protected:
        std::tuple<std::vector<double>, std::vector<double>, std::vector<double>>
        read_impedance_data(const std::string& filename) override;
        std::tuple<std::vector<double>, std::vector<double>, std::vector<double>>
        wakefield_to_impedance(const transvwake::TransverseWakefield& wakefield);
    };

    //@ class MonopoleImpedance

    class DipoleImpedance : public TransverseImpedance
    {
    public:
        using TransverseImpedance::TransverseImpedance;
        ~DipoleImpedance() override = default;
    };

    class QuadrupoleImpedance : public TransverseImpedance
    {
    public:
        using TransverseImpedance::TransverseImpedance;
        ~QuadrupoleImpedance() override = default;
    };

    //@ class SextupoleImpedance
};