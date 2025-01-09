#include "parser.h"

Parser::Parser(string input)
{
	scanner = new Scanner(input);
}

void
Parser::advance()
{
	lToken = scanner->nextToken();
}

void
Parser::match(int t)
{
	if (lToken->name == t || lToken->attribute == t)
		advance();
	else
		error("Erro inesperado");
}

void
Parser::run()
{
	advance();	

	program();

	cout << "Compilação encerrada com sucesso!\n";
}

// 1. Program -> ClassList | Epsilon
void
Parser::program()
{
	if(lToken->attribute == CLASS)
	{
		ClassList();
	}
	else if(lToken->name == EOF)
		error("Arquivo vazio!\n");
	else
		error ("Formato inválido!\n");
}

// 2. ClassList -> ClassDecl ClassList | ClassDecl
// A regra acima possui ambiguidade.
// Resolve-se fazendo fatoração a esquerda:
// A → αA' | γ
// A' → β1 | β2 | ….| βn
// Portanto:
// ClassList -> ClassDecl ClassList'
// ClassList' -> ClassList | Epsilon

void
Parser::ClassList()
{
	ClassDecl();
	ClassListLinha();
}

void
Parser::ClassListLinha()
{
	if(lToken->attribute == CLASS)
	{	
		advance();
		ClassList();
	}
	else;
}

// 3.  ClassDecl → class ID ClassBody | class ID extends ID ClassBody
// A regra acima possui ambiguidade.
// Resolve-se fazendo fatoração a esquerda:
// Portanto:
// ClassDecl → class ID ClassDecl'
// ClassDecl' → ClassBody | extends ID ClassBody

void
Parser::ClassDecl()
{
	match(CLASS);
	match(ID);
	ClassDeclLinha();
}

void
Parser::ClassDeclLinha()
{
	if(lToken->attribute == EXTENDS)
	{
		advance();
		match(ID);
		ClassBody();
	}

	else
	{
		advance();
		ClassBody();
	}
}

// 4. ClassBody -> { VarDeclListOpt ConstructDeclListOpt MethodDeclListOpt }

void
Parser::ClassBody() 
{
	match(OPEN_CHAVE);
	VarDeclListOpt();
	ConstructDeclListOpt();
	MethodDeclListOpt();
	match(CLOSE_CHAVE);
}

// 5. VarDeclListOpt -> VarDeclList | Epsilon
void
Parser::VarDeclListOpt()
{
	VarDeclList();
}

// 6. VarDeclList -> VarDeclList VarDecl | VarDecl
// A regra acima possui recursão imediata a esquerda.
// Resolve-se fazendo Eliminação de recursão a esquerda:
// 1º Passo: identifique as produções em que nenhum βi começa
// com A:
// A → Aα1 | Aα2 | …. | Aαm | β1 | β2 | …. | βn
// • 2º Passo: substitua as A-produções por:
// A → β1A' | β2A' | …. | βnA'
// A' → α1 A' | α2A' | …. | αmA' | ε
// Portanto:
// VarDeclList -> VarDecl VarDeclList'
// VarDeclList' -> VarDeclList | Epsilon

void
Parser::VarDeclList()
{
	VarDecl();
	VarDeclListLinha();
}

void
Parser::VarDeclListLinha()
{
	if(lToken->attribute == INT || lToken->name == STRING_LITERAL || lToken->name == ID)
	{
		VarDecl();
		VarDeclListLinha();
	}
	else;
}

// 7. VarDecl -> Type ID VarDeclOpt ; | Type [] ID VarDeclOpt ;
// A regra acima possui ambiguidade.
// Resolve-se fazendo fatoração a esquerda:
// Portanto:
// VarDecl -> Type VarDecl'
// VarDecl' -> ID VarDeclOpt ; | [] ID VarDeclOpt ;

void
Parser::VarDecl()
{
	Type();
	VarDeclLinha();
}

void
Parser::VarDeclLinha()
{
	if(lToken->name == ID)
	{
		advance();
		VarDeclOpt();
		match(PONTOEVIRG);
	}
	else if(lToken->attribute == OPEN_BRA)
	{
		advance();
		match(CLOSE_BRA);
		match(ID);
		VarDeclOpt();
		match(PONTOEVIRG);
	}
}

// 8. VarDecltOpt -> , ID VarDeclOpt | Epsilon

void
Parser::VarDeclOpt()
{
	if(lToken->attribute == VIRGULA)
	{
		advance();
		match(ID);
		VarDeclOpt();
	}
	else;
}

// 9. Type -> int | string | ID

void
Parser::Type()
{
	if(lToken->attribute == INT)
	{
		advance();
	}
	else if(lToken->name == STRING_LITERAL)
	{
		advance();
	}
	else if(lToken->name == ID)
	{
		advance();
	}
}

// 10. ConstructDeclListOpt -> ConstructDeclList | Epsilon

void
Parser::ConstructDeclListOpt()
{
	ConstructDeclList();
}

// 11. ConstructDeclList -> ConstructDeclList ConstructDecl | ConstructDecl
// A regra acima possui recursão imediata a esquerda.
// Resolve-se fazendo Eliminação de recursão a esquerda:
// ConstructDeclList -> ConstructDecl ConstructDeclList'
// ConstructDeclList' -> ConstructDeclList | Epsilon

void
Parser::ConstructDeclList()
{
	ConstructDecl();
	ConstructDeclListLinha();
}

void
Parser::ConstructDeclListLinha()
{
	if(lToken->attribute == CONSTRUCTOR)
	{
		advance();
		ConstructDecl();
		ConstructDeclListLinha();
	}
	else;
}

// 12. ConstructDecl -> constructor MethodBody

void
Parser::ConstructDecl()
{
	match(CONSTRUCTOR);
	MethodBody();
}

// 13. MethodDeclListOpt -> MethodDeclList | Epsilon

void
Parser::MethodDeclListOpt()
{
	MethodDeclList();
}

// 14. MethodDeclList -> MethodDeclList MethodDecl | MethodDecl
// A regra acima possui recursão imediata a esquerda.
// Resolve-se fazendo Eliminação de recursão a esquerda:
// MethodDeclList -> MethodDecl MethodDeclList'
// MethodDeclList' -> MethodDeclList | Epsilon

void
Parser::MethodDeclList()
{
	MethodDecl();
	MethodDeclListLinha();
}

void
Parser::MethodDeclListLinha()
{
	MethodDecl();
	MethodDeclListLinha();
}

// 15. MethodDecl -> Type ID MethodBody | Type [] ID MethodBody
// A regra acima possui ambiguidade.
// Resolve-se fazendo fatoração a esquerda:
// Portanto:
// MethodDecl -> Type MethodDecl'
// MethodDecl' -> ID MethodBody | [] ID MethodBody

void
Parser::MethodDecl()
{
	Type();
	MethodDeclLinha();
}

void
Parser::MethodDeclLinha()
{
	if(lToken->name == ID)
	{
		advance();
		MethodBody();
	}
	else if(lToken->attribute == OPEN_BRA)
	{
		advance();
		match(CLOSE_BRA);
		match(ID);
		MethodBody();
	}
}

// 16. MethodBody -> ( ParamListOpt) { StatementsOpt }
void
Parser::MethodBody()
{
	match(OPEN_PAR);
	ParamListOpt();
	match(CLOSE_PAR);
	match(OPEN_CHAVE);
	StatementsOpt();
	match(CLOSE_CHAVE);
}

// 17. ParamListOpt -> ParamList | Epsilon
void
Parser::ParamListOpt()
{
	ParamList();
}

// 18. ParamList -> ParamList , Param | Param
// A regra acima possui recursão imediata a esquerda.
// Resolve-se fazendo Eliminação de recursão a esquerda:
// ParamList -> Param ParamList'
// ParamList' -> , Param ParamList | Epsilon

void
Parser::ParamList()
{
	Param();
	ParamListLinha();
}

void
Parser::ParamListLinha()
{
	if(lToken->attribute == VIRGULA)
	{
		advance();
		Param();
		ParamListLinha();
	}
	else;
}

// 19. Param -> Type ID | Type [] ID
// A regra acima possui ambiguidade.
// Resolve-se fazendo fatoração a esquerda:
// Portanto:
// Param -> Type Param'
// Param' -> ID | [] ID

void
Parser::Param()
{
	Type();
	ParamLinha();
}

void
Parser::ParamLinha()
{
	if(lToken->name == ID)
	{
		advance();
	}
	else if(lToken->attribute == OPEN_BRA)
	{
		advance();
		match(CLOSE_BRA);
		match(ID);
	}
}

// 20. StatementsOpt -> Statements | Epsilon
void
Parser::StatementsOpt()
{
	Statements();
}

// 21. Statements -> Statements Statement | Statement
// A regra acima possui recursão imediata a esquerda.
// Resolve-se fazendo Eliminação de recursão a esquerda:
// Statements -> Statement Statements'
// Statements' -> Statement Statements' | Epsilon

void
Parser::Statements()
{
	Statement();
	StatementsLinha();
}

void
Parser::StatementsLinha()
{
	Statement();
	StatementsLinha();
}

// 22. Statement -> VarDeclList | ArtibStat ; | PrintStat ; | ReadStat ; | ReturnStat ; | SuperStat ; | IfStat | ForStat | break ; | ;
void
Parser::Statement()
{
	if(lToken->attribute == INT || lToken->name == STRING_LITERAL || lToken->name == ID)
	{
		advance();
		VarDeclList();
	}
	else if(lToken->name == ID)
	{
		advance();
		AtribStat();
		match(PONTOEVIRG);
	}
	else if(lToken->attribute == PRINT)
	{
		advance();
		PrintStat();
		match(PONTOEVIRG);
	}
	else if(lToken->attribute == READ)
	{
		advance();
		ReadStat();
		match(PONTOEVIRG);
	}
	else if(lToken->attribute == RETURN)
	{
		advance();
		ReturnStat();
		match(PONTOEVIRG);
	}
	else if(lToken->attribute == SUPER)
	{
		advance();
		SuperStat();
		match(PONTOEVIRG);
	}
	else if(lToken->attribute == IF)
	{
		advance();
		IfStat();
	}
	else if(lToken->attribute == FOR)
	{
		advance();
		ForStat();
	}
	else if(lToken->attribute == BREAK)
	{
		advance();
		match(PONTOEVIRG);
	}
	else if(lToken->attribute == PONTOEVIRG)
	{
		match(PONTOEVIRG);
	}
}

// 23. AtribStat -> LValue = Expression | LValue = AllocExpression
// A regra acima possui ambiguidade.
// Resolve-se fazendo fatoração a esquerda:
// Portanto:
// AtribStat -> LValue = AtribStat'
// AtribStat' -> Expression | AllocExpression

void
Parser::AtribStat()
{
	LValue();
	match(ASSIGN);
	AtribStatLinha();
}

void
Parser::AtribStatLinha()
{
	if(lToken->attribute == NEW)
	{
		advance();
		AllocExpression();
	}
	else
	{
		advance();
		if(lToken->attribute == OPEN_BRA)
		{
			Expression();
			match(CLOSE_BRA);
		}
		else
			Expression();
	}
}

// 24. PrintStat -> print Expression
void
Parser::PrintStat()
{
	if(lToken->attribute == PRINT)
	{
		advance();
		Expression();
	}
}

// 25. ReadStat -> read LValue
void
Parser::ReadStat()
{
	if(lToken->attribute == READ)
	{
		advance();
		LValue();
	}
}

// 26. ReturnStat -> return Expression
void
Parser::ReturnStat()
{
	if(lToken->attribute == RETURN)
	{
		advance();
		Expression();
	}
}

// 27. SuperStat -> super ( ArgListOpt )
void
Parser::SuperStat()
{
	if(lToken->attribute == SUPER)
	{
		advance();
		match(OPEN_PAR);
		ArgListOpt();
		match(CLOSE_PAR);
	}
}

// 28. IfStat → if ( Expression ) { Statements }
// | if ( Expression ) { Statements } else { Statements }
// A regra acima possui ambiguidade.
// Resolve-se fazendo fatoração a esquerda:
// Portanto:
// IfStat -> if ( Expression ) { Statements } IfStat'
// IfStat' -> else { Statements } | Epsilon
void
Parser::IfStat()
{
	if(lToken->attribute == IF)
	{
		advance();
		match(OPEN_PAR);
		Expression();
		match(CLOSE_PAR);
		match(OPEN_CHAVE);
		Statements();
		match(CLOSE_CHAVE);
		IfStatLinha();
	}
}

void
Parser::IfStatLinha()
{
	if(lToken->attribute == ELSE)
	{
		advance();
		match(OPEN_CHAVE);
		Statements();
		match(CLOSE_CHAVE);
	}
	else;
}

// 29. ForStat → for ( AtribStatOpt ; ExpressionOpt ; AtribStatOpt ) { Statements }
void
Parser::ForStat()
{
	if(lToken->attribute == FOR)
	{
		advance();
		match(OPEN_PAR);
		AtribStatOpt();
		match(PONTOEVIRG);
		ExpressionOpt();
		match(PONTOEVIRG);
		AtribStatOpt();
		match(CLOSE_PAR);
		match(OPEN_CHAVE);
		Statements();
		match(CLOSE_CHAVE);
	}
}

// 30. AtribStatOpt -> AtribStat | Epsilon
void
Parser::AtribStatOpt()
{
	AtribStat();
}

// 31. ExpressionOpt -> Expression | Epsilon
void
Parser::ExpressionOpt()
{
	Expression();
}

// 32. LValue -> ID LValueComp
void
Parser::LValue()
{
	if(lToken->name == ID)
	{
		advance();
		LValueComp();
	}
}

// 33. LValueComp → . ID LValueComp
// | . ID [ Expression ] LValueComp
// | . ID ( ArgListOpt ) LValueComp
// | [ Expression ] LValueComp
// | Epsilon
// A regra acima possui ambiguidade.
// Resolve-se fazendo fatoração a esquerda:
// Portanto:
// LValueComp -> . ID LValueComp' | [ Expression ] LValueComp | Epsilon
// LValueComp' -> [ Expression ] LValueComp | ( ArgListOpt ) LValueComp
void
Parser::LValueComp()
{
	if(lToken->attribute == PONTO)
	{
		advance();
		match(ID);
		LValueCompLinha();
	}
	else if(lToken->attribute == OPEN_BRA)
	{
		advance();
		Expression();
		match(CLOSE_BRA);
		LValueComp();
	}
	else;
}

void
Parser::LValueCompLinha()
{
	if(lToken->attribute == OPEN_BRA)
	{
		advance();
		Expression();
		match(CLOSE_BRA);
		LValueComp();
	}
	else if(lToken->attribute == OPEN_PAR)
	{
		advance();
		ArgListOpt();
		match(CLOSE_PAR);
		LValueComp();
	}
	else;
}

// 34. Expression -> NumExpression | NumExpression RelOp NumExpression
// A regra acima possui ambiguidade.
// Resolve-se fazendo fatoração a esquerda:
// Portanto:
// Expression -> NumExpression Expression'
// Expression' -> RelOp NumExpression | Epsilon
void
Parser::Expression()
{
	NumExpression();
	ExpressionLinha();
}

void
Parser::ExpressionLinha()
{
	if(lToken->attribute == IGUAL || lToken->attribute == DIFERENTE || lToken->attribute == MENOR || lToken->attribute == MENORIGUAL || lToken->attribute == MAIOR || lToken->attribute == MAIORIGUAL)
	{
		match(lToken->attribute);
		NumExpression();
	}
	else;
}

// 35. AllocExpression -> new ID ( ArgListOpt ) | Type [ Expression ]
void
Parser::AllocExpression()
{
	if(lToken->attribute == NEW)
	{
		advance();
		match(ID);
		match(OPEN_PAR);
		ArgListOpt();
		match(CLOSE_PAR);
	}
	else if(lToken->attribute == INT || lToken->name == STRING_LITERAL || lToken->name == ID)
	{
		advance();
		Type();
		match(OPEN_BRA);
		Expression();
		match(CLOSE_BRA);
	}
}

// 36. NumExpression -> Term + Term | Term - Term | Term
// A regra acima possui ambiguidade.
// Resolve-se fazendo fatoração a esquerda:
// Portanto:
// NumExpression -> Term NumExpression'
// NumExpression' -> + Term NumExpression' | - Term NumExpression' | Epsilon

void
Parser::NumExpression()
{
	Term();
	NumExpressionLinha();
}

void
Parser::NumExpressionLinha()
{
	if(lToken->attribute == MAIS)
	{
		advance();
		Term();
		NumExpressionLinha();
	}
	else if(lToken->attribute == MENOS)
	{
		advance();
		Term();
		NumExpressionLinha();
	}
	else;
}

// 37. Term → UnaryExpression * UnaryExpression
// | UnaryExpression / UnaryExpression
// | UnaryExpression % UnaryExpression
// | UnaryExpression
// A regra acima possui ambiguidade.
// Resolve-se fazendo fatoração a esquerda:
// Portanto:
// Term -> UnaryExpression Term'
// Term' -> * UnaryExpression Term' | / UnaryExpression Term' | % UnaryExpression Term' | Epsilon

void
Parser::Term()
{
	UnaryExpression();
	TermLinha();
}

void
Parser::TermLinha()
{
	if(lToken->attribute == ASTERISK)
	{
		advance();
		UnaryExpression();
		TermLinha();
	}
	else if(lToken->attribute == DIV)
	{
		advance();
		UnaryExpression();
		TermLinha();
	}
	else if(lToken->attribute == PERCENT)
	{
		advance();
		UnaryExpression();
		TermLinha();
	}
	else;
}

// 38. UnaryExpression → + Factor
// | - Factor

void
Parser::UnaryExpression()
{
	if(lToken->attribute == MAIS)
	{
		advance();
		Factor();
	}
	else if(lToken->attribute == MENOS)
	{
		advance();
		Factor();
	}
}

// 39. Factor → INTEGER_LITERAL
// | STRING_LITERAL
// | LValue
// | ( Expression )

void
Parser::Factor()
{
	if(lToken->name == INTEGER_LITERAL)
	{
		advance();
	}
	else if(lToken->name == STRING_LITERAL)
	{
		advance();
	}
	else if(lToken->name == ID)
	{
		advance();
		LValue();
	}
	else if(lToken->attribute == OPEN_PAR)
	{
		advance();
		Expression();
		match(CLOSE_PAR);
	}
}

// 40. ArgListOpt -> ArgList | Epsilon
void
Parser::ArgListOpt()
{
	ArgList();
}

// 41. ArgList -> ArgList , Expression | Expression
// A regra acima possui recursão imediata a esquerda.
// Resolve-se fazendo Eliminação de recursão a esquerda:
// ArgList -> Expression ArgList'
// ArgList' -> , Expression ArgList' | Epsilon

void
Parser::ArgList()
{
	Expression();
	ArgListLinha();
}

void
Parser::ArgListLinha()
{
	if(lToken->attribute == VIRGULA)
	{
		advance();
		Expression();
		ArgListLinha();
	}
	else;
}

void
Parser::error(string str)
{
	cout << "Linha " << scanner->getLine() << ": " << str << endl;

	exit(EXIT_FAILURE);
}