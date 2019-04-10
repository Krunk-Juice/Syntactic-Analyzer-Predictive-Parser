#include <iostream>
#include <vector>
#include <string>

using namespace std;

// These are the inputs for the FSM.
enum TransitionStates {
	REJECT = 0,				// REJECT is the starting state and final state.
	INTEGER = 1,
	REAL = 2,
	OPERATOR = 3,
	STRING = 4,
	UNKNOWN = 5,
	SPACE = 6,
	COMMENT = 7,
	SEPARATOR = 8,
	DOLLAR = 9
};

class FSM;
								 
class StateTable {
	friend FSM;					// INPUTS
protected:						 /* INTEGER,   REAL,      OPERATOR,  STRING,    UNKNOWN,   SPACE,   COMMENT,  SEPARATOR, DOLLAR */
	int table[9][10] = { {REJECT,	INTEGER,   REAL,      OPERATOR,  STRING,    UNKNOWN,   SPACE,   COMMENT,  SEPARATOR, DOLLAR},
	/* STATE 1 */		{INTEGER,   INTEGER,   REAL,      REJECT,	 REJECT,	REJECT,	   REJECT,  COMMENT,  REJECT,	 REJECT},
	/* STATE 2 */		{REAL,      REAL,      UNKNOWN,   REJECT,	 REJECT,	REJECT,    REJECT,  COMMENT,  REJECT,	 REJECT},
	/* STATE 3 */		{OPERATOR,  REJECT,	   REJECT,	  REJECT,	 REJECT,    REJECT,    REJECT,  COMMENT,  REJECT,	 REJECT},
	/* STATE 4 */		{STRING,    STRING,    REJECT,	  REJECT,    STRING,    REJECT,    REJECT,  COMMENT,  REJECT,	 STRING},
	/* STATE 5 */		{UNKNOWN,   UNKNOWN,   UNKNOWN,   UNKNOWN,   UNKNOWN,   UNKNOWN,   REJECT,  COMMENT,  REJECT,	 UNKNOWN},
	/* STATE 6 */		{SPACE,     REJECT,	   REJECT,	  REJECT,	 REJECT,	REJECT,    REJECT,  COMMENT,  REJECT,	 REJECT},
	/* STATE 7 */		{COMMENT,	COMMENT,   COMMENT,	  COMMENT,	 COMMENT,	COMMENT,   COMMENT, REJECT,	  COMMENT,	 COMMENT},
	/* STATE 8 */		{SEPARATOR, REJECT,	   REJECT,	  REJECT,	 REJECT,	REJECT,	   REJECT,	COMMENT,  REJECT,	 REJECT}
	};
};

class Token {
private:
	string token;
	string lexeme;
public:
	string getToken(){ return token; }
	string getLexeme() { return lexeme; }
	void setToken(string t) { token = t; }
	void setLexeme(string l) { lexeme = l; }
};

class FSM {
private:
	string currToken;
	Token T;
	StateTable S;
	bool isPrevCom = false;
	int getCol(char currChar);
	string getTokenType(int lex);
	bool isSpace(char currChar);
	bool isDigit(char currChar);
	bool isReal(char currChar);
	bool isAlpha(char currChar);
	bool isOperator(char currChar);
	bool isSeparator(char currChar);
	bool isComment(char currChar);
	bool isKeyword();
public:
	vector<Token> lexer(string);
};

vector<Token> FSM::lexer(string expression) {
	vector<Token> tokens;
	char currChar = ' ';
	int col = REJECT;
	int currState = REJECT;
	int prevState = REJECT;
	currToken = "";

	// Was the previous line a comment?
	if (isPrevCom == true) {
		currState = COMMENT;
		isPrevCom = false;
	}

	for (int i = 0; i < expression.length();) {
		currChar = expression[i];

		// What is the input?
		// get current column/input of current character
		col = getCol(currChar);

		// get current state of expression
		currState = S.table[currState][col];

		// State switching mechanism
		// Is current state starting state?
		if (currState == REJECT) {
			// Only add tokens and lexemes that are not SPACE or COMMENT to container.
			if (prevState != SPACE && prevState != COMMENT) {
				T.setLexeme(currToken);
				T.setToken(getTokenType(prevState));
				tokens.push_back(T);
			}
			// If previous state is COMMENT. We move the expression index forward.
			if (prevState == COMMENT)
				i++;
			// Reset token.
			currToken = "";
		}
		// Will skip comments entirely without catching.
		else if (currState == COMMENT)
			i++;
		// Catch and concat character for current token
		else {
			currToken += currChar;
			i++;
		}
		
		prevState = currState;
	}

	if (currState != SPACE && currToken != "" && currState != COMMENT) {
		T.setLexeme(currToken);
		T.setToken(getTokenType(currState));
		tokens.push_back(T);
	}
	// Handling block comments that extend onto other lines.
	else if (currState == COMMENT)
		isPrevCom = true;

	return tokens;
}

int FSM::getCol(char currChar) {
	if (isSpace(currChar))
		return SPACE;

	else if (isDigit(currChar))
		return INTEGER;

	else if (isReal(currChar))
		return REAL;

	else if (isAlpha(currChar))
		return STRING;

	else if (isOperator(currChar))
		return OPERATOR;

	else if (isSeparator(currChar))
		return SEPARATOR;

	else if (isComment(currChar))
		return COMMENT;

	else if (currChar == '$')
		return DOLLAR;

	return UNKNOWN;
}

string FSM::getTokenType(int lex) {
	switch (lex) {
	case INTEGER:
		return "INTEGER";
		break;
	case REAL:
		return "REAL";
		break;
	case OPERATOR:
		return "OPERATOR";
		break;
	case STRING:
		if (isKeyword())
			return "KEYWORD";
		return "IDENTIFIER";
	case SPACE:
		break;
	case SEPARATOR:
		return "SEPARATOR";
		break;
	case COMMENT:
		return "COMMENT";
		break;
	case UNKNOWN:
		return "UNKNOWN";
		break;
	default:
		return "ERROR";
		break;
	}
}

bool FSM::isSpace(char currChar) {
	if (currChar == ' ' || currChar == '	')
		return true;
	return false;
}

bool FSM::isDigit(char currChar) {
				/*	 0 ---------------- 9	*/
	if (currChar >= '0' && currChar <= '9')
		return true;
	return false;
}

bool FSM::isReal(char currChar) {
	if (currChar == '.')
		return true;
	return false;
}

bool FSM::isAlpha(char currChar) {
				/*	 A -------------- Z						a --------------- z		*/
	if ((currChar >= 65 && currChar <= 90) || (currChar >= 97 && currChar <= 122))
		return true;
	return false;
}

bool FSM::isOperator(char currChar) {
	if (currChar == '+' || currChar == '-' || currChar == '*' || currChar == '/' ||
		currChar == '>' || currChar == '<' || currChar == '=' || currChar == '%')
		return true;
	return false;
}

bool FSM::isComment(char currChar) {
	if (currChar == '!')
		return true;
	return false;
}

bool FSM::isSeparator(char currChar) {
	if (currChar == '(' || currChar == ')' || currChar == '[' || currChar == ']' ||
		currChar == '{' || currChar == '}' || currChar == '.' || currChar == ',' ||
		currChar == ':' || currChar == ';' || currChar == '\'')
		return true;
	return false;
}

bool FSM::isKeyword() {
	if (currToken == "int" || currToken == "float" || currToken == "bool" ||
		currToken == "if" || currToken == "else" || currToken == "then" ||
		currToken == "for" || currToken == "while" || currToken == "whileend" ||
		currToken == "do" || currToken == "doend" || currToken == "and" ||
		currToken == "or" || currToken == "function")
		return true;
	return false;
}