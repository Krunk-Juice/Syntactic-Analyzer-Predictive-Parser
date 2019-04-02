#include <iostream>
#include <string>
#include <istream>
#include <deque>
#include <exception>

using namespace std;

class Production;
class Expression;
class Term;
class Factor;
class Number;
class ParseError;

class Production {
	virtual ~Production();
	virtual double getValue() = 0;
};

class ParseError : public exception {
	const char* what() const throw();
};