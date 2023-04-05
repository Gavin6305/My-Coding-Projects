/* -----------------------------------------------------------------------------------------------
This file is responsible for creating a square centric board representation that will be used for
the engine. See https://www.chessprogramming.org/Board_Representation.
-------------------------------------------------------------------------------------------------- */

/* INCLUDES */

#include <vector>
#include <algorithm>
#include "functions.cpp"

/* END INCLUDES */

using namespace std;

/* -----------------------------------------------------------------------------------------------
   An object representation of a chessboard. 
   It uses a square centric board representation to keep track of the game. 
-------------------------------------------------------------------------------------------------- */ 
class ChessBoard {

    /*---------------------------------Instance Variables------------------------------------------*/

    //All the squares on the board and what pieces occupy them
    Piece squares[8][8] = {EMPTY};
    
    //Whether it is white or black's turn
    bool isWhite = 1;
    
    //All the played moves
    vector<string> moveList;
    
    //All the recorded positions
    map<string, int> pastPositions;
    
    //Where the kings are
    int wKingSpot[2] = {7, 4};
    int bKingSpot[2] = {0, 4};

    //Possible En Passant target square
    int EPTarget[2] = {-1, -1};
    
    //Castling rights: 0th index is Kingside (O-O), 1st index is Queenside (O-O-O)
    int wCastle[2] = {1, 1};
    int bCastle[2] = {1, 1};

    //Amount of moves made since the last pawn move or piece capture (Halfmove Clock)
    int halfmoves = 0;

    //Amount of completed turns (Fullmove Clock)
    int fullmoves = 0;

    /*--------------------------------END Instance Variables----------------------------------------*/

    public:
        
        /*------------------------------------Constructors------------------------------------------*/

        //Constructs a ChessBoard object using a two-dimensional array of Piece objects
        ChessBoard() {
            makeBoard(this->squares);
        }

        //Constructs a ChessBoard object using an FEN string (see https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation)
        ChessBoard(string FEN) {
            makeBoard(FEN,
            this->squares,
            isWhite,
            wKingSpot,
            bKingSpot,
            wCastle,
            bCastle,
            EPTarget,
            halfmoves,
            fullmoves);
        }

        /*-----------------------------------END Constructors----------------------------------------*/

        /*-------------------------------------Getters-----------------------------------------------*/

        //Returns the piece that occupies the chess coordinate (i.e. a1, c5...) on the board
        Piece pieceAt (string coord) {
            int row = 8 - (int)(coord.at(1) - '0');
            int col = (int)(coord.at(0)) - 97;

            Piece piece = this->squares[row][col];

            return piece;
        } 

        //Returns the piece that occupies [i][j] in array notation
        Piece pieceAt(int i, int j) {
            return this->squares[i][j];
        }
        
        //Returns true if it is white's turn
        bool whiteTurn () {
            return (*this).isWhite;
        }

        //Returns the current En Passant target square
        int * getEPTarget () {
            return this->EPTarget;
        }

        //Returns the castling rights for white
        int * getWCastle () {
            return this->wCastle;
        }

        //Returns the castling rights for black
        int * getBCastle () {
            return this->bCastle;
        }

        //Returns the number of halfmoves
        int getHalfmoves () {
            return this->halfmoves;
        }

        /*------------------------------------END Getters----------------------------------------------*/

        /*------------------------------------Game Rules-----------------------------------------------*/

        //Returns true if the king is in check
        bool inCheck () {
            return isAttacked( 
                this->squares, 
                this->isWhite, 
                (this->isWhite ? this->wKingSpot[0] : this->bKingSpot[0]), 
                (this->isWhite ? this->wKingSpot[1] : this->bKingSpot[1])
            );
        }

        //Returns the text version of the draw type passed from the isDraw() function
        string drawCases (int code) {
            string draws[4] = {"threefold repetition", "insufficient material", "stalemate", "fifty move rule"};
            return draws[code];
        }
        
        //Returns true if the game is a draw
        int * isDraw () {

            static int threefold[2] = {1,0};
            static int material[2] = {1,1};
            static int stalemate[2] = {1,2};
            static int fiftyMove[2] = {1,3};
            static int notDraw[2] = {0,0};

            /*** Threefold repetition: If a position is repeated 3 times ***/
            map<string, int>::iterator it;
            for (it = pastPositions.begin(); it != pastPositions.end(); it++) {
                if (it->second >= 3) {
                    return threefold;
                }
            }

            /*** Insufficient material ***/       
            string drawCases[] = {
            "Kk", 
            "BKk", 
            "Kbk", 
            "KNk", 
            "Knk", 
            "BKbk"
            };

            string piecesLeft = "";
            string mateable = "QqRrPp";
            int bishops[2][2] = {{-1,-1}, {-1,-1}}; //White bishop pos, Black bishop pos

            for (int row = 0; row < 8; row++) {
                for (int col = 0; col < 8; col++) {

                    Piece piece = this->squares[row][col];
                    char symbol = pieceToSymbol(piece);

                    //If a queen, rook, or pawn exists on the board, there's no draw
                    if (mateable.find(symbol) != string::npos) {
                        goto STALEMATE; 
                    }

                    else if (piece != EMPTY) {

                        piecesLeft += symbol;

                        if (symbol == 'B') {
                            bishops[0][0] = row;
                            bishops[0][1] = col;
                        }

                        else if (symbol == 'b') {
                            bishops[1][0] = row;
                            bishops[1][1] = col;
                        }
                    }
                }
            }

            sort(piecesLeft.begin(), piecesLeft.end());

            for (int i = 0; i < 6; i++) {

                if (piecesLeft.compare(drawCases[i]) == 0) {
                    if (i == 5) {

                        //Check if both bishops are on the same color
                        if (isDarkSquare(bishops[0][0], bishops[0][1]) == isDarkSquare(bishops[1][0], bishops[1][1])) {
                            return material;
                        }
                    }
                    else {
                        return material;
                    }
                }
            }

            STALEMATE:
            /*** Stalemate (If there are no legal moves) ***
            if (bestMoves().size() == 0) {
                return stalemate;
            }

            /*** 50 move rule ***
            return this->getHalfmoves() >= 100;

            */
            return notDraw;
        }

        /*------------------------------------END Game Rules-------------------------------------------*/

        /*------------------------------------Move Generation------------------------------------------*/

        vector<string> generateKingMoves(); 
        vector<string> generatePawnMoves();
        vector<string> generateKnightMoves();
        vector<string> generateBishopMoves();
        vector<string> generateRookMoves();
        vector<string> generateQueenMoves();
        
        vector<string> bestMoves() {
            vector<string> moves = vector<string>();
            return moves;
        }

        void move (int from, int to, int flag) {

            int fromX = from % 10;
            int fromY = from / 10;

            int toX = to % 10;
            int toY = to / 10;

            if (flag )
            
        }

        /*------------------------------------END Move Generation----------------------------------------*/

        //The evaluation of the current position
        int evaluation (int depth) {
            
            int eval = 0;

            if (this->isDraw()) {
                return eval;
            }

            for (int i = 0; i < depth; i++) {

            }

            return 0;
        }
};

