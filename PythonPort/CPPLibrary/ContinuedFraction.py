from CPPLibrary import *

def ContinuedFractionExpansion(gamma : numpy.double, count : int) -> numpy.array :
    func = CPPMathLibrary.CPPMathLibrary_ManagedPort_ContinuedFraction
    func.argtypes = [ctypes.c_double, ctypes.c_int]
    func.restype = numpy.ctypeslib.ndpointer(dtype=ctypes.c_int, shape=(count,), flags="C_CONTIGUOUS")
    result = func(gamma, count)
    return result

def PrintConvergents(qs : numpy.array, count : int) :
    func = CPPMathLibrary.CPPMathLibrary_ManagedPort_PrintConvergents
    func.argtypes = [numpy.ctypeslib.ndpointer(dtype=ctypes.c_int, shape=(count,), flags="C_CONTIGUOUS"), ctypes.c_int]
    func.restype = None
    func(qs, count)