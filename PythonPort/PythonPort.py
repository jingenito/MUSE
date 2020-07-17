import sys
import numpy
from CPPLibrary import ContinuedFraction

n = numpy.sqrt(2)
print("Calculating Continued Fraction Expansion for ", n)

count = 10
qs = ContinuedFraction.ContinuedFractionExpansion(n, count)
print(qs)
print()
ContinuedFraction.PrintConvergents(qs, count)

