import sys
import numpy as np
from CPPLibrary import ContinuedFraction

n = np.sqrt(2)
print("Calculating Continued Fraction Expansion for ", n)

count = 10
qs = ContinuedFraction.ContinuedFractionExpansion(n, count)
print(qs)
print()
ContinuedFraction.PrintConvergents(qs, count)

