#pragma once
#ifndef NODE_H
#define NODE_H

#include "lex.h"

typedef enum class NodeType { IDENTIFIER, VARIABLE_DECLARATION, USING_VARIABLE, VARIABLE_TYPE,
                                INTEGER_CONST, ADD, SUB, MUL, DIV, UNARY_MINUS, SET, EQUAL,
                                NOT_EQUAL, LESS, GREATER, LESS_EQUAL, GREATER_EQUAL,FOR, WHILE,
                                REPEAT, IF, IF_ELSE, EXPRESSION, STATEMENT,
                                STATEMENT_LIST, PRINT }; //Набор типов узлов


//Класс узла синтаксического дерева
class Node
{
public:
    string value; //Значение узла
    NodeType type; //Тип узла

    Node* op1; //Потомок 1
    Node* op2; //Потомок 2
    Node* op3; //Потомок 3
    Node* op4; //Потомок 4

public:
    //Конструктор
    Node(NodeType type, const string& value = "", Node* op1 = nullptr, Node* op2 = nullptr, Node* op3 = nullptr,
         Node* op4 = nullptr);

    bool isComparisonOperator(); //Функция проверяющая, является ли узел оператором сравнения
};

#endif //!NODE_H