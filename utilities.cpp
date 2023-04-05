#include "board.cpp"

void chessBubbleText () {
    cout << "\t    _______   __    __   _______   _______   _______" << endl;
    cout << "\t   /  _____| |  |  |  | |   ____| |  _____| |  _____|" << endl;
    cout << "\t  |  |       |  |__|  | |  |__    | |_____  | |_____" << endl;
    cout << "\t  |  |       |   __   | |   __|   |_____  | |_____  |" << endl;
    cout << "\t  |  |_____  |  |  |  | |  |____   ____|  |  ____|  |" << endl;
    cout << "\t   \\_______| |__|  |__| |_______| |_______| |_______|" << endl;
    cout << endl;
    cout << "+--------+----------+---------+---------+---------+---------+---------+" << endl;
}

//Provides a visual for a chessboard and prints it to the console
void visualize (ChessBoard board) {

    chessBubbleText();
    cout << "                                        |\n";

    bool isWhite = board.whiteTurn();

    //displaying the ranks on the left side of the board
    char rank = isWhite ? '8' : '1';

    //For displaying the pieces captured
    int currentPieces[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};

    //printing the borders and the pieces of the board
    cout << "  +---+---+---+---+---+---+---+---+\t|" << endl;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {

            if (isWhite && col == 0) {
                cout << rank;
                rank--;
            }
            else if (!isWhite && col == 0) {
                cout << rank;
                rank++;
            }
            
            cout << " ";
            cout << "| ";

            Piece piece = EMPTY;

            if (isWhite) {
                piece = board.pieceAt(row, col);
            }
            else {
                piece = board.pieceAt(7 - row, 7 - col);
            }
            cout << pieceToSymbol(piece);

            if (piece >= 1 && piece <= 12) {
                currentPieces[piece]++;
            }
        }
        cout << " |\t|";
        if (row == 0) {
            cout << (isWhite ? "  White's turn to move." : "  Black's turn to move.") << endl;
        }
        else if (row == 1) {
            if (board.inCheck()) {
                cout << "  Your king is in check!" << endl;
            }
            else if (board.isDraw()[0]) {
                cout << "  Draw by " << board.drawCases(board.isDraw()[1]) << endl;
            }
            else {
                cout << "  Your king is currently safe." << endl;
            }
        }
        else if (row == 2) {
            int rowHere = board.getEPTarget()[0];
            int colHere = board.getEPTarget()[1];
            if (rowHere >= 0) {
                cout << "  If possible, you may perform an En Passant capture on ";
                char file = colHere + 97;
                char rank = 8 - (rowHere - '0');
                cout << file;
                cout << rank;
                cout << "." << endl;
            }
            else {
                cout << "  There are currently no En Passant moves available." << endl;
            }
        }
        else if (row == 3) {
            int startPieces[13] = {0, 8, 2, 2, 2, 1, 1, 8, 2, 2, 2, 1, 1};
            
            cout << "  Pieces captured: ";
            for (int i = (isWhite ? 7 : 1); i < (isWhite ? 12 : 6); i++) {
                for (int j = 0; j < startPieces[i] - currentPieces[i]; j++) {
                    cout << pieceToSymbol((Piece)i);
                }
                cout << " ";
            }
            cout << endl;

        }
        else if (row == 6) {
            int h = board.getHalfmoves() / 2;
            if (h >= 40) {
                cout << "  NOTE: It has been ";
                cout << h;
                cout << " moves since the last pawn move or piece capture.";
            }
            cout << endl;
        }
        else {
            cout << endl;
        }
        cout << "  +---+---+---+---+---+---+---+---+\t|" << endl;
    } 

    //displaying the files on the bottom of the board
    if (isWhite) {
        cout << "    a   b   c   d   e   f   g   h       |" << endl;
    }
    else {
        cout << "    h   g   f   e   d   c   b   a       |" << endl;
    }

    cout << "                                        |\n";
    cout << "+--------+----------+---------+---------+---------+---------+---------+" << endl;
}

