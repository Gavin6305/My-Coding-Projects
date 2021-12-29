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

Element varOrFunct (string word, char variable) {
    char var = toupper(variable);
    if (isIn(word, functions)) {
        return Element(functions[word], word);
    }
    else if (isIn(word, constants)) {
        return Element(constants[word], word);
    }
    else if (word.length() == 1 && word[0] == var) {
        return Element(VARIABLE, word);
    }
    return Element();
}

Element validConst (string number) {
    if (number[number.length() - 1] == '.') {
        return Element();
    }
    bool hasDecimal = false;
    for (int i = 0; i < number.length(); i++) {
        char digit = number[i];
        if (digit == '.') {
            if (!hasDecimal) {
                hasDecimal = true;
            }
            else {
                return Element();
            }
        }
    }
    return Element(CONSTANT, number);
}

vector<Element> getElements (string expression, char variable) {
    vector<Element> result;
    State currentState = START;
    string symbol;
    string reset = symbol;
    for (int i = 0; i < expression.length(); i++) {
        char current = isalpha(expression[i]) ? toupper(expression[i]) : expression[i];
        symbol += current;
        switch (currentState) {
            case START:
                if (isIn(current, operators)) {
                    currentState = START;
                    result.push_back(Element(operators[current], symbol));
                    symbol = reset;
                }
                else if (current == variable) {
                    currentState = START;
                    result.push_back(Element(VARIABLE, symbol));
                    symbol = reset;
                }
                else if (isDigit(current)) {
                    currentState = INCONSTANT;
                }
                else if (isLetter(current)) {
                    currentState = INFUNCTION;
                }
                else if (isspace(current)) {
                    currentState = START;
                    symbol = reset;
                }
                else {
                    currentState = START;
                    result.push_back(Element(ERR, symbol));
                    symbol = reset;
                }
                break;
            case INCONSTANT:
                if (isDigit(current) || current == '.') {
                    currentState = INCONSTANT;
                }
                else {
                    currentState = START;
                    result.push_back(validConst(symbol.substr(0, symbol.length() - 1)));
                    symbol = current;
                }
                break;
            case INFUNCTION:
                if (isLetter(current)) {
                    currentState = INFUNCTION;
                }
                else {
                    currentState = START;
                    result.push_back(varOrFunct(symbol.substr(0, symbol.length() - 1), variable));
                    symbol = current;
                }
                break;
            default:
                currentState = START;
                result.push_back(Element(ERR, symbol));
                symbol = reset;
        }
    }
    result.push_back(Element(DONE, "End of expression"));
    return result;
}

ostream& operator<< (ostream& out, const Element e) {
    Token t = e.GetToken();
    string symbol = e.GetSymbol();
    cout << tokenAsString[t] << " (\"" << symbol << "\")";
    return out;
}

