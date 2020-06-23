#pragma once
#ifndef AST_H
#define AST_H

#include <iostream>
#include <string>
#include "node.h"
#include "varTable.h"

//Класс-исключение ошибки семантического анализатора
class SemErr
{
public:
    inline SemErr(string str)
    {
        cout << "Semantic error: " << str << endl;
    }
};

//Класс абстрактного синтаксического дерева
class Ast
{
public:
    Node* root; //Указатель на вершину дерева

public:
    Ast(); //Конструктор

    void Analyze(); //Функция начинающая анализ
    void Print(); //Функция распечатывающая дерево
    VarTable GetVars(); //Функция возвращающая таблицу переменных

private:
    void FindVars(Node* currentNode); //Функция для поиска всех переменных
    static void Print(Node* currentNode, size_t level); //Функция для рекурсивной распечатки дерева

private:
    VarTable _vars; //Таблица переменных
};

#endif // !AST_H