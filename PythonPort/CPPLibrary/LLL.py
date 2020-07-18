from CPPLibrary import *

def GramSchmidtOrthogonalization(matrix: numpy.array) -> numpy.array :
    count = matrix.shape[0]
    indata = HelperFunctions.NumpyArrArrToDblPtrPtr(matrix)

    func = CPPMathLibrary.CPPMathLibrary_ManagedPort_GramSchmidt
    func.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_double)), ctypes.c_int32]
    func.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_double))
    result = func(indata, count)

    return HelperFunctions.DblPtrPtrToNumpyArrArr(result, count)