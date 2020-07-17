import os, sys
import numpy as np
import ctypes

app_path = os.path.dirname(os.path.realpath('PythonPort.py'))
dll_path = os.path.join(app_path, 'CPPLibrary', 'CPPLibrary.dll')

CPPMathLibrary = ctypes.cdll.LoadLibrary(dll_path)

def GreatestCommonDivisor(num1 : int, num2 : int) -> int :
    return CPPMathLibrary.CPPMathLibrary_ManagedPort_GetGCD(num1, num2)