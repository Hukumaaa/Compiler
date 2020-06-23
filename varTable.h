#pragma once
#ifndef VAR_TABLE_H
#define VAR_TABLE_H

#include <vector>
#include "var.h"

//Класс таблицы переменных
class VarTable
{
public:
    VarTable(); //Конструктор
    ~VarTable(); //Деструктор

    void AddVar(Var* var); //Функция добавляющая переменную в таблицу
    bool HasVar(const string& name); //Функция проверяющая есть ли такая переменная в таблице
    void Print(); //Функция распечатывающая таблицу
    vector<Var*> GetArray(); //Функция возвращающая массив переменныъ

private:
    vector<Var*> _variables; //Массив переменных
};


#endif // !VAR_TABLE_H