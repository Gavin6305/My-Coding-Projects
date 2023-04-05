#include "utilities.cpp"

using namespace std;

int main () {
    ChessBoard test = ChessBoard("5K1k/7b/8/8/8/8/4B3/8 w - - 0 1");
    visualize(test);
    cout << "Draw: " << (test.isDraw()[0] ? "Y" : "N") << endl;
    return 0;
}