from CPPLibrary import *

def ContinuedFractionExpansion(gamma : np.double, count : int) -> np.array :
    func = CPPMathLibrary.CPPMathLibrary_ManagedPort_ContinuedFraction
    func.argtypes = [ctypes.c_double, ctypes.c_int]
    func.restype = np.ctypeslib.ndpointer(dtype=ctypes.c_int, shape=(count,), flags="C_CONTIGUOUS")
    result = func(gamma, count)
    return np.array(result)
