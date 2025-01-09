#include "scanner.h"

class Parser
{
	private:
		Scanner* scanner;
		Token* lToken;

		void advance();
		void match(int);

		void program();
		void ClassList();
		void ClassListLinha();
		void ClassDecl();
		void ClassDeclLinha();
		void ClassBody();
		void VarDeclListOpt();
		void VarDeclList();
		void VarDeclListLinha();
		void VarDecl();
		void VarDeclLinha();
		void VarDeclOpt();
		void Type();
		void ConstructDeclListOpt();
		void ConstructDeclList();
		void ConstructDeclListLinha();
		void ConstructDecl();
		void MethodDeclListOpt();
		void MethodDeclList();
		void MethodDeclListLinha();
		void MethodDecl();
		void MethodDeclLinha();
		void MethodBody();
		void ParamListOpt();
		void ParamList();
		void ParamListLinha();
		void Param();
		void ParamLinha();
		void StatementsOpt();
		void Statements();
		void StatementsLinha();
		void Statement();
		void AtribStat();
		void AtribStatLinha();
		void PrintStat();
		void ReadStat();
		void ReturnStat();
		void SuperStat();
		void IfStat();
		void IfStatLinha();
		void ForStat();
		void AtribStatOpt();
		void ExpressionOpt();
		void LValue();
		void LValueComp();
		void LValueCompLinha();
		void Expression();
		void ExpressionLinha();
		void AllocExpression();
		void NumExpression();
		void NumExpressionLinha();
		void Term();
		void TermLinha();
		void UnaryExpression();
		void Factor();
		void ArgListOpt();
		void ArgList();
		void ArgListLinha();
		void error(string);

	public:
		Parser(string);
		void run();
};