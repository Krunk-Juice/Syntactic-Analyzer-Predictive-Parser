#include <iostream>
#include <string>
#include <stack>
#include <vector>

using namespace std;

bool isLAlpha(char);
bool isUAlpha(char);
bool isTerminal(char);
int getRow(char);
int getCol(char);

/*			id		+		-		*		/		(		)		$		*/	
// E = 1
// Q = 2
// T = 3
// R = 4
// F = 5

vector<vector<string>> table = { 

	{ "ERROR",	"i",		"+",		"-",		"*",		"/",		"(",		")",		"$" },
	{ "E",		"TQ",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"TQ",		"ERROR",	"ERROR" },
	{ "Q",		"ERROR",	"+TQ",		"-TQ",		"ERROR",	"ERROR",	"ERROR",	"\0",		"\0" },
	{ "T",		"FR",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"FR",		"ERROR",	"ERROR"},
	{ "R",		"ERROR",	"\0",		"\0",		"*FR",		"/FR",		"ERROR",	"\0",		"\0" },
	{ "F",		"i",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"(E)",		"ERROR",	"ERROR" } 

};

int main() {
	stack<char> stack_;
	stack_.push('$');
	stack_.push('E');

	string str = "a - b * c + d / f";
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
			//if (t == 'E')
			//	l = 0;
			//else if (t == 'Q')
			//	l = 1;
			//else if (t == 'T')
			//	l = 2;
			//else if (t == 'R')
			//	l = 3;
			//else
			//	l = 4;

			int k = getCol(c);
			//if (isLAlpha(c))
			//	k = 0;
			//else if (c == '+')
			//	k = 1;
			//else if (c == '-')
			//	k = 2;
			//else if (c == '*')
			//	k = 3;
			//else if (c == '/')
			//	k = 4;
			//else if (c == '(')
			//	k = 5;
			//else if (c == ')')
			//	k = 6;
			//else if (c == '$')
			//	k = 7;
			//else {
			//	cout << "String not accepted." << endl;
			//	cout << "At character: " << c << endl;
			//	k = 0;
			//	l = 0;
			//}

			if (k == -1) {
				cout << "String not accepted." << endl;
				cout << "At character: " << c << endl;
				break;
			}

			if (table[l][k] != "ERROR") {
				stack_.pop();
				cout << t << " -> " << table[l][k] << endl;
				for (int x = table[l][k].length() - 1; x >= 0; x--) {
					if (table[l][k][x] == 'i')
						stack_.push(c);
					else
						stack_.push(table[l][k][x]);
				}
			}
			else {
				cout << "You read an ERROR cell." << endl;
				i++;
			}
		}
	}

	if (stack_.empty())
		cout << "String Accepted!" << endl;

	return 0;
}

bool isLAlpha(char c) {
	if (c <= 122 && c >= 97)
		return true;
	return false;
}

bool isUAlpha(char c) {
	if (c <= 90 && c >= 65)
		return true;
	return false;
}

bool isTerminal(char c) {
	if (isLAlpha(c) || c == '+' || c == '-' ||
		c == '*' || c == '/' || c == '(' ||
		c == ')' || c == '$')
		return true;
	return false;
}

int getRow(char c) {
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

int getCol(char c) {
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