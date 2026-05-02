#pragma once
#include<string>
#include<tuple>
#include<vector>
#include "../wakefield.hpp"
#include "../impedance.hpp"
#include "transvimpe.hpp"

// 前向声明
namespace transvimpe {class TransverseImpedance;}

//  阻抗定义：实部奇函数，虚部偶函数
namespace transvwake {
    class TransverseWakefield : public FileWakefield
    {
        public:
        double twiss_beta_;
        double scale_;
        //  从数据文件构造尾场对象
        TransverseWakefield(const std::string& filename,double twiss_beta = 1.0,double scale = 1.0);
        //  从阻抗对象构造尾场对象
        TransverseWakefield(const transvimpe::TransverseImpedance& impedance);
        ~TransverseWakefield() = default;

        protected:
        std::tuple<std::vector<double>, std::vector<double>>
        impedance_to_wakefield(const transvimpe::TransverseImpedance& impedance);
    };

    //@ class MonopoleWakefield

    class DipoleWakefield : public TransverseWakefield
    {
        public:
        using TransverseWakefield::TransverseWakefield;
        ~DipoleWakefield() override = default;
    };

    class QuadrupoleWakefield : public TransverseWakefield
    {
        public:
        using TransverseWakefield::TransverseWakefield;
        ~QuadrupoleWakefield() override = default;
    };

    //@ class SextupoleWakefield

}