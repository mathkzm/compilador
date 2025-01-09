#include <fstream>
#include "token.h"

class Scanner 
{
    private: 
        string input; // Armazena o texto de entrada
        int pos; // Posição atual
        int line;
    
    public:
   
        Scanner(string); // Construtor

        int getLine();
    
        Token* nextToken(); // Método que retorna o próximo token da entrada       
    
        void lexicalError(string); // Método para manipular erros
};
