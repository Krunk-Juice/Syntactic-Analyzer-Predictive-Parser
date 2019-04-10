#include <iostream>
#include <string>
#include <stack>
#include <vector>

using namespace std;

class PDA;

class PredictiveTable {
	friend PDA;
protected:
vector<vector<string>> table = {

	{ "ERROR",	"i",		"+",		"-",		"*",		"/",		"(",		")",		"$" },
	{ "E",		"TQ",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"TQ",		"ERROR",	"ERROR" },
	{ "Q",		"ERROR",	"+TQ",		"-TQ",		"ERROR",	"ERROR",	"ERROR",	"\0",		"\0" },
	{ "T",		"FR",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"FR",		"ERROR",	"ERROR"},
	{ "R",		"ERROR",	"\0",		"\0",		"*FR",		"/FR",		"ERROR",	"\0",		"\0" },
	{ "F",		"i",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"(E)",		"ERROR",	"ERROR" }

	};
};

class PDA {
private:
	stack<char> stack_;
	string str;
	PredictiveTable P;
	bool isLAlpha(char);
	bool isUAlpha(char);
	bool isTerminal(char);
	int getRow(char);
	int getCol(char);
public:
	bool parser(string);
};

bool PDA::parser(string s) {
	str = s;
	stack_.push('$');
	stack_.push('E');

	str = str + '$';

	int i = 0;

	while (!stack_.empty()) {
		char t = stack_.top();
		char c = str[i];

		if (c == ' ')
			i++;
		else if (isTerminal(t)) {
			if (t == c) {
				cout << "Processing: " << stack_.top() << endl;
				cout << "Lexeme: " << c << endl;
				stack_.pop();
				i++;
			}
			else
				cout << "Top of stack " << t << " != character input " << c << endl;
		}
		else {
			int l = getRow(t);

			int k = getCol(c);

			if (k == -1) {
				cout << "String NOT Accepted." << endl;
				cout << "At character: " << c << endl;
				break;
			}

			if (P.table[l][k] != "ERROR") {
				stack_.pop();
				cout << t << " -> " << P.table[l][k] << endl;
				for (int x = P.table[l][k].length() - 1; x >= 0; x--) {
					if (P.table[l][k][x] == 'i')
						stack_.push(c);
					else
						stack_.push(P.table[l][k][x]);
				}
			}
			else {
				cout << "You read an ERROR cell." << endl;
				i++;
			}
		}
	}

	if (stack_.empty())
		//cout << "String Accepted!" << endl;
		return true;
	else
		return false;
}

bool PDA::isLAlpha(char c) {
	if (c <= 122 && c >= 97)
		return true;
	return false;
}

bool PDA::isUAlpha(char c) {
	if (c <= 90 && c >= 65)
		return true;
	return false;
}

bool PDA::isTerminal(char c) {
	if (isLAlpha(c) || c == '+' || c == '-' ||
		c == '*' || c == '/' || c == '(' ||
		c == ')' || c == '$')
		return true;
	return false;
}

int PDA::getRow(char c) {
	if (c == 'E')
		return 1;
	else if (c == 'Q')
		return 2;
	else if (c == 'T')
		return 3;
	else if (c == 'R')
		return 4;
	else
		return 5;
}

int PDA::getCol(char c) {
	if (isLAlpha(c))
		return 1;
	else if (c == '+')
		return 2;
	else if (c == '-')
		return 3;
	else if (c == '*')
		return 4;
	else if (c == '/')
		return 5;
	else if (c == '(')
		return 6;
	else if (c == ')')
		return 7;
	else if (c == '$')
		return 8;
	else
		return -1;
}