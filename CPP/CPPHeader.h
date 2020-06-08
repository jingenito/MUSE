#pragma once

#ifndef CPPHEADER_H
#define CPPHEADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

#include <Eigen/Dense>
#include <nlohmann/json.hpp>

#include "RationalNumber.h"

using namespace std;

using Eigen::MatrixXd;
using json = nlohmann::json;

const int MAXCOUNT = 100;
const double pi = 3.14159265358979323846;
const double e = 2.7182818284590452353602874713527;
const double phi = 1.61803398874989484820458683436;

extern long* continuedFractionExpansion(double, size_t&, size_t&);
extern void printArray(long*, size_t);
extern void printConvergences(long*, size_t);
extern double tryParseNumFromArgs(string);
extern double tryParseMathFromArgs(string);

inline bool is_number(const string& s)
{
    return !s.empty() && find_if(s.begin(),
        s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end();
}

json convertQSequenceToJSON(long*, size_t, size_t);

#endif /*CPPHEADER_H*/

