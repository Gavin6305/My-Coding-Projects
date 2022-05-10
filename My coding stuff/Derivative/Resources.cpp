#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

enum Token {
    //operators
    PLUS, MINUS, DIV, MULT, LPAREN, RPAREN, EXPOP,
    //functions
    SQRT, LOG, LN, SIN, COS, TAN, CSC, SEC, COT,
    //recognized constants
    ECONST, PICONST, CONSTANT,
    //variable
    VARIABLE,
    //other
    ERR, DONE
};

enum State {
    START, INFUNCTION, INCONSTANT
};

map<char, Token> operators = {{'+', PLUS}, {'-', MINUS}, {'/', DIV}, {'*', MULT}, {'(', LPAREN}, {')', RPAREN}, {'^', EXPOP}};
map<string, Token> functions = {{"SQRT", SQRT}, {"LOG", LOG}, {"LN", LN}, {"SIN", SIN}, {"COS", COS}, {"TAN", TAN}, {"CSC", CSC}, {"SEC", SEC}, {"COT", COT}};
map<string, Token> constants = {{"E", ECONST}, {"PI", PICONST}};
map<Token, string> tokenAsString = {
    {PLUS, "PLUS"},
    {MINUS, "MINUS"},
    {DIV, "DIV"},
    {MULT, "MULT"},
    {LPAREN, "LPAREN"},
    {RPAREN, "RPAREN"},
    {EXPOP, "EXPOP"},
    {SQRT, "SQRT"},
    {LOG, "LOG"},
    {LN, "LN"},
    {SIN, "SIN"},
    {COS, "COS"},
    {TAN, "TAN"},
    {CSC, "CSC"},
    {SEC, "SEC"},
    {COT, "COT"},
    {ECONST, "ECONST"},
    {PICONST, "PICONST"},
    {CONSTANT, "CONSTANT"},
    {VARIABLE, "VARIABLE"},
    {ERR, "ERROR"},
    {DONE, "DONE"}
};
                                    
bool isIn (char word, map<char, Token> group) { 
    map<char, Token>::iterator it = group.find(word);
    return it != group.end();
}

bool isIn (string word, map<string, Token> group) { 
    map<string, Token>::iterator it = group.find(word);
    return it != group.end();
}

bool isDigit (char ch) {
    string numbers = "1234567890";
    for (int i = 0; i < numbers.length(); i++) {
        if (ch == numbers[i]) {
            return true;
        }
    }
    return false;
}

bool isLetter (char ch) {
    string letter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < letter.length(); i++) {
        if (ch == letter[i]) {
            return true;
        }
    }
    return false;
}

string removeSpaces (string s) {
    string r;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] != ' ') {
            r += s[i];
        }
    }
    return r;
}

class Element {
    Token type;
    string symbol;

    public:
	Element() {
		type = ERR;
		symbol = " ";
	}
	Element(Token type, string symbol) {
		this->type = type;
		this->symbol = symbol;
	}

	bool operator==(const Token type) const { return this->type == type; }
	bool operator!=(const Token type) const { return this->type != type; }

	Token GetToken() const { return type; }
	string GetSymbol() const { return symbol; }
};

ostream& operator<< (ostream& out, const Element e) {
    Token t = e.GetToken();
    string symbol = e.GetSymbol();
    cout << tokenAsString[t] << " (\"" << symbol << "\")";
    return out;
}