#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>

#include "wakeimpe/longitudinal/longitimpe.hpp"
#include "wakeimpe/longitudinal/longitwake.hpp"
#include "wakeimpe/transverse/transvimpe.hpp"
#include "wakeimpe/transverse/transvwake.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_wakeimpe_cpp, m) {
    m.doc() = "C++ implementation of wakeimpe module";
    m.attr("__version__") = "0.0.1";

    // ========== 纵向阻抗 ==========
    py::class_<longitimpe::MonopoleImpedance>(m, "MonopoleImpedance")
        .def(py::init<const std::string&, double>(),
             py::arg("filename"), py::arg("scale") = 1.0)
        .def(py::init<const longitwake::MonopoleWakefield&>(),
             py::arg("wakefield"))
        .def("__call__", py::overload_cast<double>(
                &longitimpe::MonopoleImpedance::operator(), py::const_),
             py::arg("freq_query"))
        .def("__call__", py::overload_cast<const std::vector<double>&>(
                &longitimpe::MonopoleImpedance::operator(), py::const_),
             py::arg("freq_queries"));

    // ========== 纵向尾场 ==========
    py::class_<longitwake::MonopoleWakefield>(m, "MonopoleWakefield")
        .def(py::init<const std::string&, double>(),
             py::arg("filename"), py::arg("scale") = 1.0)
        .def(py::init<const longitimpe::MonopoleImpedance&>(),
             py::arg("impedance"))
        .def("__call__", py::overload_cast<double>(
                &longitwake::MonopoleWakefield::operator(), py::const_),
             py::arg("z_query"))
        .def("__call__", py::overload_cast<const std::vector<double>&>(
                &longitwake::MonopoleWakefield::operator(), py::const_),
             py::arg("z_queries"));

    // ========== 横向偶极阻抗 ==========
    py::class_<transvimpe::DipoleImpedance>(m, "DipoleImpedance")
        .def(py::init<const std::string&, double, double>(),
             py::arg("filename"), py::arg("twiss_beta") = 1.0, py::arg("scale") = 1.0)
        .def(py::init<const transvwake::DipoleWakefield&>(),
             py::arg("wakefield"))
        .def("__call__", py::overload_cast<double>(
                &transvimpe::DipoleImpedance::operator(), py::const_),
             py::arg("freq_query"))
        .def("__call__", py::overload_cast<const std::vector<double>&>(
                &transvimpe::DipoleImpedance::operator(), py::const_),
             py::arg("freq_queries"));

    // ========== 横向四极阻抗 ==========
    py::class_<transvimpe::QuadrupoleImpedance>(m, "QuadrupoleImpedance")
        .def(py::init<const std::string&, double, double>(),
             py::arg("filename"), py::arg("twiss_beta") = 1.0, py::arg("scale") = 1.0)
        .def(py::init<const transvwake::QuadrupoleWakefield&>(),
             py::arg("wakefield"))
        .def("__call__", py::overload_cast<double>(
                &transvimpe::QuadrupoleImpedance::operator(), py::const_),
             py::arg("freq_query"))
        .def("__call__", py::overload_cast<const std::vector<double>&>(
                &transvimpe::QuadrupoleImpedance::operator(), py::const_),
             py::arg("freq_queries"));

    // ========== 横向偶极尾场 ==========
    py::class_<transvwake::DipoleWakefield>(m, "DipoleWakefield")
        .def(py::init<const std::string&, double, double>(),
             py::arg("filename"), py::arg("twiss_beta") = 1.0, py::arg("scale") = 1.0)
        .def(py::init<const transvimpe::DipoleImpedance&>(),
             py::arg("wakefield"))
        .def("__call__", py::overload_cast<double>(
                &transvwake::DipoleWakefield::operator(), py::const_),
             py::arg("z_query"))
        .def("__call__", py::overload_cast<const std::vector<double>&>(
                &transvwake::DipoleWakefield::operator(), py::const_),
             py::arg("z_queries"));

    // ========== 横向四极尾场 ==========
    py::class_<transvwake::QuadrupoleWakefield>(m, "QuadrupoleWakefield")
        .def(py::init<const std::string&, double, double>(),
             py::arg("filename"), py::arg("twiss_beta") = 1.0, py::arg("scale") = 1.0)
        .def(py::init<const transvimpe::QuadrupoleImpedance&>(),
             py::arg("wakefield"))
        .def("__call__", py::overload_cast<double>(
                &transvwake::QuadrupoleWakefield::operator(), py::const_),
             py::arg("z_query"))
        .def("__call__", py::overload_cast<const std::vector<double>&>(
                &transvwake::QuadrupoleWakefield::operator(), py::const_),
             py::arg("z_queries"));
}