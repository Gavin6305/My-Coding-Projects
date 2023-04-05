#include <iostream>
#include <string>
#include <map>
#include <cmath>

using namespace std;

//An enum representing a chess piece
enum Piece {
    PAWN = 1, ROOK, KNIGHT, BISHOP, QUEEN, KING,
    BPAWN, BROOK, BKNIGHT, BBISHOP, BQUEEN, BKING,
    EMPTY,
    ERROR
};

//One character representations of chess pieces (Indices are (int)Pieces)
char pieceSymbols[15] = {' ', 'P', 'R', 'N', 'B', 'Q', 'K', 'p', 'r', 'n', 'b', 'q', 'k', ' ', '0'};

//String representations of chess pieces
string pieceStrings[15] = {"Out of bounds", "White Pawn", "White Rook", "White Knight", "White Bishop", "White Queen", "White King",
 "Black Pawn", "Black Rook", "Black Knight", "Black Bishop", "Black Queen", "Black King", "Empty Square", "Error"};

//Point value of each chess piece
char pieceValues[15] = {0, 1, 5, 3, 3, 9, 10, 1, 5, 3, 3, 9, 10, 0, 0};

//Converts a chess notation character such as 'N' to a Piece enum, i.e. 'N' = KNIGHT
Piece symToPiece (char s) {
    map<char, Piece> symToPiece;
    for (int i = 1; i <= 13; i++) {
        symToPiece[pieceSymbols[i]] = (Piece)(i);
    }
    return symToPiece[s];
}

//Converts the Piece enum to a single character based on chess notation, i.e. PAWN = 'P', BPAWN = 'p'
char pieceToSymbol (Piece piece) {
    return pieceSymbols[piece];
}

//Converts the Piece enum to a readable string, i.e. PAWN = "White Pawn", BPAWN = "Black Pawn"
string pieceToStr (Piece piece) {
    return pieceStrings[piece];
}

//Returns the point value of the piece.
int getValue (Piece piece) {
    return pieceValues[piece];
}

//Returns true if pieces are same color, false otherwise
bool sameColors (Piece p1, Piece p2) {
    return (p1 >= 1 && p1 <= 6) == (p2 >= 1 && p2 <= 6);
}

//Returns true if board(row,col) is a dark square
bool isDarkSquare (int row, int col) {
    return (row % 2 == 0 && col % 2 == 1);
}