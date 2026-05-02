"""
wakeimpe - Wakefield and Impedance module for collective effects analysis

Submodules:
    - transverse: DipoleImpedance, QuadrupoleImpedance, DipoleWakefield, QuadrupoleWakefield
    - longitudinal: MonopoleImpedance, MonopoleWakefield
"""

from . import transverse
from . import longitudinal

# from ._wakeimpe_cpp import (
#     MonopoleImpedance,
#     MonopoleWakefield,
#     DipoleImpedance,
#     QuadrupoleImpedance,
#     DipoleWakefield,
#     QuadrupoleWakefield,
#     __version__,
# )

# __cpp_available__ = True
# __all__ = [
#     "MonopoleImpedance",
#     "MonopoleWakefield",
#     "DipoleImpedance",
#     "QuadrupoleImpedance",
#     "DipoleWakefield",
#     "QuadrupoleWakefield",
# ]

# import sys
# import os

# __all__ = [
#     "MonopoleImpedance",
#     "MonopoleWakefield",
#     "DipoleImpedance",
#     "QuadrupoleImpedance",
#     "DipoleWakefield",
#     "QuadrupoleWakefield",
# ]

# __version__ = "0.0.1"

# # 尝试导入 C++ 扩展
# _cpp_available = False

# try:
#     from ._wakeimpe_cpp import (
#         MonopoleImpedance,
#         MonopoleWakefield,
#         DipoleImpedance,
#         QuadrupoleImpedance,
#         DipoleWakefield,
#         QuadrupoleWakefield,
#         __version__ as _cpp_version,
#     )
#     _cpp_available = True
#     __version__ = _cpp_version
#     print(f"[wakeimpe] Using C++ backend (version {__version__})")
# except ImportError as e:
#     # C++ 扩展不可用
#     print(f"[wakeimpe] C++ backend unavailable: {e}")
#     print("[wakeimpe] Falling back to Python backend (if available)")

#     try:
#         from ._core import (
#             MonopoleImpedance,
#             MonopoleWakefield,
#             DipoleImpedance,
#             QuadrupoleImpedance,
#             DipoleWakefield,
#             QuadrupoleWakefield,
#         )
#         _cpp_available = False
#     except ImportError:
#         raise ImportError(
#             "Neither C++ extension nor Python fallback available.\n"
#             "Please ensure collective-effects is properly installed.\n"
#             f"Original error: {e}"
#         )

# # 可选：导出 __cpp_available__ 供用户检查
# __cpp_available__ = _cpp_available