from CPPLibrary import *

def GramSchmidtOrthogonalization(matrix : numpy.array, count : int) -> numpy.array :
    func = CPPMathLibrary.CPPMathLibrary_ManagedPort_GramSchmidt
    func.argtypes = [numpy.ctypeslib.ndpointer(dtype=ctypes.c_double, ndim=2, shape=(count,count), flags="C_CONTIGUOUS"), ctypes.c_int]
    func.restype = numpy.ctypeslib.ndpointer(dtype=ctypes.c_double, ndim=2, shape=(count,count), flags="C_CONTIGUOUS")
    result = func(matrix, count)
    return result