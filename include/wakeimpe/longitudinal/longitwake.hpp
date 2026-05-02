#pragma once
#include<string>
#include<tuple>
#include<vector>
#include "../wakefield.hpp"
#include "../impedance.hpp"
#include "longitimpe.hpp"

// 前向声明
namespace longitimpe {class LongitudinalImpedance;}

//  阻抗定义：实部偶函数，虚部奇函数
namespace longitwake {
    class LongitudinalWakefield : public FileWakefield
    {
        public:
            double scale_;
            LongitudinalWakefield(const std::string& filename, double scale = 1.0);
            LongitudinalWakefield(const longitimpe::LongitudinalImpedance& impedance);
            ~LongitudinalWakefield() = default;

        protected:
            std::tuple<std::vector<double>, std::vector<double>>
            impedance_to_wakefield(const longitimpe::LongitudinalImpedance& impedance);
    };

    class MonopoleWakefield : public LongitudinalWakefield
    {
        public:
        using LongitudinalWakefield::LongitudinalWakefield;
        ~MonopoleWakefield() = default;
    };

    //@ class DipoleWakefield

    //@ class QuadrupoleWakefield

    //@ class SextupoleWakefield
}