#include "scanner.h"    

// Construtor que recebe uma string com o nome do arquivo de entrada e preenche input com seu conteúdo

Scanner::Scanner(string input)
{
    pos = 0;
    line = 1;

    ifstream inputFile(input, ios::in);
    string line;

    if (inputFile.is_open())
    {
        while (getline(inputFile,line) )
        {
            this->input.append(line + '\n');
        }
        inputFile.close();
    }
    else 
        cout << "Unable to open file\n"; 
}

int
Scanner::getLine()
{
    return line;
}

// Método que retorna o próximo token da entrada

Token *
Scanner::nextToken()
{
    Token *tok;
    string lexeme;
    int state = 0;

    while (true)
    {
        switch (state)
        {
        case 0:  
            if(input[pos] == '\0') {
                tok = new Token(END_OF_FILE);
                return tok;
            }
            else if(isalpha(input[pos]) || input[pos] == '_')
                state = 1;
            else if(isspace(input[pos]))
                state = 3;
            else if(isdigit(input[pos]))
                state = 5;
            // Necessário para diferenciar !=, >= <= de = e ==
            else if(input[pos] == '=' && input[pos-1] != '>' && input[pos-1] != '<' && input[pos-1] != '!')
                state = 22;
            else if(input[pos] == '"')
                state = 7;
            else if(input[pos] == '/')
                state = 9;
            else if(input[pos] == '<')
                state = 14;
            else if(input[pos] == '>')
                state = 17;
            else if(input[pos] == '!')
                state = 20;
            else if(input[pos] == '+')
                state = 25;
            else if(input[pos] == '-')
                state = 26;
            else if(input[pos] == '*')
                state = 27;
            else if(input[pos] == '%')
                state = 28;
            else if(input[pos] == ',')
                state = 29;
            else if(input[pos] == ';')
                state = 30;
            else if(input[pos] == '.')
                state = 31;
            else if(input[pos] == '(')
                state = 32;
            else if(input[pos] == ')')
                state = 33;
            else if(input[pos] == '{')
                state = 34;
            else if(input[pos] == '}')
                state = 35;
            else if(input[pos] == '[')
                state = 36;
            else if(input[pos] == ']')
                state = 37;

            pos++;
            break;
        // Case 1 e 2 são para identificadores
        case 1:
            if(!isalnum(input[pos]) && input[pos] != '_') {
                state = 2;
            }
            
            lexeme += input[pos-1];
            pos++;
            break;
        case 2:
            pos--;
            // Verifica se a palavra é reservada primeiramente
            if(lexeme == "class")
                tok = new Token(PALAVRA_RESERVADA, CLASS);
            else if(lexeme == "extends")
                tok = new Token(PALAVRA_RESERVADA, EXTENDS);
            else if(lexeme == "int")
                tok = new Token(PALAVRA_RESERVADA, INT);
            else if(lexeme == "float")
                tok = new Token(PALAVRA_RESERVADA, STRING);
            else if(lexeme == "break")
                tok = new Token(PALAVRA_RESERVADA, BREAK);
            else if(lexeme == "print")
                tok = new Token(PALAVRA_RESERVADA, PRINT);
            else if(lexeme == "read")
                tok = new Token(PALAVRA_RESERVADA, READ);
            else if(lexeme == "return")
                tok = new Token(PALAVRA_RESERVADA, RETURN);
            else if(lexeme == "super")
                tok = new Token(PALAVRA_RESERVADA, SUPER);
            else if(lexeme == "if")
                tok = new Token(PALAVRA_RESERVADA, IF);
            else if(lexeme == "else")
                tok = new Token(PALAVRA_RESERVADA, ELSE);
            else if(lexeme == "for")
                tok = new Token(PALAVRA_RESERVADA, FOR);
            else if(lexeme == "new")
                tok = new Token(PALAVRA_RESERVADA, NEW);
            else if(lexeme == "constructor")
                tok = new Token(PALAVRA_RESERVADA, CONSTRUCTOR);
            // Se não for reservada, é um identificador
            else {
                tok = new Token(ID, lexeme);
            }
            
            return tok;
        // Case 3 e 4 são para espaços em branco
        case 3:
            if(!isspace(input[pos])) {
                state = 4;
            }
            pos++;
            break;
        case 4:
            pos--;
            state = 0;
            break;
        // Case 5 e 6 são para números inteiros
        case 5:
            if(!isdigit(input[pos])) {
                state = 6;
            }
            pos++;
            break;
        case 6:
            pos--;
            tok = new Token(INTEGER_LITERAL);
            return tok;
        // Case 7 e 8 são para strings
        case 7:
            if(input[pos] == '"') {
                state = 8;
            }
            pos++;
            break;
        case 8:
            tok = new Token(STRING_LITERAL);
            return tok;
        // Case 9, 10, 11 e 13 são para comentários
        case 9:
            if(input[pos] == '/') {
                state = 10;
            }
            else if(input[pos] == '*') {
                state = 11;
            }
            else {
                state = 12;
            }
            pos++;
            break;
        // Case 10 é para comentários de linha
        case 10:
            if(input[pos] == '\n' || input[pos] == '\0') {
                state = 0;
            }
            pos++;
            break;
        // Case 11 e 13 são para comentários de bloco
        case 11:
            if(input[pos] == '*') {
                state = 13;
            }
            pos++;
            break;
        // Case 12 e pro operador de divisão
        case 12:
            pos--;
            tok = new Token(OP, DIV);
            return tok;
        // Case 13 comentário de bloco
        case 13:
            if(input[pos] == '/') {
                state = 0;
            }
            else {
                state = 11;
            }
            pos++;
            break;
        // Case 14, 15 e 16 são para operadores de comparação
        case 14:
            if(input[pos] == '=') {
                state = 15;
            }
            else {
                state = 16;
            }
            pos++;
            break;
        // case 15 e o operador de menor igual
        case 15:
            pos--;
            tok = new Token(OP, MENORIGUAL);
            return tok;
        // case 16 e o operador de menor
        case 16:
            pos--;
            tok = new Token(OP, MENOR);
            return tok;
        // case 17 e um passo para o operador de maior igual e maior
        case 17:
            if(input[pos] == '=') {
                state = 18;
            }
            else {
                state = 19;
            }
            pos++;
            break;
        // case 18 e o operador de maior igual
        case 18:
            pos--;
            tok = new Token(OP, MAIORIGUAL);
            return tok;
        // case 19 e o operador de maior
        case 19:
            pos--;
            tok = new Token(OP, MAIOR);
            return tok;
        // case 20 e um passo para o operador de diferente e de atribuição
        case 20:
            if(input[pos] == '=') {
                state = 21;
            }
            else {
                lexicalError("Caractere invalido");
            }
            pos++;
            break;
        // case 21 e o operador de diferente
        case 21:
            pos--;
            tok = new Token(OP, DIFERENTE);
            return tok;
        // case 22 e um passo para o operador de igual e de atribuição
        case 22:
            if(input[pos] == '=') {
                state = 23;
            }
            else {
                state = 24;
            }
            pos++;
            break;
        // case 23 e o operador de igual
        case 23:
            pos--;
            tok = new Token(OP, IGUAL);
            return tok;
        // case 24 e o operador de atribuição
        case 24:
            pos--;
            tok = new Token(OP, ASSIGN);
            return tok;
        // case 25 e o operador de soma
        case 25:
            tok = new Token(OP, MAIS);
            return tok;
        // case 26 e o operador de subtração
        case 26:
            tok = new Token(OP, MENOS);
            return tok;
        // case 27 e o operador de multiplicação
        case 27:
            tok = new Token(OP, ASTERISK);
            return tok;
        // case 28 e o operador de resto
        case 28:
            tok = new Token(OP, PERCENT);
            return tok;
        case 29:
            tok = new Token(SEP, VIRGULA);
            return tok;
        case 30:
            tok = new Token(SEP, PONTOEVIRG);
            return tok;
        case 31:
            tok = new Token(SEP, PONTO);
            return tok;
        case 32:
            tok = new Token(SEP, OPEN_PAR);
            return tok;
        case 33:
            tok = new Token(SEP, CLOSE_PAR);
            return tok;
        case 34:
            tok = new Token(SEP, OPEN_CHAVE);
            return tok;
        case 35:
            tok = new Token(SEP, CLOSE_CHAVE);
            return tok;
        case 36:
            tok = new Token(SEP, OPEN_BRA);
            return tok;
        case 37:
            tok = new Token(SEP, CLOSE_BRA);
            return tok;
        }

    }
}

void 
Scanner::lexicalError(string msg)
{
    cout << "Linha "<< line << ": " << msg << endl;
    
    exit(EXIT_FAILURE);
}
