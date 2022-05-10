#include <iostream>
#include "Derivative_Lexer.cpp"
using namespace std;

void printVector (vector<Element> group) {
    for (Element i : group) {
        cout << i << endl;
    }
}

void WinMain1() {
    string e;
    cin >> e;
    cout << removeSpaces(e) << " " << removeSpaces(e).length() << endl;
}

void WinMain2() {
    string e;
    cin >> e;
    string f;
    for (int i = 0; i < e.length(); i++) {
        f[i] = toupper(e[i]);
    }
    cout << f << endl;
}

void WinMain3 () {
    string expression;
    cout << "Enter expression: " << endl;
    cin >> expression;
    vector<Element> elements = getElements(expression, 'x');
    cout << elements.size() << " elements found" << endl;
    printVector(elements);
}

int WinMain () {
    WinMain3();
    return 0;
}
