import sys
import numpy as np
from CPPLibrary import ContinuedFraction

n = np.sqrt(2)
print("Calculating Continued Fraction Expansion for ", n)

qs = ContinuedFraction.ContinuedFractionExpansion(n, 10)

print(qs)
