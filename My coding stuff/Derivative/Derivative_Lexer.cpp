#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "Resources.cpp"


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
    return Element(ERR, word);
}

Element validConst (string number) {
    if (number[number.length() - 1] == '.') {
        return Element(ERR, number);
    }
    bool hasDecimal = false;
    for (int i = 0; i < number.length(); i++) {
        char digit = number[i];
        if (digit == '.') {
            if (!hasDecimal) {
                hasDecimal = true;
            }
            else {
                return Element(ERR, number);
            }
        }
    }
    return Element(CONSTANT, number);
}

vector<Element> getElements (string& expression, char variable) {
    
    vector<Element> result;
    State currentState = START;
    string symbol; //start with empty string

    for (int i = 0; i < expression.length(); i++) {
        char current = isalpha(expression[i]) ? toupper(expression[i]) : expression[i];
        switch (currentState) {
            case START:
                //cout << "In Start: " << current << endl;   
                symbol = current;
                if (isIn(current, operators)) {
                    result.push_back(Element(operators[current], symbol));
                    symbol = "";
                }
                else if (current == toupper(variable)) {
                    result.push_back(Element(VARIABLE, symbol));
                    symbol = "";
                }
                else if (isDigit(current) || current == '.') {
                    currentState = INCONSTANT;
                }
                else if (isLetter(current)) {
                    currentState = INFUNCTION;
                }
                else {
                    result.push_back(Element(ERR, symbol));
                    symbol = "";
                }
                break;
            case INCONSTANT:
                //cout << "In constant: " << current << endl;
                if (isDigit(current) || current == '.') {
                    symbol += current;
                }
                else {
                    currentState = START;
                    result.push_back(validConst(symbol.substr(0, symbol.length())));
                    cout << validConst(symbol.substr(0, symbol.length())) << endl;
                    symbol = current;
                    i--;
                }
                break;
            case INFUNCTION:
                //cout << "In function: " << current << endl;
                if (isLetter(current)) {
                    symbol += current;
                }
                else {
                    currentState = START;
                    result.push_back(varOrFunct(symbol.substr(0, symbol.length()), variable));
                    symbol = current;
                    i--;
                }
                break;
            default:
                currentState = START;
                result.push_back(Element(ERR, symbol));
        }
    }
    result.push_back(Element(DONE, "End of expression"));
    return result;
}



