#include "Predictive Parser.h"

/* STATIC FUNCTIONS */

static void ignoreSpace(istream& in) {
	while (isSpace(in.peek()))
		in.get();
}

static char getChar(istream& in) {
	ignoreSpace(in);
	char ret = in.get();
	ignoreSpace(in);
	return ret;
}

Production::~Production() {}

const char* ParseError::what() const throw() {
	return "a parsing error occurred.";
}

bool isSpace(char currChar) {
	if (currChar == ' ' || currChar == '	')
		return true;
	return false;
}

bool isDigit(char currChar) {
	/*	 0 ---------------- 9	*/
	if (currChar >= '0' && currChar <= '9')
		return true;
	return false;
}

bool isReal(char currChar) {
	if (currChar == '.')
		return true;
	return false;
}

bool isAlpha(char currChar) {
	/*	 A -------------- Z						a --------------- z		*/
	if ((currChar >= 65 && currChar <= 90) || (currChar >= 97 && currChar <= 122))
		return true;
	return false;
}

bool isOperator(char currChar) {
	if (currChar == '+' || currChar == '-' || currChar == '*' || currChar == '/' ||
		currChar == '>' || currChar == '<' || currChar == '=' || currChar == '%')
		return true;
	return false;
}

bool isComment(char currChar) {
	if (currChar == '!')
		return true;
	return false;
}

bool isSeparator(char currChar) {
	if (currChar == '(' || currChar == ')' || currChar == '[' || currChar == ']' ||
		currChar == '{' || currChar == '}' || currChar == '.' || currChar == ',' ||
		currChar == ':' || currChar == ';' || currChar == '\'')
		return true;
	return false;
}