#include "Lexical Analyzer.h"
#include "Syntactic Analyzer.h"

int main() {
	ifstream infile;
	//ofstream outfile("LexerOutputFile.txt");
	//string str;
	string fileName;
	FSM T;
	PDA U;
	vector<Token> tokens;
	bool accepted = true;

	cout << "Enter file name: ";
	getline(cin, fileName);

	infile.open(fileName.c_str());

	if (infile.fail()) {
		cout << "File " << fileName << " cannot be found." << endl;
		return 0;
	}

	//while (!infile.eof()) {
	//	getline(infile, str);

	//	tokens = T.lexer(str);

	//	if (!U.parser(str, tokens))
	//		accepted = false;

	//	//for (int i = 0; i < tokens.size(); i++) {
	//	//	cout << "Token: " << left << setw(15) << tokens[i].getToken() <<
	//	//		"Lexeme: " << tokens[i].getLexeme() << endl;
	//	//	//outfile << "Token: " << left << setw(15) << tokens[i].getToken() <<
	//	//	//	"Lexeme: " << tokens[i].getLexeme() << endl;
	//	//}
	//}

	/* Read file contents as one string. */
	string str((istreambuf_iterator<char>(infile)),
				(istreambuf_iterator<char>()));

	tokens = T.lexer(str);

	if (!U.parser(str, tokens))
		accepted = false;

	if (accepted)
		cout << "String Accepted." << endl;
	else
		cout << "String NOT Accepted." << endl;

	infile.close();

	return 0;
}