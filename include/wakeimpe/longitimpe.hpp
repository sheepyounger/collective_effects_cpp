#pragma

#include "impedance.hpp"



class FileBasedImpedance : public Impedance
{
private:
    /* data */
public:
    /**
     * @brief 从数据文件构造阻抗模型
     * @param freq 频率数组 [Hz]
     * @param real_part 阻抗实部
     * @param imag_part 阻抗虚部
     */
    FileBasedImpedance(/* args */);
    ~FileBasedImpedance();
};

FileBasedImpedance::FileBasedImpedance(/* args */)
{
}

FileBasedImpedance::~FileBasedImpedance()
{
}
