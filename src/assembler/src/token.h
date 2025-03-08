//
// Created by alexanderfisher on 08/03/25.
//

#ifndef TOKEN_H
#define TOKEN_H

// Token Type:
typedef enum {
  // A-instruction
  AT,                      // @
  INTEGER_LITERAL,         // value (e.g. 0, 1, 5, 10, 1463)
  SYMBOL,                  // Variable or label (e.g. LOOP, count)

  // C-instruction
  DEST,                    // Destination register (e.g. 'D=', 'M=')
  COMP,                    // Computation (e.g. 'D+M', 'A-1')
  JUMP,                    // Jump condition (e.g. ';JMP', ';JGT')

  // Label
  L_PAREN,                 // (
  R_PAREN,                 // )

  // Operator and Separtor
  EQUALS,                  // =
  SEMICOLON,               // ;

  // Other
  COMMENT,                 // '// this is a comment'
  NEWLINE,                 // newline character
  INVALID                  // Unrecognised token
} token_type;

typedef enum {

}







#endif //TOKEN_H
