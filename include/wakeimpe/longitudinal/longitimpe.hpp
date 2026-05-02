#pragma once
#include<string>
#include<tuple>
#include<vector>
#include "../wakefield.hpp"
#include "../impedance.hpp"
#include "wakeimpe/longitudinal/longitwake.hpp"

// 前向声明
namespace longitwake {class LongitudinalWakefield;}

namespace longitimpe {
    class LongitudinalImpedance : public ::FileImpedance
    {
        public:
            double scale_;
            //  从数据文件构造阻抗对象
            LongitudinalImpedance(const std::string& filename, double scale = 1.0);
            //  从尾场对象构造阻抗对象
            LongitudinalImpedance(const longitwake::LongitudinalWakefield& wakefield);
            ~LongitudinalImpedance() = default;

        protected:
            std::tuple<std::vector<double>, std::vector<double>, std::vector<double>>
            read_impedance_data(const std::string& filename) override;
            std::tuple<std::vector<double>, std::vector<double>, std::vector<double>>
            wakefield_to_impedance(const longitwake::LongitudinalWakefield& wakefield);
    };

    class MonopoleImpedance : public LongitudinalImpedance
    {
        public:
        using LongitudinalImpedance::LongitudinalImpedance;
        ~MonopoleImpedance() override = default;
    };

    //@ class DipoleImpedance

    //@ class QuadrupoleImpedance

    //@ class SextupoleImpedance
};

