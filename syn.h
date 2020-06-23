#pragma once
#ifndef SYN_H
#define SYN_H

#include "lex.h"
#include "ast.h"

//Класс-исключение ошибки синтаксического анализатора
class SynErr
{
public:
    inline SynErr(string str)
    {
        cout << "Syntax error: " << str << endl;
    }
};

//Класс синтаксического анализатора
class Syn
{
public:
    Syn(Lex* lex); //Конструктор
    ~Syn(); //Деструктор

public:
    void Parse(); //Функция начинающая анализ
    Ast* GetAst(); //Функция для получения указателя на дерево
    int flag=0;
	int temp = 0;


private:
    //Нетерминалы грамматики
    Node* Program(void);
    Node* Number(void);
    Node* Identifier(void);
    Node* ParenthesizedExpression(void);
    Node* VariableDeclaration(void);
    Node* PrimaryExpression(void);
    Node* UnaryExpression(void);
    Node* MultiplicativeExpression(void);
    Node* AdditiveExpression(void);
    Node* RelationalExpression(void);
    Node* EqualityExpression(void);
    Node* AssignmentExpression(void);
    Node* Expression(void);
    Node* SelectionStatement(void);
    Node* IterationStatement(void);
    Node* ExpressionStatement(void);
    Node* StatementList(void);
    Node* Statement(void);



    inline void Error(const string& message) //Функция вызывающая ошибку
    {
        throw SynErr("Was expected '" + message + "'! Current token: '" + _lex->CurrentTokenValue() + "'\n");
    }


private:
    Lex* _lex; //Указатель на лексический анализатор
    Ast* _tree; //Указатель на синтаксическое дерево
};

#endif //!SYN_H
