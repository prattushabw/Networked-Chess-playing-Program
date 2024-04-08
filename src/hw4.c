#include "hw4.h"
// #include <string.h> // Include for memset
// #include <ctype.h>  // Include for isdigit

void initialize_game(ChessGame *game) {
    game->moveCount = 0;
    game->capturedCount = 0;
    
    game->currentPlayer = WHITE_PLAYER;
    
    
    const char *initialRows[8] = {
        "rnbqkbnr", 
        "pppppppp", 
        "........", 
        "........",
        "........",
        "........",
        "PPPPPPPP", 
        "RNBQKBNR"  
    };
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            game->chessboard[i][j] = initialRows[i][j];
        }
    }
}

void chessboard_to_fen(char fen[], ChessGame *game) {
    int k=0;
    for (int i = 0; i < 8; i++) {
        int emptyCount=0;
        for (int j = 0; j < 8; j++) {
            if (game->chessboard[i][j] =='.'){
                emptyCount++;
            }else{
                if(emptyCount!=0){
                    fen[k++]='0'+emptyCount;
                    emptyCount=0;
                }
                fen[k++]=game->chessboard[i][j];
            }
        }
        if (emptyCount!=0){
             fen[k++] = '0' + emptyCount;
        }
        if (i < 7) {
            fen[k++] = '/';
        }
    }
    fen[k++] = ' ';
    fen[k++] = game->currentPlayer == WHITE_PLAYER ? 'w' : 'b';
  
    fen[k] = '\0';

}

bool is_valid_pawn_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    int moveDirection = (piece == 'P') ? -1 : 1; 
    char targetPiece = game->chessboard[dest_row][dest_col];

    if (src_col == dest_col && dest_row == src_row + moveDirection && targetPiece == '.') {
        return true;
    }

    if (src_col == dest_col && ((piece == 'P' && src_row == 6 && dest_row == 4) || (piece == 'p' && src_row == 1 && dest_row == 3)) && targetPiece == '.' && game->chessboard[src_row + moveDirection][src_col] == '.') {
        return true;
    }

    if (abs(dest_col - src_col) == 1 && dest_row == src_row + moveDirection && targetPiece != '.') {
        if (piece == 'P' && islower(targetPiece)) { 
            return true;
        } else if (piece == 'p' && isupper(targetPiece)) { 
            return true;
        }
    }

    return false; 
    return false;
}

bool is_valid_rook_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if (src_row != dest_row && src_col != dest_col) {
        return false;
    }

    int rowDirection = 0;
    int colDirection = 0;
    
    
    if (dest_row > src_row) {
        rowDirection = 1; //  down the board
    } else if (dest_row < src_row) {
        rowDirection = -1; //  up the board
    }
    if (dest_col > src_col) {
        colDirection = 1; //  right on the board
    } else if (dest_col < src_col) {
        colDirection = -1; //  left on the board
    }
    
    int currentRow = src_row + rowDirection;
    int currentCol = src_col + colDirection;
    
    while ((currentRow != dest_row) || (currentCol != dest_col)) {
        char pieceAtCurrent = game->chessboard[currentRow][currentCol];
        if (pieceAtCurrent != '.') {
            if ((currentRow == dest_row) && (currentCol == dest_col)) {
                if ((isupper(pieceAtCurrent) && islower(game->chessboard[src_row][src_col])) || 
                    (islower(pieceAtCurrent) && isupper(game->chessboard[src_row][src_col]))) {
                    return true;
                }
                return false;
            } else {
                return false;
            }
        }
        currentRow += rowDirection;
        currentCol += colDirection;
    }
    return true;
   
}

bool is_valid_knight_move(int src_row, int src_col, int dest_row, int dest_col) {
    int row_diff = abs(dest_row - src_row);
    int col_diff = abs(dest_col - src_col);

    bool isValidLMove = (row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2);

    if (!isValidLMove) {
        return false;
    }

    return true;
}

bool is_valid_bishop_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if (abs(dest_row - src_row) != abs(dest_col - src_col)) {
        return false; 
    }

    int rowDirection = (dest_row > src_row) ? 1 : -1;
    int colDirection = (dest_col > src_col) ? 1 : -1;

    int currentRow, currentCol;
    for (currentRow = src_row + rowDirection, currentCol = src_col + colDirection;
         currentRow != dest_row;
         currentRow += rowDirection, currentCol += colDirection) {

        if (game->chessboard[currentRow][currentCol] != '.') {
            return false;
        }
    }

 
    char destPiece = game->chessboard[dest_row][dest_col];
    char srcPiece = game->chessboard[src_row][src_col];
    if (destPiece != '.') {
        if ((isupper(srcPiece) && isupper(destPiece)) || (islower(srcPiece) && islower(destPiece))) {
            return false;
        }
    }

    return true; 
}

bool is_valid_queen_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if (src_row == dest_row || src_col == dest_col) {
        return is_valid_rook_move(src_row, src_col, dest_row, dest_col, game);
    }
    else if (abs(dest_row - src_row) == abs(dest_col - src_col)) {
        return is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game);
    }
  
    return false;
}

bool is_valid_king_move(int src_row, int src_col, int dest_row, int dest_col) {
    int row_diff = abs(dest_row - src_row);
    int col_diff = abs(dest_col - src_col);

    return row_diff <= 1 && col_diff <= 1;
}

bool is_valid_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    char piece_type = tolower(piece);

    switch (piece_type) {
        case 'p': 
            return is_valid_pawn_move(piece,src_row, src_col, dest_row, dest_col, game);
        case 'r': 
            return is_valid_rook_move(src_row, src_col, dest_row, dest_col, game);
        case 'n': 
            return is_valid_knight_move(src_row, src_col, dest_row, dest_col);
        case 'b': 
            return is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game);
        case 'q': 
            return is_valid_queen_move(src_row, src_col, dest_row, dest_col, game);
        case 'k': 
            return is_valid_king_move(src_row, src_col, dest_row, dest_col);
        default: 
            return false;
    }
}

void fen_to_chessboard(const char *fen, ChessGame *game) {
    int row=0;
    int col=0;
    memset(game->chessboard,'.',sizeof(game->chessboard));
    game->moveCount=0;
    game->capturedCount=0;

    for(const char *p=fen; *p; ++p){
        if(*p==' '){
            if((*++p)=='w'){
                game->currentPlayer=0;
            }else{
                game->currentPlayer=1;
            }
        }else if(isdigit(*p)){
            int emptySquares = *p - '0'; //turn the char into a number
            col += emptySquares;
        }else if(*p=='/'){
            row++;
            col=0;
        }else{
            game->chessboard[row][col++] = *p;
        }
    }
}

int parse_move(const char *move, ChessMove *parsed_move) {
    int len = strlen(move);
    if (len != 4 && len != 5) {
        return PARSE_MOVE_INVALID_FORMAT;
    }
    
    if (move[0] < 'a' || move[0] > 'h' || move[2] < 'a' || move[2] > 'h') {
        return PARSE_MOVE_INVALID_FORMAT;
    }

    if (move[0] < 'a' || move[0] > 'h' || move[2] < 'a' || move[2] > 'h' ||
        move[1] < '1' || move[1] > '8' || move[3] < '1' || move[3] > '8') {
        return PARSE_MOVE_OUT_OF_BOUNDS;
    }

    strncpy(parsed_move->startSquare, move, 2);
    parsed_move->startSquare[2] = '\0';  
    strncpy(parsed_move->endSquare, move + 2, len == 5 ? 3 : 2);
    if (len == 5) {
        parsed_move->endSquare[3] = '\0';  
    } else {
        parsed_move->endSquare[2] = '\0';  
    }

    if (len == 5) {
        if ((move[1] == '7' && move[3] == '8') || (move[1] == '2' && move[3] == '1')) {
            if (move[4] != 'q' && move[4] != 'r' && move[4] != 'b' && move[4] != 'n') {
                return PARSE_MOVE_INVALID_PROMOTION;
            }
        } else {
            return PARSE_MOVE_INVALID_DESTINATION;
        }
    }
    return 0;  
}

int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move) {
    int startRow = 8 - (move->startSquare[1] - '1') - 1;
    int startCol = move->startSquare[0] - 'a';
    int endRow = 8 - (move->endSquare[1] - '1') - 1;
    int endCol = move->endSquare[0] - 'a';
    char piece = game->chessboard[startRow][startCol];
    char targetPiece;

    if (validate_move) {
        if ((is_client && game->currentPlayer != WHITE_PLAYER) || (!is_client && game->currentPlayer != BLACK_PLAYER)) {
            return MOVE_OUT_OF_TURN;
        }

        if (piece == '.') {
            return MOVE_NOTHING;
        }

        bool isPieceWhite = isupper(piece);
        if ((isPieceWhite && !is_client) || (!isPieceWhite && is_client)) {
            return MOVE_WRONG_COLOR;
        }

        targetPiece = game->chessboard[endRow][endCol];
        if ((isupper(targetPiece) && isPieceWhite) || (islower(targetPiece) && !isPieceWhite)) {
            return MOVE_SUS;
        }

        bool isPromotion = strlen(move->endSquare) == 3;
        if (isPromotion) {
            if (tolower(piece) != 'p' || ((isPieceWhite && endRow != 0) || (!isPieceWhite && endRow != 7))) {
                return MOVE_NOT_A_PAWN;
            }
        } else {
            if ((tolower(piece) == 'p') && ((isPieceWhite && endRow == 0) || (!isPieceWhite && endRow == 7))) {
                return MOVE_MISSING_PROMOTION;
            }
        }

        if (!is_valid_move(piece, startRow, startCol, endRow, endCol, game)) {
            return MOVE_WRONG;
        }
    }

    game->chessboard[endRow][endCol] = game->chessboard[startRow][startCol];
    game->chessboard[startRow][startCol] = '.';

    if (strlen(move->endSquare) == 3) {
        char promotedPiece = move->endSquare[2];
        game->chessboard[endRow][endCol] = is_client ? toupper(promotedPiece) : tolower(promotedPiece);
    }

    game->moves[game->moveCount++] = *move; 
    if (targetPiece != '.') {
        game->capturedPieces[game->capturedCount++] = targetPiece;
    }
    game->currentPlayer = (game->currentPlayer == WHITE_PLAYER) ? BLACK_PLAYER : WHITE_PLAYER;

    return 0; 
}


void display_chessboard(ChessGame *game) {
    printf("\nChessboard:\n");
    printf("  a b c d e f g h\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < 8; j++) {
            printf("%c ", game->chessboard[i][j]);
        }
        printf("%d\n", 8 - i);
    }
    printf("  a b c d e f g h\n");
}
