import sys
import numpy
from CPPLibrary import LLL

x = numpy.array([[3.0, -1.0, 5.0], 
                [-5.0, 2.0, -1.0], 
                [-3.0, 9.0, 2.0]], 
                dtype = numpy.double)
gso = LLL.GramSchmidtOrthogonalization(x)

print("X:")
print(x)
print()
print("GSO:")
print(gso)

