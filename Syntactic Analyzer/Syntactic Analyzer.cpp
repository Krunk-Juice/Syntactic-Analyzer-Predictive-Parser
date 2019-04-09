#include <iostream>
#include <string>
#include <stack>
#include <vector>

using namespace std;
/*			id		+		*		(		)		$		*/	
// E = 0
// Q = 1
// T = 2
// R = 3
// F = 4

vector<vector<string>> table = { { "TQ", "ERROR", "ERROR", "TQ", "ERROR", "ERROR" },
								{ "ERROR", "+TQ", "ERROR", "ERROR", "\0", "\0" },
								{ "FR", "ERROR", "ERROR", "FR", "ERROR", "ERROR"},
								{ "ERROR", "\0", "*FR", "ERROR", "\0", "\0" },
								{ "i", "ERROR", "ERROR", "(E)", "ERROR", "ERROR" } };

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

int main() {
	stack<char> stack_;
	stack_.push('$');
	stack_.push('E');

	string str = "a * b + c";
	str = str + '$';

	int i = 0;

	while (!stack_.empty()) {
		char t = stack_.top();
		char c = str[i];

		if (c == ' ')
			i++;
		else if (isLAlpha(t) ||
			t == '+' ||
			t == '*' ||
			t == '(' ||
			t == ')' ||
			t == '$') {
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
			int l;
			if (t == 'E')
				l = 0;
			else if (t == 'Q')
				l = 1;
			else if (t == 'T')
				l = 2;
			else if (t == 'R')
				l = 3;
			else
				l = 4;

			int k;
			if (isLAlpha(c))
				k = 0;
			else if (c == '+')
				k = 1;
			else if (c == '*')
				k = 2;
			else if (c == '(')
				k = 3;
			else if (c == ')')
				k = 4;
			else if (c == '$')
				k = 5;
			else {
				cout << "String not accepted." << endl;
				cout << "At character: " << c << endl;
				k = 5;
				l = 0;
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

	cout << "String Accepted!" << endl;

	return 0;
}