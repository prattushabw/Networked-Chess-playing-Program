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

int send_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    if (!game || !message) return COMMAND_ERROR;

    if (strncmp(message, "/move ", 6) == 0) {
        ChessMove move;
        if (parse_move(message + 6, &move) == 0) {
            if (make_move(game, &move, is_client, true) == 0) {
                send(socketfd, message, strlen(message), 0); 
                return COMMAND_MOVE;
            }
        }
        return COMMAND_ERROR;
    } else if (strcmp(message, "/forfeit") == 0) {
        send(socketfd, message, strlen(message), 0); 
        return COMMAND_FORFEIT;
    } else if (strcmp(message, "/chessboard") == 0 && !is_client) {
        display_chessboard(game); 
        return COMMAND_DISPLAY;
    } else if (strncmp(message, "/import ", 8) == 0 && !is_client) {
        fen_to_chessboard(message + 8, game);
        send(socketfd, message, strlen(message), 0);  
        return COMMAND_IMPORT;
    } else if (strncmp(message, "/save", 5) == 0) {

        if (save_game(game, message + 6, "./game_database.txt") == 0) {
            send(socketfd, message, strlen(message), 0);
            return COMMAND_SAVE;
        }
        return COMMAND_ERROR;
    } else if (strncmp(message, "/load ", 6) == 0 && is_client) {
        return COMMAND_UNKNOWN;  
    }
    return COMMAND_UNKNOWN;
}

int receive_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    if (!game || !message) return COMMAND_ERROR;

    const char *response = "";

    if (strncmp(message, "/move ", 6) == 0) {
        ChessMove move;
        if (parse_move(message + 6, &move) == 0 && make_move(game, &move, is_client, false) == 0) {
            response = "Move accepted";
            return COMMAND_MOVE;
        }
        response = "Invalid move";
        return COMMAND_ERROR;
    } else if (strcmp(message, "/forfeit") == 0) {
        response = "Forfeit acknowledged";
        close(socketfd);  
        return COMMAND_FORFEIT;
    } else if (strncmp(message, "/import", 7) == 0 && is_client) {
        fen_to_chessboard(message + 8, game);
        return COMMAND_IMPORT;
    } else if (!is_client && strncmp(message, "/load ", 6) == 0) {
        char username[64]; 
        int save_number;
        if (sscanf(message + 6, "%63s %d", username, &save_number) == 2) {
            if (load_game(game, username, "game_database.txt", save_number) == 0) {
                return COMMAND_LOAD;
            } else {
                return COMMAND_ERROR;
            }
        } else {
            return COMMAND_ERROR;
        }
    } else {
        response = "Unknown command";
        return COMMAND_UNKNOWN;
    }

    if (socketfd >= 0) {
        send(socketfd, response, strlen(response), 0);
    }
}

bool is_valid_username(const char *username) {
    if (username == NULL || *username == '\0') return false;
    for (; *username; ++username) {
        if (*username == ' ') return false;
    }
    return true;
}

int save_game(ChessGame *game, const char *username, const char *db_filename) {
    if (!is_valid_username(username)) {
        return -1; 
    }
    FILE *file = fopen(db_filename, "a"); 
    if (!file) {
        return -1;
    }

    char fen[BUFFER_SIZE];
    chessboard_to_fen(fen, game); 

    if (fprintf(file, "%s:%s\n", username, fen) < 0) {
        fclose(file); 
        return -1; 
    }

    fclose(file); 
    return 0; 
}

int load_game(ChessGame *game, const char *username, const char *db_filename, int save_number) {
    FILE *file = fopen(db_filename, "r");
    if (!file) {
        return -1; 
    }
    char line[BUFFER_SIZE];
    int found = 0;
    int count = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        char *file_username = strtok(line, ":");
        char *fen = strtok(NULL, "\n");
        
        if (file_username && strcmp(file_username, username) == 0) {
            ++count;
            if (count == save_number) {
                fen_to_chessboard(fen, game); 
                found = 1;
                break;
            }
        }
    }

    fclose(file);
    return found ? 0 : -1; 
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
