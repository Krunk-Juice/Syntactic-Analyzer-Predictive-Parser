#include <stack>
#include <iomanip>
#include <fstream>

class PDA;

class PredictiveTable {
	friend PDA;
protected:

	/* Production Rules:
		S -> id=E
		E -> TQ
		Q -> +TQ | -TQ | \0
		T -> FR
		R -> *FR | /FR | \0
		F -> id | (E)
	*/

vector<vector<string>> table = {

	{ "ERROR",	"id",		"+",		"-",		"*",		"/",		"=",		"(",		")",		"$" },
	{ "S",		"id=E",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR" },
	{ "A",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"\0" },
	{ "E",		"TQ",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"TQ",		"ERROR",	"ERROR" },
	{ "Q",		"ERROR",	"+TQ",		"-TQ",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"\0",		"\0" },
	{ "T",		"FR",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"FR",		"ERROR",	"ERROR"},
	{ "R",		"ERROR",	"\0",		"\0",		"*FR",		"/FR",		"ERROR",	"ERROR",	"\0",		"\0" },
	{ "F",		"id",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"(E)",		"ERROR",	"ERROR" }

	};
};

class PDA {
private:
	stack<string> stack_;
	string str;
	PredictiveTable P;
	bool isTerminal(string);
	bool productionS(string);
	int getRow(string);
	int getCol(pair<string, string>);
	void productionPrint(string, string);
	bool isKeyword(string);
public:
	bool parser(string, vector<Token>);
};

bool PDA::parser(string s, vector<Token> tokens) {
	bool pcheck = true;
	str = s;

	stack_.push("$");
	//stack_.push("E");
	stack_.push("S");

	str = str + '$';

	// Adds an extra element to the end of the vector to prevent out of bounds.
	tokens.push_back(Token("END", "$"));

	int i = 0;

	while (!stack_.empty()) {
		string t = stack_.top();
		string c = tokens[i].getLexeme();
		pair<string, string> tp (tokens[i].getLexeme(), tokens[i].getToken());

		// Print Token:------ & Lexeme:------
		if (pcheck) {
			cout << "Token: " << left << setw(15) << tokens[i].getToken() <<
				"Lexeme: " << tokens[i].getLexeme() << endl;
			//outfile << "Token: " << left << setw(15) << tokens[i].getToken() <<
			//	"Lexeme: " << tokens[i].getLexeme() << endl;

			pcheck = false;
		}

		//if (c == ' ')
		//	i++;
		if (isTerminal(t)) {
			if (t == c || (t == "id" && tokens[i].getToken() == "IDENTIFIER")) {
				//cout << "Processing: " << stack_.top() << endl;
				//cout << "Lexeme: " << c << endl;
				stack_.pop();
				i++;
				pcheck = true;
			}
			else {
				cout << "Top of stack " << t << " != character input " << c << endl;
				break;
			}
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
				//cout << t << " -> " << P.table[l][k] << endl;
				productionPrint(t, P.table[l][k]);
				if (P.table[l][k] == "id")
					stack_.push("id");
				else if (P.table[l][k] == "id=E") {
					stack_.push("E");
					stack_.push("=");
					stack_.push("id");
				}
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

bool PDA::isTerminal(string c) {
	if (c == "id" || c == "+" || c == "-" ||
		c == "*" || c == "/" || c == "(" ||
		c == ")" || c == "$" || c == "=")
		return true;
	return false;
}

int PDA::getRow(string c) {
	if (c == "S")
		return 1;
	else if (c == "A")
		return 2;
	else if (c == "E")
		return 3;
	else if (c == "Q")
		return 4;
	else if (c == "T")
		return 5;
	else if (c == "R")
		return 6;
	else
		return 7;
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
	else if (c.first == "=")
		return 6;
	else if (c.first == "(")
		return 7;
	else if (c.first == ")")
		return 8;
	else if (c.first == "$")
		return 9;
	else
		return -1;
}

void PDA::productionPrint(string t, string s) {
	if (t == "S" && s == "id=E") {
		cout << "<Statement> -> <Assign>" << endl;
		cout << "<Assign> -> <Identifier> = <Expression>" << endl;
	}

	else if (s == "TQ")
		cout << "<Expression> -> <Term> <Expression Prime>" << endl;

	else if (s == "+TQ")
		cout << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;

	else if (s == "-TQ")
		cout << "<Expression Prime> -> - <Term> <Expression Prime>" << endl;

	else if (s == "FR")
		cout << "<Term> -> <Factor> <Term Prime>" << endl;

	else if (s == "*FR")
		cout << "<Term Prime> -> * <Factor> <Term Prime>" << endl;

	else if (s == "/FR")
		cout << "<Term Prime> -> / <Factor> <Term Prime>" << endl;

	else if (s == "id")
		cout << "<Factor> -> <Identifier>" << endl;

	else if (s == "(E)")
		cout << "<Factor> -> ( <Expression> )" << endl;

	else if (t == "Q" && s == "\0")
		cout << "<Expression Prime> -> <Epsilon>" << endl;

	else if (t == "R" && s == "\0")
		cout << "<Term Prime> -> <Epsilon>" << endl;

	else
		cout << s << endl;
}

bool PDA::isKeyword(string c) {
	if (c == "int" || c == "float" || c == "bool" ||
		c == "if" || c == "else" || c == "then" ||
		c == "for" || c == "while" || c == "whileend" ||
		c == "do" || c == "doend" || c == "and" ||
		c == "or" || c == "function")
		return true;
	return false;
}