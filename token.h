#include <iostream>
#include <ctype.h>
#include <string>

using namespace std;

enum Names 
{
    UNDEF, // 0
    ID, // 1
    INTEGER_LITERAL, // 2
    OP, // 3
    MENOR, // 4
    MAIOR, // 5
    MENORIGUAL, // 6
    MAIORIGUAL, // 7
    MAIS, // 8
    MENOS, // 9
    ASTERISK, // 10
    DIV, // 11
    PERCENT, // 12
    ASSIGN, // 13
    IGUAL, // 14
    DIFERENTE, // 15
    SEP, // 16
    OPEN_PAR, // 17
    CLOSE_PAR, // 18
    OPEN_BRA, // 19
    CLOSE_BRA, // 20
    OPEN_CHAVE, // 21
    CLOSE_CHAVE, // 22
    PONTOEVIRG, // 23
    PONTO, // 24
    VIRGULA, // 25
    STRING_LITERAL, // 26
    COMMENT, // 27
    PALAVRA_RESERVADA, // 28
    CLASS, // 29
    EXTENDS, // 30
    INT, // 31
    STRING, // 32
    BREAK, // 33
    PRINT, // 34  
    READ, // 35
    RETURN, // 36
    SUPER, // 37
    IF, // 38
    ELSE, // 39
    FOR, // 40
    NEW, // 41
    CONSTRUCTOR, // 42
    END_OF_FILE // 43
};

class Token 
{
    public: 
        int name;
        int attribute;
        string lexeme;
    
        Token(int name)
        {
            this->name = name;
            this->attribute = UNDEF;
        }

        Token(int name, string lex)
        {
            this->name = name;
            this->lexeme = lex;
        }
        
        Token(int name, int attr)
        {
            this->name = name;
            this->attribute = attr;
        }
};
