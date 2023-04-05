#include "functions.h"

//Constructs a 2d array of Pieces given an FEN string
void makeBoard (string FEN, Piece coords[8][8], bool &isWhite, int wKingPos[2], int bKingPos[2], int wCastle[2], int bCastle[2], int EPTarget[2], int &halfmove, int &fullmove) {
    int field = 1;

    int row = 0;
    int col = 0;

    set(wCastle, 0, 0);
    set(wCastle, 1, 0);
    set(bCastle, 0, 0);
    set(bCastle, 1, 0);

    for (int i = 0; i < FEN.length(); i++) {
        //The current character of the FEN string
        char ch = FEN[i];

        //Separation of fields
        if (ch == ' ') {
            field++;
            continue;
        }

        //Piece layout
        else if (field == 1) { 
            //Separation of ranks
            if (ch == '/') {
                row++;
                col = 0;
            }
            //Empty spaces
            else if (ch >= '1' && ch <= '8') {
                int num = ch - '0';
                for (int i = 0; i < num; i++) {
                    set(coords, row, col, EMPTY);
                    col++;
                }
            }
            //Pieces
            else if ((ch >= 'b' && ch <= 'r') || (ch >= 'B' && ch <= 'R')) {
                Piece piece = symToPiece(ch);
                set(coords, row, col, piece);

                if (ch == 'k' || ch == 'K') {
                    int *ptr = (ch == 'k') ? &bKingPos[0] : &wKingPos[0];
                    *ptr = row;

                    ptr = (ch == 'k') ? &bKingPos[1] : &wKingPos[1];
                    *ptr = col;
                }

                col++; 
            }
        }
        
        //Current color
        else if (field == 2) { 
            isWhite = (ch == 'w');
        }
        
        //Castling rights
        else if (field == 3) { 
            if (ch == '-') {
                continue;
            }
            else if (ch == 'K') {
                set(wCastle, 0, 1);
            }
            else if (ch == 'Q') {
                set(wCastle, 1, 1);
            }
            else if (ch == 'k') {
                set(bCastle, 0, 1);
            }
            else if (ch == 'q') {
                set(bCastle, 1, 1);
            }
        }
        
        //En Passant target square
        else if (field == 4) { 
            if (ch == '-') {
                continue;
            }
            else if (ch >= 'a' && ch <= 'h') {
                set(EPTarget, 1, (int)(ch) - 97);
            }
            else if (ch >= '1' && ch <= '8') {
                set(EPTarget, 0, 8 - (int)(ch - '0'));
            }
        }
        
        //Halfmove counter
        else if (field == 5) { 
            int digit = ch - '0';
            halfmove *= 10;
            halfmove += digit;
        }   

        //Fullmove counter
        else if (field == 6) { 
            int digit = ch - '0';
            fullmove *= 10;
            fullmove += digit;
        }
    }
}   

//Converts a chess position into the board construction part of an FEN string
string posToString (Piece coords[8][8]) {

    string result = "";

    int spaces = 0;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {

            Piece square = coords[row][col];

            if (square == EMPTY) {
                spaces++;
            }
            else {
                if (spaces > 0) {
                    result += ('0' + spaces);
                }
                spaces += pieceToSymbol(square);
            }
            if (col == 7) {
                result += '/';
            }
        }
    }

    return result;
}

//Returns true if the square at coords[row][col] is attacked by an enemy piece.
bool isAttacked (Piece coords[8][8], bool isWhite, int row, int col) {

    Piece square = coords[row][col];
    Piece attacker = EMPTY;

    /*** Testing if square is attacked by EVERY PIECE EXCEPT KNIGHTS ***/
    int multiplier = 1;
    int outOfBoard = 0;

    //tr, tl, br, bl, up, down, left, right
    int dirs[8][2] = {{1,1}, {1,-1}, {-1,1}, {-1,-1}, {1,0}, {-1,0}, {0,-1}, {0, 1}};

    while (multiplier < 8) {

        outOfBoard = 0;
        
        for (int i = 0; i < 8; i++) {

            int attR = row + multiplier * dirs[i][0];
            int attC = col + multiplier * dirs[i][1];

            //First check if the indices are within the board
            if (attR >= 0 && attR < 8 && attC >= 0 && attC < 8) {

                //Assign an attacker piece
                attacker = coords[row][col];
                
                //If one of the square's directions is blocked by a piece of the same color, make the direction vector invalid
                if (sameColors(square, attacker)) {
                    dirs[i][0] *= 10;
                    dirs[i][1] *= 10;
                }
                
                //If the square is attacked by the enemy king (used for avoiding illegal king moves)
                else if (multiplier == 1 && attacker == (isWhite ? BKING : KING)) {
                    return true;
                }

                //If the square is being attacked by PAWNS (only on IMMEDIATE (multiplier == 1) tl or tr if white, bl or br if black)
                else if (   (isWhite ? (i < 2) : (i == 2 || i == 3)) 
                            && multiplier == 1 
                            && attacker == (isWhite ? BPAWN : PAWN)
                        ) 
                {
                    return true;
                }

                //If the square is attacked by an enemy queen or bishop on its diagonals
                else if (   i < 4 
                            && (    attacker == (isWhite ? BQUEEN : QUEEN)
                                    || attacker == (isWhite ? BBISHOP : BISHOP)
                            )
                        ) 
                {
                    return true;
                }

                //If the square is attacked by an enemy queen or rook on its straight paths
                else if (   i >= 4 
                            && (    attacker == (isWhite ? BQUEEN : QUEEN) 
                                    || attacker == (isWhite ? BROOK : ROOK)
                            )
                        ) 
                {
                    return true;
                }
            }

            //Increment outOfBoard if the vector results in a square outside of the board
            else {
                outOfBoard++;
            }
        }

        //Once all dirs are checked, if outOfBoard >= 8 then all dirs are checked thoroughly
        if (outOfBoard >= 8) {
            break;
        }

        //Increment multiplier for vectors to check along straight paths and diagonals
        multiplier++;
    }

    /*** Testing if square is attacked by KNIGHTS ***/
    int kDirs[8][2] = {{2,1}, {2,-1}, {-2,1}, {-2,-1}};

    for (int i = 0; i < 4; i++) {

        //The knight attacks in an L shape, and an L can be drawn 2 different ways (i.e. 2 up, 1 right and 2 right, 1 up)
        int attR = row + kDirs[i][0];
        int attC = col + kDirs[i][1];

        int attR2 = row + kDirs[i][1];
        int attC2 = col + kDirs[i][0];

        if (attR >= 0 && attR < 8 && attC >= 0 && attC < 8) {
            attacker = coords[attR][attC];
            if (attacker == (isWhite ? BKNIGHT : KNIGHT)) {
                return true;
            }
        }

        if (attR2 >= 0 && attR2 < 8 && attC2 >= 0 && attC2 < 8) {
            attacker = coords[attR2][attC2];
            if (attacker == (isWhite ? BKNIGHT : KNIGHT)) {
                return true;
            }
        }
    }

    return false;
}