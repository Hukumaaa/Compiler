#pragma once
#ifndef VAR_H
#define VAR_H

#include <iostream>
#include <string>

using namespace std;

//Класс "Переменная"
class Var
{
public:
    Var(const string& name); //Конструктор

    void Print(); //Функция распечатывающая переменную
    string GetName(); //Функция возвращающая имя переменной

private:
    string _name; //Имя переменной
};

#endif // !VAR_H