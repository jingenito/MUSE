import sys
import numpy
from CPPLibrary import SimultaneousDiophantine

alpha = 0.75
epsilon = 1.0 / 10
x = numpy.array([239.0 / 169.0, 265.0 / 153.0, 682.0 / 305.0, 590.0 / 223.0 ], 
                dtype = numpy.double)
sd = SimultaneousDiophantine.SameDivisor(x, alpha, epsilon)

print("Alpha: ", alpha)
print("Epsilon: ", epsilon)
print("X:")
print(x)
print()
print("LLL C:")
print(sd)

