/* ---------------------------------------------------------------------------
This file contains helpful functions that will be used throughout the project.
------------------------------------------------------------------------------ */

#include <string.h>
#include "piece.h"

//array[i][j] = value
void set (Piece array[][8], int i, int j, Piece value) {
    Piece *ptr = &array[i][j];
    *ptr = value;
}

//array[i] = value
void set (int array[], int i, int value) {
    int *ptr = &array[i];
    *ptr = value;
}

//Constructs a chess board in the starting position
void makeBoard (Piece coords[8][8]) {

   //Placing pawns on second rank for white and black
   for (int file = 0; file < 8; file++) {
        int wRank = 6;
        int bRank = 1;

        set(coords, wRank, file, PAWN);
        set(coords, bRank, file, BPAWN);
   }
   
   //Placing major pieces on the first and eighth rank
    Piece piece = ROOK;
    int order = 1;

    for (int file = 0; file < 8; file++) {
        int wRank = 7;
        int bRank = 0;

        set(coords, wRank, file, piece);
        set(coords, bRank, file, (Piece)(piece + 6));

        if (piece == KING) {
            piece = (Piece)(BISHOP + 1);
            order = -1;
        }

        piece = (Piece)(piece + order);
    }

    //Setting all the open squares in the center to EMPTY
    for (int row = 2; row <= 5; row++) {
        for (int col = 0; col < 8; col++) {
            set(coords, row, col, EMPTY);
        }
    }
}


bool isOpposite (bool isWhite, Piece dest) {
    int diff = isWhite ? 0 : 6;
    return isWhite != (dest >= PAWN + diff && dest <= KING + diff);
}