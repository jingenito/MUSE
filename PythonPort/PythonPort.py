import sys
from CPPLibrary import GreatestCommonDivisor

n1 = 0
t1 = input("Enter a number ")
if t1.isnumeric() :
	n1 = int(t1)
else :
	print("Not number")
	sys.exit()

n2 = 0
t2 = input("Enter a number ")
if t2.isnumeric() :
	n2 = int(t2)
else :
	print("Not Number")
	sys.exit()

gcd = GreatestCommonDivisor(n1, n2)
print("The GCD is ", gcd)