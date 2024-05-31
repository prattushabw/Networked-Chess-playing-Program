# Project: Networked Chess-playing Program

## Overview
This project involves implementing a networked Chess-playing program where one player starts a server, and another player connects as the client. They play until one player forfeits. The project focuses on message processing and updating the game state.

## Chess Rules Implemented
- Basic Chess movements excluding en passant, castling, checking, and checkmating.
- Move format: `[piece_old_location][piece_new_location]`
- FEN string generation and parsing for game state representation.

## Key Functions

### 1. Initialize the Game State
```c
void initialize_game(ChessGame *game);
```
### Validating Piece Movement
Functions to validate movement of different Chess pieces:
- bool is_valid_pawn_move(char piece, int src_row, int src_col, int dest_row, ChessGame *game);
- bool is_valid_rook_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game);
- bool is_valid_knight_move(int src_row, int src_col, int dest_row, int dest_col);
- bool is_valid_bishop_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game);
- bool is_valid_queen_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game);
- bool is_valid_king_move(int src_row, int src_col, int dest_row, int dest_col);
- bool is_valid_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game);

### Parsing Movement Strings
Parses a string representing a Chess move and modifies the ChessMove structure accordingly:
- int parse_move(const char *str, ChessMove *move);

### Making a Move
Updates the game state based on the move specified in the ChessMove structure:
- int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move);

### FEN String Generation
Generates a FEN (Forsyth-Edwards Notation) string to represent the current state of the Chess board:
- void chessboard_to_fen(char fen[], ChessGame *game);

### FEN String Parsing
Parses a given FEN string and updates the game state accordingly:
- void fen_to_chessboard(const char *fen, ChessGame *game);

### Save & Load Games
Functions to save and load the game state to and from a file-based database:
int save_game(ChessGame *game, const char *username, const char *db_filename);
int load_game(ChessGame *game, const char *username, char *db_filename, int save_number);

### Send and Receive Messages
Functions to handle communication between client and server by sending and receiving commands:
int send_command(ChessGame *game, const char *message, int socketfd, bool is_client);
int receive_command(ChessGame *game, const char *message, int socketfd, bool is_client);

## Getting Started
To get started, follow these steps:

- Clone the repository to your local machine.
- Initialize the game state using initialize_game.
- Use the functions provided to play the game, validate moves, and manage the game state.
- Save and load games using save_game and load_game.
- Handle communication between client and server using send_command and receive_command.
