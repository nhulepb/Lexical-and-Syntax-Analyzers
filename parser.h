#ifndef PARSER_H
#define PARSER_H

void program();
void statement();
void condition();
void expr();
void term();
void factor();
void operation();
void variable();
void number();
void syntaxError(const char *expected); // Handle syntax errors

extern int lineNum;
extern int nextToken;
#endif
