from CPPLibrary import *

def SameDivisor(reals : numpy.array, alpha : numpy.double, epsilon : numpy.double) -> numpy.array :
    count = reals.shape[0]
    func = CPPMathLibrary.CPPMathLibrary_ManagedPort_Simultaneous_SameDivisor
    func.argtypes = [numpy.ctypeslib.ndpointer(dtype=ctypes.c_double, shape=(count,), flags="C_CONTIGUOUS"), ctypes.c_int, ctypes.c_double, ctypes.c_double]
    func.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_int))
    result = func(reals, count, alpha, epsilon)

    return HelperFunctions.IntPtrPtrToNumpyArrArr(result, count)