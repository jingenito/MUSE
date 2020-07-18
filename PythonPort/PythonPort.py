import sys
import numpy
from CPPLibrary import LLL

alpha = 0.75
x = numpy.array([[-2, 7, 7, -5], 
                [3, -2, 6 , -1], 
                [2, -8, -9, -7],
                [8, -9, 6, -4]], 
                dtype = numpy.double)
gso = LLL.ReduceBasis_LLL(x, alpha)

print("Alpha: ", alpha)
print("X:")
print(x)
print()
print("LLL:")
print(gso)

