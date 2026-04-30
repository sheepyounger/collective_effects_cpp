#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "collective_effects/wakeimpe/wake_base.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_wakeimpe_cpp, m) {
    m.doc() = "C++ implementation of wakeimpe module";

    py::class_<collective_effects::wakeimpe::WakeBase>(m, "WakeBase")
        .def(py::init<std::vector<double>, std::vector<double>>(),
             py::arg("z"), py::arg("w"),
             "Construct wake function from z and w arrays")
        .def("__call__",
             py::overload_cast<double>(&collective_effects::wakeimpe::WakeBase::operator(), py::const_),
             py::arg("z_query"),
             "Interpolate wake value at single point")
        .def("__call__",
             py::overload_cast<const std::vector<double>&>(&collective_effects::wakeimpe::WakeBase::operator(), py::const_),
             py::arg("z_queries"),
             "Interpolate wake values at multiple points");
}