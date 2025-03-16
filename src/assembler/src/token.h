//
// Created by alexanderfisher on 08/03/25.
//
#ifndef TOKEN_H
#define TOKEN_H

// Enum representing different token types
typedef enum {
    // A-instruction tokens
    TOKEN_AT,               // @
    TOKEN_SYMBOL,           // Variable or label reference (e.g., @var)
    TOKEN_INTEGER,          // Numeric address (e.g., @10)

    // L-instruction tokens
    TOKEN_LPAREN,           // (
    TOKEN_RPAREN,           // )

    // C-instruction tokens
    // Destinations
    TOKEN_DEST_NULL,        // null
    TOKEN_DEST_M,           // M=
    TOKEN_DEST_D,           // D=
    TOKEN_DEST_MD,          // MD=
    TOKEN_DEST_A,           // A=
    TOKEN_DEST_AM,          // AM=
    TOKEN_DEST_AD,          // AD=
    TOKEN_DEST_AMD,         // AMD=

    // Computation mnemonics
    TOKEN_COMP_0,           // 0
    TOKEN_COMP_1,           // 1
    TOKEN_COMP_NEG1,        // -1
    TOKEN_COMP_D,           // D
    TOKEN_COMP_A,           // A
    TOKEN_COMP_M,           // M
    TOKEN_COMP_NOT_D,       // !D
    TOKEN_COMP_NOT_A,       // !A
    TOKEN_COMP_NOT_M,       // !M
    TOKEN_COMP_NEG_D,       // -D
    TOKEN_COMP_NEG_A,       // -A
    TOKEN_COMP_NEG_M,       // -M
    TOKEN_COMP_DPLUS1,      // D+1
    TOKEN_COMP_APLUS1,      // A+1
    TOKEN_COMP_MPLUS1,      // M+1
    TOKEN_COMP_DMINUS1,     // D-1
    TOKEN_COMP_AMINUS1,     // A-1
    TOKEN_COMP_MMINUS1,     // M-1
    TOKEN_COMP_DPLUSA,      // D+A
    TOKEN_COMP_DPLUSM,      // D+M
    TOKEN_COMP_DMINUSA,     // D-A
    TOKEN_COMP_DMINUSM,     // D-M
    TOKEN_COMP_AMINUSD,     // A-D
    TOKEN_COMP_MMINUSD,     // M-D
    TOKEN_COMP_DANDA,       // D&A
    TOKEN_COMP_DANDM,       // D&M
    TOKEN_COMP_DORA,        // D|A
    TOKEN_COMP_DORM,        // D|M

    // Jump mnemonics
    TOKEN_JUMP_NULL,        // null
    TOKEN_JUMP_JGT,         // ;JGT
    TOKEN_JUMP_JEQ,         // ;JEQ
    TOKEN_JUMP_JGE,         // ;JGE
    TOKEN_JUMP_JLT,         // ;JLT
    TOKEN_JUMP_JNE,         // ;JNE
    TOKEN_JUMP_JLE,         // ;JLE
    TOKEN_JUMP_JMP,         // ;JMP

    // Separators
    TOKEN_EQUALS,           // =
    TOKEN_SEMICOLON,        // ;

    // End-of-file or invalid token or newline
    TOKEN_EOF,              // End of file
    TOKEN_INVALID,          // Unrecognized token
    NEWLINE                 // Newline
} TokenType;


// Union to store only necessary values
typedef union {
    int integer;    // Stores an integer literal (e.g., @10)
    char *symbol;   // Stores a symbol (e.g., LOOP, count, var)
} TokenValue;


// Token structure
typedef struct {
    TokenType type;
    TokenValue value;  // Only used for TOKEN_SYMBOL and TOKEN_INTEGER
} Token;


// Token *create_token(TokenType type, void *value);
Token *create_token(TokenType type, ...);


void free_token(Token *token);


char *token_to_str(const Token *token);

#endif //TOKEN_H
