//
// Created by alexanderfisher on 08/03/25.
//

#ifndef TOKEN_H
#define TOKEN_H


// Token Type:
typedef enum {
    SYMBOL,            // Labels, variables, and predefined symbols (e.g. LOOP, i, count, SP, THAT)
    KEYWORD,           // Hack instruction keywords (e.g. A, D, M, JGT, JMP)
    INTEGER_LITERAL,   // Numeric constants (e.g. 0, 1, 15, 32767)
    OPERATOR,          // Operators used in computation (e.g. @ =, +, -, !, &, |)
    SEPARATOR,         // Separators in instructions (e.g. ;, (, ))
    NEWLINE,           // End of a Hack instruction
    INVALID            // Invalid token
} TokenType;

typedef enum {
    KW_A,               // 'A' register (Address register)
    KW_D,               // 'D' register (Data register)
    KW_M,               // 'M' register (RAM[A], memory access)
    KW_JGT,             // Jump if greater than zero
    KW_JEQ,             // Jump if equal to zero
    KW_JGE,             // Jump if greater than or equal to zero
    KW_JLT,             // Jump if less than zero
    KW_JNE,             // Jump if not equal to zero
    KW_JLE,             // Jump if less than or equal to zero
    KW_JMP              // Unconditional jump
} Keyword;

typedef enum {
    OP_AT,              // '@'  (A-instruction prefix: e.g., @100, @LOOP)
    OP_ASSIGN,          // '='  (Assignment in C-instruction: e.g., D=M+1)
    OP_ADD,             // '+'  (Addition: e.g., D=A+1)
    OP_SUB,             // '-'  (Subtraction/Negation: e.g., D=D-1, M=-1)
    OP_NOT,             // '!'  (Bitwise NOT: e.g., D=!A)
    OP_AND,             // '&'  (Bitwise AND: e.g., D=D&A)
    OP_OR               // '|'  (Bitwise OR: e.g., D=D|M)
} Operator;

typedef enum {
    SEP_SEMICOLON,  // ';' (Separates `comp` and `jump` in C-instructions: e.g., `D;JGT`)
    SEP_LPAREN,     // '('  (Label declaration start: e.g., `(LOOP)`)
    SEP_RPAREN     // ')'  (Label declaration end)
} Separator;


// Token Data:
typedef union {
    Keyword keyword;      // Stores a Hack keyword (A, D, M, JGT, etc.)
    Operator operator;    // Stores an operator (@ =, +, -, &, |, !)
    Separator separator;  // Stores a separator (;, (, ))
    int integer;          // Stores an integer literal (e.g., 0, 1, 15, 32767)
    char *symbol;         // Stores a symbol (e.g., LOOP, count, variable names)
} TokenData;

// Token
typedef struct Token {
    TokenType type;
    TokenData value;
} Token;

// Mapping enums to human-readable strings:
const char *token_type_to_str(TokenType type);
const char *keyword_to_str(Keyword keyword);
const char *operator_to_str(Operator op);
const char *separator_to_str(Separator sep);

// Token *create_token(TokenType type, void *value);
Token *create_token(TokenType type, ...);
void free_token(Token *token);
char *token_to_str(const Token *token);



#endif //TOKEN_H
