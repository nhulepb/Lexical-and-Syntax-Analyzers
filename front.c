/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "front.h"
#include "parser.h"

/* Global Variable */
int nextToken;
char lexeme[100];
int lineNum = 1;

/* Local Variables */
static int charClass;
static char nextChar;
static int lexLen;
static FILE *in_fp;

/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();
static void getTokenName();

/******************************************************/
/* main */
int main(int argc, char *argv[]) {
    printf("Cooke Parser :: R11834468\n");
    /* Open the input data file and process its contents */
    if (argc < 2) {
        fprintf(stderr, "Error: No input file provided.\n");
        return 2;
    }

    // Case: File does not exist or cannot be opened
    if ((in_fp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Error: Cannot open input file %s\n", argv[1]);
        return 3;
    }
 
    getChar(); // Initialize character reading
    lex();     // Get the first token
    program();
    while (nextToken != EOF);
    fclose(in_fp);
    return 0;
} 

/******************************************************/
/* Error Reporting Function */
void error() {
    printf("Error encountered on line %d: The next lexeme was \"%s\" and the next token was ", lineNum, lexeme);
    getTokenName();
    exit(1); // Return exit code 1 for error
}

/* getTokenName - a function to get uppercase name of the token */
static void getTokenName() {
    if (nextToken == ASSIGN_OP) {
    printf("ASSIGN_OP\n");
    } else if (nextToken == LESSER_OP) {
        printf("LESSER_OP\n");
    } else if (nextToken == GREATER_OP) {
        printf("GREATER_OP\n");
    } else if (nextToken == EQUAL_OP) {
        printf("EQUAL_OP\n");
    } else if (nextToken == NEQUAL_OP) {
        printf("NEQUAL_OP\n");
    } else if (nextToken == LEQUAL_OP) {
        printf("LEQUAL_OP\n");
    } else if (nextToken == GEQUAL_OP) {
        printf("GEQUAL_OP\n");
    } else if (nextToken == LEFT_PAREN) {
        printf("LEFT_PAREN\n");
    } else if (nextToken == RIGHT_PAREN) {
        printf("RIGHT_PAREN\n");
    } else if (nextToken == ADD_OP) {
        printf("ADD_OP\n");
    } else if (nextToken == SUB_OP) {
        printf("SUB_OP\n");
    } else if (nextToken == MULT_OP) {
        printf("MULT_OP\n");
    } else if (nextToken == DIV_OP) {
        printf("DIV_OP\n");
    } else if (nextToken == POW_OP) {
        printf("POW_OP\n");
    } else if (nextToken == INC_OP) {
        printf("INC_OP\n");
    } else if (nextToken == DEC_OP) {
        printf("DEC_OP\n");
    } else if (nextToken == SEMICOLON) {
        printf("SEMICOLON\n");
    } else if (nextToken == COLON) {
        printf("COLON\n");
    } else if (nextToken == KEY_READ) {
        printf("KEY_READ\n");
    } else if (nextToken == KEY_PRINT) {
        printf("KEY_PRINT\n");
    } else if (nextToken == KEY_IF) {
        printf("KEY_IF\n");
    } else if (nextToken == KEY_ELSE) {
        printf("KEY_ELSE\n");
    } else if (nextToken == KEY_BEGIN) {
        printf("KEY_BEGIN\n");
    } else if (nextToken == KEY_END) {
        printf("KEY_END\n");
    } else if (nextToken == IDENT) {
        printf("IDENT\n");
    } else if (nextToken == INT_LIT) {
        printf("INT_LIT\n");
    } else if (nextToken == UNKNOWN) {
        printf("UNKNOWN\n");
    } else if (nextToken == EOF) {
        printf("EOF\n");
    }
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the token */
static int lookup(char ch) {
    switch (ch) {
        case ':':
            addChar();
            getChar ();
            if (nextChar == '=') {
                addChar ();
                nextToken = ASSIGN_OP;
                getChar ();
            } else {
                nextToken = COLON;
            }
            break;
        case '>':
            addChar();
            getChar ();
            if (nextChar == '=') {
                addChar ();
                nextToken = GEQUAL_OP;
                getChar ();
            } else {
                nextToken = GREATER_OP;
            }
            break;
        case '=':
            addChar();
            nextToken = EQUAL_OP;
            getChar ();
            break;
        case '<':
            addChar();
            getChar ();
            if (nextChar == '>') {
                addChar ();
                nextToken = NEQUAL_OP;
                getChar ();
            } else if (nextChar == '=') {
                addChar ();
                nextToken = LEQUAL_OP;
                getChar ();
            } else {
                nextToken = LESSER_OP;
            }  
            break;
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            getChar ();
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            getChar ();
            break;
        case '+':
            addChar();
            getChar ();
            if (nextChar == '+') {
                addChar ();
                nextToken = INC_OP;
                getChar ();
            } else {
                nextToken = ADD_OP;
            }
            break;
        case '-':
            addChar();
            getChar ();
            if (nextChar == '-') {
                addChar ();
                nextToken = DEC_OP;
                getChar ();
            } else {
                nextToken = SUB_OP;
            }
            break;
        case '*':
            addChar();
            getChar ();
            if (nextChar == '*') {
                addChar ();
                nextToken = POW_OP;
                getChar ();
            } else {
                nextToken = MULT_OP;
            }
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            getChar ();
            break;
        case ';':
            addChar();
            nextToken = SEMICOLON;
            getChar ();
            break;
        default:
            addChar();
            nextToken = UNKNOWN;
            getChar ();
            break;
    }
    return nextToken;
}

/******************************************************/
/* Parsing Functions */
/* P ::= S */
void program() {
    statement();
     if (nextToken != EOF) {
        error();  // If we don't encounter EOF, it's an error
    } else {
        printf("Syntax Validated\n");
        exit(0);  // Successful parsing, exit with 0
    }
}

/* S ::= V := E | O | read(V) | print(E) | if C: begin S end | if C: begin S else: S end | S ; S */
void statement() {
    // Parse statements while valid tokens are found
    while (nextToken == IDENT || nextToken == KEY_READ || nextToken == KEY_PRINT || nextToken == KEY_IF ||
           nextToken == INC_OP || nextToken == DEC_OP) {

        if (nextToken == IDENT) {
            variable(); //parse v
            if (nextToken == ASSIGN_OP) {
                lex(); //consume
                expr();
            } else {
                error(); 
            }

        } else if (nextToken == INC_OP || nextToken == DEC_OP) {
            operation();

        } else if (nextToken == KEY_READ) {
            lex();//consume
            if (nextToken == LEFT_PAREN) {
                lex();
                variable();
                if (nextToken == RIGHT_PAREN) {
                    lex();
                } else {
                    error();
                }
            } else {
                error(); 
            }

        } else if (nextToken == KEY_PRINT) {
            lex();//consume
            if (nextToken == LEFT_PAREN) {
                lex();
                expr();
                if (nextToken == RIGHT_PAREN) {
                    lex();
                } else {
                    error();
                }
            } else {
                error();
            }
        } else if (nextToken == KEY_IF) {
            lex(); // consume "if"
            condition();
            if (nextToken == COLON) {
                lex();
                if (nextToken == KEY_BEGIN) {
                    lex();
                    statement();  

                    if (nextToken == KEY_END) {
                        lex(); // consume 
                    } else if (nextToken == KEY_ELSE) {
                        lex(); 
                        if (nextToken == COLON) {
                            lex(); 
                            statement(); 

                            if (nextToken == KEY_END) {
                                lex(); 
                            } else {
                                error();
                            }
                        } else {
                            error();
                        }
                    } else {
                        error();
                    }
                } else {
                    error();
                }
            } else {
                error();
            }
        }
        if (nextToken == SEMICOLON) {
            statement();
            lex(); // consume ;
            statement();
            continue; // parse next statement
        } else {
            break; // stop if no semicolon
        }
    }
}

/* C ::= E < E | E > E | E = E | E <> E | E <= E | E >= E */
void condition() {
    expr();
    if (nextToken == LESSER_OP || nextToken == GREATER_OP || nextToken == EQUAL_OP || nextToken == NEQUAL_OP || nextToken == LEQUAL_OP || nextToken == GEQUAL_OP) {
        //expr();
        lex();
        expr();
    } else {
        error();
    }
}

/* E ::= T | E + T | E - T */ /*left recursion*/
void expr() {
    term();
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex(); // Process operator
        term();
    }
}

/* T ::= F | T * F | T / F | T ** F */
void term() {
    factor();
    while (nextToken == MULT_OP || nextToken == DIV_OP || nextToken == POW_OP) {
        lex(); // Process operator
        factor();
    }
}

/* F ::= (E) | N | V */
void factor() {
    if (nextToken == LEFT_PAREN) {
        lex();
        expr();
        if (nextToken == RIGHT_PAREN) {
            lex();
        } else {
            error();
        }
    } else if (nextToken == INT_LIT) {
        lex();
    } else if (nextToken == IDENT) {
        variable();
    } else {
        error ();
    }
}

/* O ::= ++V | --V */
void operation() {
    if (nextToken == INC_OP || nextToken == DEC_OP) {
        lex();
        variable();
    } else {
        error();
    }
}

/* V ::= a | b | … | y | z | aV | bV | … | yV | zV */
void variable() {
    if (nextToken == IDENT) {
        lex();
    } else {
        error();
    }
}

/* N ::= 0 | 1 | … | 8 | 9 | 0N | 1N | … | 8N | 9N */
void number() { 
    if (nextToken >= '0' && nextToken <= '9') {
        lex();
        while (nextToken >= '0' && nextToken <= '9') {
            lex();
        }
    } else {
        error();
    }
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
static void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';
    } else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its 
 * character class */
static void getChar() {
    nextChar = getc(in_fp);
    if (nextChar == '\n') {
        lineNum++;  // Increment line number only when newline is encountered
    }
    if (nextChar != EOF) {
        if (isalpha(nextChar)) {
            charClass = LETTER;
        } else if (isdigit(nextChar)) {
            charClass = DIGIT;
        } else {
            charClass = UNKNOWN;
        }
    } else {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace 
 * character */
static void getNonBlank() {
    while (isspace(nextChar)) {
        if (nextChar == '\n') lineNum++;  // Increment line number when newline is encountered
        getChar();  // Move to the next character
    }
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    // lexeme[0] = '\0';
    getNonBlank();

    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            if ((strcmp(lexeme,"read") == 0)) {
                nextToken = KEY_READ;
            } else if ((strcmp(lexeme,"print") == 0)) {
                nextToken = KEY_PRINT;
            } else if ((strcmp(lexeme,"if") == 0)) {
                nextToken = KEY_IF;
            } else if ((strcmp(lexeme,"else") == 0)) {
                nextToken = KEY_ELSE;
            } else if ((strcmp(lexeme,"begin") == 0)) {
                nextToken = KEY_BEGIN;
            } else if ((strcmp(lexeme,"end") == 0)) {
                nextToken = KEY_END;
            } else {
                nextToken = IDENT;
            }
            break;

        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            // getChar();
            break;

        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    } /* End of switch */

    switch (nextToken){
        case 0:
            printf("%s          LETTER\n",lexeme);
            break;
        case 1:
            printf("%s          DIGIT\n",lexeme);
            break;
        case 2:
            printf("%s          ASSIGN_OP\n",lexeme);
            break;
        case 4:
            printf("%s          LESSER_OP\n",lexeme);
            break;
        case 6:
            printf("%s          GREATER_OP\n",lexeme);
            break;
        case 8:
            printf("%s          EQUAL_OP\n",lexeme);
            break;
        case 10:
            printf("%s          NEQUAL_OP\n",lexeme);
            break;
        case 12:
            printf("%s          LEQUAL_OP\n",lexeme);
            break;
        case 14:
            printf("%s          GEQUAL_OP\n",lexeme);
            break;
        case 16:
            printf("%s          LEFT_PAREN\n",lexeme);
            break;
        case 18:
            printf("%s          RIGHT_PAREN\n",lexeme);
            break;
        case 20:
            printf("%s          ADD_OP\n",lexeme);
            break;
        case 22:
            printf("%s          SUB_OP\n",lexeme);
            break;
        case 24:
            printf("%s          MULT_OP\n",lexeme);
            break;
        case 26:
            printf("%s          DIV_OP\n",lexeme);
            break;
        case 28:
            printf("%s          POW_OP\n",lexeme);
            break;
        case 30:
            printf("%s          INC_OP\n",lexeme);
            break;
        case 32:
            printf("%s          DEC_OP\n",lexeme);
            break;
        case 34:
            printf("%s          SEMICOLON\n",lexeme);
            break;
        case 36:
            printf("%s          COLON\n",lexeme);
            break;
        case 38:
            printf("%s          KEY_READ\n",lexeme);
            break;
        case 40:
            printf("%s          KEY_PRINT\n",lexeme);
            break;
        case 42:
            printf("%s          KEY_IF\n",lexeme);
            break;
        case 44:
            printf("%s          KEY_ELSE\n",lexeme);
            break;
        case 46:
            printf("%s          KEY_BEGIN\n",lexeme);
            break;
        case 48:
            printf("%s          KEY_END\n",lexeme);
            break;
        case 50:
            printf("%s          IDENT\n",lexeme);
            break;
        case 52:
            printf("%s          INT_LIT\n",lexeme);
            break;
        case 99:
            printf("%s          UNKNOWN\n",lexeme);
            break;
    }

    // printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
} /* End of function lex */
