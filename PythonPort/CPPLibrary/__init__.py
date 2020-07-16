from ctypes import cdll
CPPMathLibrary = cdll.LoadLibrary('c:/users/jinge/source/muse/cpplibrary/debug/cpplibrary.dll')

def GreatestCommonDivisor(num1 : int, num2 : int) -> int :
    return CPPMathLibrary.CPPMathLibrary_ManagedPort_GetGCD(num1, num2)