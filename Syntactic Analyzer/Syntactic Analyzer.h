#include <stack>

class PDA;

class PredictiveTable {
	friend PDA;
protected:
vector<vector<string>> table = {

	{ "ERROR",	"id",		"+",		"-",		"*",		"/",		"(",		")",		"$" },
	{ "E",		"TQ",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"TQ",		"ERROR",	"ERROR" },
	{ "Q",		"ERROR",	"+TQ",		"-TQ",		"ERROR",	"ERROR",	"ERROR",	"\0",		"\0" },
	{ "T",		"FR",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"FR",		"ERROR",	"ERROR"},
	{ "R",		"ERROR",	"\0",		"\0",		"*FR",		"/FR",		"ERROR",	"\0",		"\0" },
	{ "F",		"id",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"(E)",		"ERROR",	"ERROR" }

	};
};

class PDA {
private:
	stack<string> stack_;
	string str;
	PredictiveTable P;
	//bool isLAlpha(string);
	//bool isUAlpha(string);
	bool isTerminal(string);
	bool isKeyword(string);
	int getRow(string);
	int getCol(pair<string, string>);
public:
	bool parser(string, vector<Token>);
};

bool PDA::parser(string s, vector<Token> tokens) {
	str = s;

	stack_.push("$");
	stack_.push("E");

	str = str + '$';

	// Adds an extra element to the end of the vector to prevent out of bounds.
	tokens.push_back(Token("END", "$"));

	int i = 0;

	while (!stack_.empty()) {
		string t = stack_.top();
		string c = tokens[i].getLexeme();
		pair<string, string> tp (tokens[i].getLexeme(), tokens[i].getToken());

		//if (c == ' ')
		//	i++;
		if (isTerminal(t)) {
			if (t == c || (t == "id" && tokens[i].getToken() == "IDENTIFIER")) {
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

			int k = getCol(tp);

			if (k == -1) {
				cout << "String NOT Accepted." << endl;
				cout << "At character: " << c << endl;
				break;
			}

			if (P.table[l][k] != "ERROR") {
				stack_.pop();
				cout << t << " -> " << P.table[l][k] << endl;
				if (P.table[l][k] == "id")
					stack_.push("id");
				else {
					for (int x = P.table[l][k].length() - 1; x >= 0; x--) {
							stack_.push(string(1, P.table[l][k][x]));
					}
				}
			}
			else {
				cout << "You read an ERROR cell." << endl;
				i++;
			}
		}
	}

	// Deletes unnecessary Token("END", "$") that was added at the beginning.
	tokens.pop_back();

	if (stack_.empty())
		//cout << "String Accepted!" << endl;
		return true;
	else
		return false;
}

//bool PDA::isLAlpha(char c) {
//	if (c <= 122 && c >= 97)
//		return true;
//	return false;
//}
//
//bool PDA::isUAlpha(char c) {
//	if (c <= 90 && c >= 65)
//		return true;
//	return false;
//}

bool PDA::isTerminal(string c) {
	if (isKeyword(c) || c == "+" || c == "-" ||
		c == "*" || c == "/" || c == "(" ||
		c == ")" || c == "$" || c == "id")
		return true;
	return false;
}

int PDA::getRow(string c) {
	if (c == "E")
		return 1;
	else if (c == "Q")
		return 2;
	else if (c == "T")
		return 3;
	else if (c == "R")
		return 4;
	else
		return 5;
}

int PDA::getCol(pair<string, string> c) {
	if (c.second == "IDENTIFIER")
		return 1;
	else if (c.first == "+")
		return 2;
	else if (c.first == "-")
		return 3;
	else if (c.first == "*")
		return 4;
	else if (c.first == "/")
		return 5;
	else if (c.first == "(")
		return 6;
	else if (c.first == ")")
		return 7;
	else if (c.first == "$")
		return 8;
	else
		return -1;
}

bool PDA::isKeyword(string c) {
	if (c == "int"	|| c == "float"	|| c == "bool" ||
		c == "if"	|| c == "else"	|| c == "then" ||
		c == "for"	|| c == "while" || c == "whileend" ||
		c == "do"	|| c == "doend" || c == "and" ||
		c == "or"	|| c == "function")
		return true;
	return false;
}