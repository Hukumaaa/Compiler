#pragma once
#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <fstream>
#include "ast.h"
#include "codeGenConsts.h"

enum class AssemblerPart { DATA, BEFORE_MAIN, MAIN }; // Перечисление мест в ассеблере

//Класс генератора кода
class CodeGen
{
public:
    CodeGen(const char* fname, Ast* ast); //Конструктор
    void GenerateAssembler(); //Функция начинающая генерацию кода


private:
    void GenerateAssemblerCodeForBlocks(); //Общая функция для генерации кода блоков
    void GenerateAssemblerCodeForAllBlocks(Node* currentNode); //Функция генерирующая код для блоков

    void GenerateAssemblerCodeForSetBlock(Node* currentNode); //Функция генерирующая код для присваиваний
    void GenerateAssemblerCodeForIfBlock(Node* currentNode); //Функция генерирующая код для условий
    void GenerateAssemblerCodeForWhileBlock(Node* currentNode); //Функция генерирующая код для цикла while
    void GenerateAssemblerCodeForForBlock(Node* currentNode); //Функция генерирующая код для цикла for
    void GenerateAssemblerCodeForPrintFunctionCall(Node* currentNode); //Функция генерирующая код для вызова функции write
    void GenerateAssemblerCodeForExpressions(Node* currentNode); //Функция генерирующая код для арифметических выражений
    void GenerateAssemblerCodeForConditionExpressions(Node* currentNode); //Функция генерирующая код для логических выражений
    void GenerateAssemblerCodeForLocalVars(); //Функция генерирующая код для локальных переменных
    void GenerateAssemblerCodeForDivOperand(); //Функция генерирующая код для операндов деления
    void GenerateAssemblerCodeForPrintFunction(); //Функция генерирующая код для функции write
    void GenerateAssemblerCodeForStackVariable(Var* variable); //Функция генерирующая код для переменных на стеке

    void WriteInOutputFile(const string& text); //Функция напрямую пишущая в файл
    void SetPlaceInAssemblerForWrite(AssemblerPart place); //Функция для установки текущего места в ассемблере

    static string GetAssemblerLocalVar(const string& value); //Функция возвращающая представление локальной переменной в ассемблере

    // Функции для генерации основных команд ассемблера
    void GenPush(const string& value);
    void GenPop(const string& value);
    void GenAdd(const string& value1, const string& value2);
    void GenSub(const string& value1, const string& value2);
    void GenImul(const string& value1, const string& value2);
    void GenMov(const string& value1, const string& value2);
    void GenRaw(const string& value);
    void GenCmp(const string& value1, const string& value2);
    void GenJmp(const string& value);
    void GenJe(const string& value);
    void GenJne(const string& value);
    void GenJl(const string& value);
    void GenJle(const string& value);
    void GenJg(const string& value);
    void GenJge(const string& value);
    void GenLabel(const string& value);
    void GenFinit();
    void GenFild(const string& value);
    void GenFdiv(const string& value1, const string& value2);
    void GenFist(const string& value);


private:
    ofstream _outputFile; //Файл для записи полученного ассемблерного кода
    Ast* _tree; //Указатель на дерево синтаксического анализа

    string _assemblerPlaceData; //Сегмент данных
    string _assemblerPlaceBeforeMain; //Сегмент до основной метки
    string _assemblerPlaceMain; //Сегмент команд
    string* _assemblerCurrentPlace; //Указатель на текущее место в ассемблере

    int _byteOnStack;
    int _countLabels; //Количество меток в программе
};

#endif // !CODE_GEN_H
