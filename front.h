#ifndef FRONT_H
#define FRONT_H

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define ASSIGN_OP 2
#define LESSER_OP 4
#define GREATER_OP 6
#define EQUAL_OP 8
#define NEQUAL_OP 10
#define LEQUAL_OP 12
#define GEQUAL_OP 14
#define LEFT_PAREN 16
#define RIGHT_PAREN 18
#define ADD_OP 20
#define SUB_OP 22
#define MULT_OP 24
#define DIV_OP 26
#define POW_OP 28
#define INC_OP 30
#define DEC_OP 32
#define SEMICOLON 34
#define COLON 36
#define KEY_READ 38
#define KEY_PRINT 40
#define KEY_IF 42
#define KEY_ELSE 44
#define KEY_BEGIN 46
#define KEY_END 48
#define IDENT 50
#define INT_LIT 52




int lex();
extern int lineNum;

#endif
