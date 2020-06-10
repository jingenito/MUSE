#pragma once

#include <iostream>

//definition can be found in algorithms.cpp
extern size_t greatestCommonDivisor(size_t, size_t);

class RationalNumber {
private:
	long _numerator, _divisor;

public:
	RationalNumber() {
		this->_numerator = 0;
		this->_divisor = 0;
	}

	RationalNumber(long n) {
		this->_numerator = n;
		this->_divisor = 1;
	}

	RationalNumber(long n, long d) {
		this->_numerator = n;
		this->_divisor = d;
	}

	RationalNumber(const RationalNumber& r) {
		this->_numerator = r._numerator;
		this->_divisor = r._divisor;
	}

	inline long GetNumerator() { return _numerator; }
	inline void SetNumerator(long num) { _numerator = num; }

	inline long GetDivisor() { return _divisor; }
	inline void SetDivisor(long num) { _divisor = num; }

	//Returns the value, but will default to 0.0 if NaN so check inputs carefully.
	inline double GetValue() {
		if (!this->isRealNumber()) { return 0.0; }
		return static_cast<double>(this->_numerator) / this->_divisor;
	}

	inline bool isRealNumber() { return _divisor != 0; }

	//Simplifies the current instance completely.
	inline void Simplify() {
		//this method should not be called on any of the operator overloads so the user
		//has complete control over efficiency
		size_t gcd = greatestCommonDivisor(abs(this->_numerator), abs(this->_divisor));
		this->_numerator = this->_numerator / gcd;
		this->_divisor = this->_divisor / gcd;
	}

	//Changes the current instance to it's reciprocal.
	inline void Reciprocal() {
		long temp = this->_numerator;
		this->_numerator = this->_divisor;
		this->_divisor = temp;
	}

	inline RationalNumber operator/(const RationalNumber& n) {
		RationalNumber r(this->_numerator * n._divisor, this->_divisor * n._numerator);
		return r;
	}
	inline RationalNumber operator/(const long& n) {
		RationalNumber r(this->_numerator, this->_divisor * n);
		return r;
	}
	inline double operator/(const double& rhs) {
		if (rhs == 0) { return 0.0; }
		return this->GetValue() / rhs;
	}

	inline RationalNumber operator*(const RationalNumber& n) {
		RationalNumber r(this->_numerator * n._numerator, this->_divisor * n._divisor);
		return r;
	}
	inline RationalNumber operator*(const long& n) {
		RationalNumber r(this->_numerator * n, this->_divisor);
		return r;
	}
	inline double operator*(const double& rhs) {
		return this->GetValue() * rhs;
	}

	inline RationalNumber operator-(const RationalNumber& n) {
		RationalNumber r((this->_numerator * n._divisor) - (this->_divisor * n._numerator), this->_divisor * n._divisor);
		return r;
	}
	inline RationalNumber operator-(const long& n) {
		RationalNumber r(this->_numerator - (this->_divisor * n), this->_divisor);
		return r;
	}
	inline double operator-(const double& rhs) {
		return this->GetValue() - rhs;
	}

	inline RationalNumber operator+(const RationalNumber& n) {
		RationalNumber r((this->_numerator * n._divisor) + (this->_divisor * n._numerator), this->_divisor * n._divisor);
		return r;
	}
	inline RationalNumber operator+(const long& n) {
		RationalNumber r(this->_numerator + (this->_divisor * n), this->_divisor);
		return r;
	}
	inline double operator+(const double& rhs) {
		return this->GetValue() + rhs;
	}

	inline bool operator==(const RationalNumber& rhs) {
		RationalNumber blah = rhs;
		return this->GetValue() == (double)blah;
	}
	inline friend bool operator==(const RationalNumber& lhs, const double& rhs) {
		RationalNumber blah = lhs;
		return rhs == (double)blah;
	}
	inline friend bool operator==(const double& lhs, const RationalNumber& rhs) {
		RationalNumber blah = rhs;
		return lhs == (double)blah;
	}

	inline friend std::ostream& operator<<(std::ostream& os, RationalNumber const& n) {
		if (n._divisor == 0) { return os << "NaN"; }
		if (n._divisor == 1) { return os << n._numerator; }
		return os << n._numerator << "/" << n._divisor;
	}

	inline friend std::istream& operator>>(std::istream& in, RationalNumber& rhs) {
		std::cout << "Enter numerator ";
		in >> rhs._numerator;
		std::cout << "Enter divisor ";
		in >> rhs._divisor;
		return in;
	}

	inline operator double() { return this->GetValue(); }

	inline friend RationalNumber operator+(const long& n, const RationalNumber& r) {
		return RationalNumber(n) + r;
	}
	inline friend double operator+(const double& lhs, const RationalNumber& rhs) {
		RationalNumber blah = rhs;
		return blah + lhs;
	}

	inline friend RationalNumber operator-(const long& n, const RationalNumber& r) {
		return RationalNumber(n) - r;
	}
	inline friend double operator-(const double& lhs, const RationalNumber& rhs) {
		RationalNumber blah = rhs;
		return lhs - (double)blah;
	}

	inline friend RationalNumber operator*(const long& n, const RationalNumber& r) {
		return RationalNumber(n) * r;
	}
	inline friend double operator*(const double& lhs, const RationalNumber& rhs) {
		RationalNumber blah = rhs;
		return blah * lhs;
	}

	inline friend RationalNumber operator/(const long& n, const RationalNumber& r) {
		return RationalNumber(n) / r;
	}
	inline friend double operator/(const double& lhs, const RationalNumber& rhs) {
		RationalNumber blah = rhs;
		if (!blah.isRealNumber()) { return 0.0; }
		return lhs / (double)blah;
	}
};
