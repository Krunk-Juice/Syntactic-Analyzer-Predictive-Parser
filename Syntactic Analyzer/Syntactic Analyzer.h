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
		F -> idZ | (E)Z
		Z -> ; | \0
	*/

vector<vector<string>> table = {
			/*	1			2			3			4			5			6			7			8			9			10				11			12			13			14			15	*/
	{ "ERROR",	"id",		"+",		"-",		"*",		"/",		"=",		"(",		")",		";",		"while",		"<",		">",		"{",		"}",		"$" },
	{ "S",		"A",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"W",			"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR" },
	{ "A",		"id=E",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR" },
	{ "W",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"while(C){S}",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR" },
	{ "C",		"EP",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"EP",		"ERROR",	"ERROR",	"ERROR",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR" },
	{ "P",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",		"<E",		">E",		"ERROR",	"ERROR",	"ERROR" },
	{ "E",		"TQ",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"TQ",		"ERROR",	"ERROR",	"ERROR",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR" },
	{ "Q",		"ERROR",	"+TQ",		"-TQ",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"\0",		"ERROR",	"ERROR",		"\0",		"\0",		"ERROR",	"\0",		"\0" },
	{ "T",		"FR",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"FR",		"ERROR",	"ERROR",	"ERROR",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR"},
	{ "R",		"ERROR",	"\0",		"\0",		"*FR",		"/FR",		"ERROR",	"ERROR",	"\0",		"ERROR",	"ERROR",		"\0",		"\0",		"ERROR",	"\0",		"\0" },
	{ "F",		"idZ",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR",	"(E)Z",		"ERROR",	"ERROR",	"ERROR",		"ERROR",	"ERROR",	"ERROR",	"ERROR",	"ERROR" },
	{ "Z",		"ERROR",	"\0",		"\0",		"\0",		"\0",		"ERROR",	"ERROR",	"\0",		";",		"ERROR",		"\0",		"\0",		"ERROR",	"ERROR",	"\0" }

	};
};

class PDA {
private:
	ofstream outfile;
	stack<string> stack_;
	string str;
	int _index = 0;
	PredictiveTable P;
	bool isTerminal(string);
	void productionS(vector<Token>, stack<string>&);
	int getRow(string);
	int getCol(pair<string, string>);
	void productionPrint(string, string);
	bool isOperator(string currChar);
	bool isSeparator(string currChar);
	bool isKeyword(string currChar);
	bool isDigit(string currChar);
	bool isSpace(string currChar);
public:
	bool parser(string, vector<Token>);
};

bool PDA::parser(string s, vector<Token> tokens) {
	outfile.open("ParserOutputFile.txt");

	bool pcheck = true;
	str = s;
	str = str + '$';

	stack_.push("$");

	// Adds an extra element to the end of the vector to prevent out of bounds.
	tokens.push_back(Token("END", "$"));

	/* If use S -> id=E. Use stack_.push("E") & productionS(vector<Token>). */
	//stack_.push("E");
	//productionS(tokens);

	/* If use S -> idA & A -> =E. Use stack_.push("S").*/
	stack_.push("S");

	while (!stack_.empty()) {

		/* Top of Stack */
 		string t = stack_.top();

		/* Current token being evaluated by the parser. */
		string c = tokens[_index].getLexeme();

		pair<string, string> tp (tokens[_index].getLexeme(), tokens[_index].getToken());

		// Print Token:------ & Lexeme:------
		if (pcheck) {
			cout << "Token: " << left << setw(15) << tokens[_index].getToken() <<
				"Lexeme: " << tokens[_index].getLexeme() << endl;
			outfile << "Token: " << left << setw(15) << tokens[_index].getToken() <<
				"Lexeme: " << tokens[_index].getLexeme() << endl;

			pcheck = false;
		}

		//if (t == "S") {
		//	productionS(tokens, stack_);
		//}
		// Is the top of stack a terminal? Yes.
		else if (isTerminal(t)) {
			if (t == c || (t == "id" && tokens[_index].getToken() == "IDENTIFIER")) {
				cout << "Processing: " << stack_.top() << endl;
				cout << "Lexeme: " << c << endl;
				stack_.pop();
				_index++;
				pcheck = true;
			}
			else {
				cout << "Top of stack " << t << " != character input " << c << endl;
				break;
			}
		}
		// Is the top of stack a terminal? No. Get production rule.
		else {
			int l = getRow(t);

			int k = getCol(tp);

			if (k == -1) {
				cout << "String NOT Accepted." << endl;
				cout << "At character: " << c << endl;
				break;
			}

			/* Was an "ERROR" cell read? No. */
			if (P.table[l][k] != "ERROR") {
				stack_.pop();
				cout << t << " -> " << P.table[l][k] << endl;

				//productionPrint(t, P.table[l][k]);

				/* Pushes strings in the table in reverse onto the stack. */
				if (P.table[l][k] == "id")
					stack_.push("id");
				//else if (P.table[l][k] == "idA") {
				//	stack_.push("A");
				//	stack_.push("id");
				//}
				else if (P.table[l][k] == "id=E") {
					stack_.push("E");
					stack_.push("=");
					stack_.push("id");
				}
				else if (P.table[l][k] == "while(C){S}") {
					stack_.push("}");
					stack_.push("S");
					stack_.push("{");
					stack_.push(")");
					stack_.push("C");
					stack_.push("(");
					stack_.push("while");
				}
				else if (P.table[l][k] == "idZ") {
					stack_.push("Z");
					stack_.push("id");
				}
				else {
					for (int x = P.table[l][k].length() - 1; x >= 0; x--) {
							stack_.push(string(1, P.table[l][k][x]));
					}
				}
			}
			/* Was an "ERROR" cell read? Yes. */
			else {
				cout << "You read an ERROR cell." << endl;
				_index++;
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

/* Check if string is terminal. */
bool PDA::isTerminal(string c) {
	if (c == "id" || isSeparator(c) || isOperator(c) || isKeyword(c) || isDigit(c) || isSpace(c))
		return true;
	return false;
}

/* Production Rule S */
void PDA::productionS(vector<Token> tokens, stack<string>& sta) {
	if (tokens[_index].getToken() == "$")
		return;
	// Is <Statement> -> <Assign> ?
	else if (tokens[_index].getToken() == "IDENTIFIER" && tokens[_index + 1].getLexeme() == "=") {
		//cout << "Token: " << left << setw(15) << tokens[_index].getToken() <<
		//	"Lexeme: " << tokens[_index].getLexeme() << endl;
		//outfile << "Token: " << left << setw(15) << tokens[_index].getToken() <<
		//	"Lexeme: " << tokens[_index].getLexeme() << endl;

		productionPrint("S", "id=E");
		_index++;

		cout << "Token: " << left << setw(15) << tokens[_index].getToken() <<
			"Lexeme: " << tokens[_index].getLexeme() << endl;
		outfile << "Token: " << left << setw(15) << tokens[_index].getToken() <<
			"Lexeme: " << tokens[_index].getLexeme() << endl;

		_index++;
		// Pop "S"
		sta.pop();
		// Push "E"
		sta.push("E");
		return;
	}
	else if (tokens[_index].getLexeme() == "while") {
		if (tokens[_index + 1].getLexeme() == "(") {
			
		}
	}
	else {
		sta.pop();
		sta.push("E");
		return;
	}
}

/* Get table row. */
int PDA::getRow(string c) {
	if (c == "S")
		return 1;
	else if (c == "A")
		return 2;
	else if (c == "W")
		return 3;
	else if (c == "C")
		return 4;
	else if (c == "P")
		return 5;
	else if (c == "E")
		return 6;
	else if (c == "Q")
		return 7;
	else if (c == "T")
		return 8;
	else if (c == "R")
		return 9;
	else if (c == "F")
		return 10;
	else
		return 11;
}

/* Get table column. */
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
	else if (c.first == ";")
		return 9;
	else if (c.first == "while")
		return 10;
	else if (c.first == "<")
		return 11;
	else if (c.first == ">")
		return 12;
	else if (c.first == "{")
		return 13;
	else if (c.first == "}")
		return 14;
	else if (c.first == "$")
		return 15;
	else
		return -1;
}

/* Print production rules. */
void PDA::productionPrint(string t, string s) {
	if (t == "S" && s == "id=E") {
		cout << "<Statement> -> <Assign>" << endl;
		cout << "<Assign> -> <Identifier> = <Expression>" << endl;
		outfile << "<Statement> -> <Assign>" << endl;
		outfile << "<Assign> -> <Identifier> = <Expression>" << endl;
	}

	else if (s == "TQ") {
		cout << "<Expression> -> <Term> <Expression Prime>" << endl;
		outfile << "<Expression> -> <Term> <Expression Prime>" << endl;
	}

	else if (s == "+TQ") {
		cout << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;
		outfile << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;
	}

	else if (s == "-TQ") {
		cout << "<Expression Prime> -> - <Term> <Expression Prime>" << endl;
		outfile << "<Expression Prime> -> - <Term> <Expression Prime>" << endl;
	}

	else if (s == "FR") {
		cout << "<Term> -> <Factor> <Term Prime>" << endl;
		outfile << "<Term> -> <Factor> <Term Prime>" << endl;
	}

	else if (s == "*FR") {
		cout << "<Term Prime> -> * <Factor> <Term Prime>" << endl;
		outfile << "<Term Prime> -> * <Factor> <Term Prime>" << endl;
	}

	else if (s == "/FR") {
		cout << "<Term Prime> -> / <Factor> <Term Prime>" << endl;
		outfile << "<Term Prime> -> / <Factor> <Term Prime>" << endl;
	}

	else if (s == "id") {
		cout << "<Factor> -> <Identifier>" << endl;
		outfile << "<Factor> -> <Identifier>" << endl;
	}

	else if (s == "(E)") {
		cout << "<Factor> -> ( <Expression> )" << endl;
		outfile << "<Factor> -> ( <Expression> )" << endl;
	}

	else if (t == "Q" && s == "\0") {
		cout << "<Expression Prime> -> <Epsilon>" << endl;
		outfile << "<Expression Prime> -> <Epsilon>" << endl;
	}

	else if (t == "R" && s == "\0") {
		cout << "<Term Prime> -> <Epsilon>" << endl;
		outfile << "<Term Prime> -> <Epsilon>" << endl;
	}

	else if (s == "idZ") {
		cout << "<Factor> -> <Identifier> <Factor Prime>" << endl;
		outfile << "<Factor> -> <Identifier> <Factor Prime>" << endl;
	}

	else if (s == "(E)Z") {
		cout << "<Factor> -> ( <Expression> ) <Factor Prime>" << endl;
		outfile << "<Factor> -> ( <Expression> ) <Factor Prime>" << endl;
	}

	else if (s == ";") {
		cout << "<Factor Prime> -> <Delimiter>" << endl;
		outfile << "<Factor Prime> -> <Delimiter>" << endl;
	}

	else if (t == "Z" && s == "\0") {
		cout << "<Factor Prime> -> <Epsilon>" << endl;
		outfile << "<Factor Prime> -> <Epsilon>" << endl;
	}

	else {
		//cout << "No format found for " << s << " in productionPrint." << endl;
		//outfile << "No format found for " << s << " in productionPrint." << endl;
	}
}

bool PDA::isOperator(string currChar) {
	if (currChar == "+" || currChar == "-" || currChar == "*" || currChar == "/" ||
		currChar == ">" || currChar == "<" || currChar == "=" || currChar == "%")
		return true;
	return false;
}

bool PDA::isSeparator(string currChar) {
	if (currChar == "(" || currChar == ")" || currChar == "[" || currChar == "]" ||
		currChar == "{" || currChar == "}" || currChar == "." || currChar == "," ||
		currChar == ":" || currChar == ";" || currChar == "\'")
		return true;
	return false;
}

bool PDA::isKeyword(string currChar) {
	if (currChar == "int" || currChar == "float" || currChar == "bool" ||
		currChar == "if" || currChar == "else" || currChar == "then" ||
		currChar == "for" || currChar == "while" || currChar == "whileend" ||
		currChar == "do" || currChar == "doend" || currChar == "and" ||
		currChar == "or" || currChar == "function")
		return true;
	return false;
}

bool PDA::isDigit(string currChar) {
	bool d = true;
	int i = currChar.size() - 1;
	int result = 0, multiplier = 1;
	for (; i >= 0; i--) {
		if (currChar[i] >= '0' && currChar[i] <= '9') {
			result = result + currChar[i] * multiplier;
			multiplier *= 10;
		}
		else {
			d = false;
			break;
		}
	}
	return d;
}

bool PDA::isSpace(string currChar) {
	if (currChar.size() == 1 && (currChar[0] == ' ' || currChar[0] == '	' || currChar[0] == '\n'))
		return true;
	return false;
}