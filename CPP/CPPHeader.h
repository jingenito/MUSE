#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

#include <nlohmann/json.hpp>

#include "CPPLibrary.h"
#include "RationalNumber.h"

using json = nlohmann::json;

const int MAXCOUNT = 100;

json convertQSequenceToJSON(long*, size_t, size_t);

