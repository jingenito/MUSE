from CPPLibrary import *

def GramSchmidtOrthogonalization(matrix: numpy.array) -> numpy.array :
    count = matrix.shape[0]
    indata = HelperFunctions.NumpyArrArrToDblPtrPtr(matrix)

    func = CPPMathLibrary.CPPMathLibrary_ManagedPort_GramSchmidt
    func.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_double)), ctypes.c_int32]
    func.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_double))
    result = func(indata, count)

    return HelperFunctions.DblPtrPtrToNumpyArrArr(result, count)

def ReduceBasis_LLL(matrix: numpy.array, alpha : numpy.double) -> numpy.array :
    count = matrix.shape[0]
    indata = HelperFunctions.NumpyArrArrToDblPtrPtr(matrix)

    func = CPPMathLibrary.CPPMathLibrary_ManagedPort_ReduceBasis_LLL
    func.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_double)), ctypes.c_int32, ctypes.c_double]
    func.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_double))
    result = func(indata, count, alpha)

    return HelperFunctions.DblPtrPtrToNumpyArrArr(result, count)

def ReduceBasis_LLL_C(matrix: numpy.array, alpha : numpy.double) -> numpy.array :
    count = matrix.shape[0]
    indata = HelperFunctions.NumpyArrArrToDblPtrPtr(matrix)

    func = CPPMathLibrary.CPPMathLibrary_ManagedPort_ReduceBasis_LLL_C
    func.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_double)), ctypes.c_int32, ctypes.c_double]
    func.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_double))
    result = func(indata, count, alpha)

    return HelperFunctions.DblPtrPtrToNumpyArrArr(result, count)