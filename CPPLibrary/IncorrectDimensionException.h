/*
	Author: Joe Ingenito
	Date Created: 6/16/2020
	Description: An exception that inherits from std::exception to be used for exceptions with matrix and vector operations.
*/

#pragma once

#include <string>
#include <exception> 

class IncorrectDimensionException : std::exception {
private:
	std::string _message;
	std::exception _inner;
public:
	IncorrectDimensionException() {

	}

	IncorrectDimensionException(std::string message) {
		this->_message = message;
	}

	IncorrectDimensionException(std::string message, std::exception inner) {
		this->_message = message;
		this->_inner = inner;
	}

	inline std::string getMessage() { return this->_message; }

	inline std::exception getInnerException() { return this->_inner; }
};