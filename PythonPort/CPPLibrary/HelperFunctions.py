import numpy
import ctypes

def NumpyArrArrToDblPtrPtr(arr : numpy.array) -> ctypes.POINTER(ctypes.POINTER(ctypes.c_double)) :
    """ Convert a numpy array to a double ** for C/C++ """
    count = arr.shape[0]
    # Allocate array of double*
    indata = (ctypes.POINTER(ctypes.c_double) * count)()
    for i in range(count):
        # Allocate arrays of double
        indata[i] = (ctypes.c_double * count)()
        for j in range(count):
            indata[i][j] = arr[i][j]
    return indata

def DblPtrPtrToNumpyArrArr(arr : ctypes.POINTER(ctypes.POINTER(ctypes.c_double)), count : int) -> numpy.array :
    """ Convert a C/C++ double ** to a numpy.array """
    matrix = numpy.zeros((count,count), dtype=numpy.double)
    for i in range(count) :
        for j in range(count) :
            matrix[i][j] = arr[i][j]
    return matrix