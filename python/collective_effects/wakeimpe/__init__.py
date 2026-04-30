"""
wakeimpe module - Beam-environment wakefield and impedance calculation
"""

# 尝试导入 C++ 扩展（如果可用）
try:
    from ._wakeimpe_cpp import WakeBase
    _use_cpp = True
except ImportError:
    _use_cpp = False
    # 回退到纯 Python 实现（如果有）
    from ._python_impl import WakeBase  # 未来实现

__all__ = ['WakeBase']
__version__ = '0.1.0'

# 从 _core 导入可以保持统一的 API