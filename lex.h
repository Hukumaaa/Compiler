#pragma once
#ifndef LEX_H
#define LEX_H

#include<map>
#include<iostream>
#include<vector>
#include<string>
#include<fstream>

using namespace std;

typedef enum Statement { S0, S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, 
							S11, S12, S13, S14, S15, S16, S17, S18, S19, S20, 
							S21, S22, S23, S24, S25, S26, S27, S28, S29, S30, 
							S31, S32, S33, S34, S35}; // Набор состояний

typedef enum Output { O0, O1, O2, O3, O4, O5, O6, O7, O8, O9, O10, 
						O11, O12, O13, O14, O15, O16, O17, O18, O19, O20, 
						O21, O22, O23, O24, O25, O26, O27, O28, O29}; // Набор выходных сигналов

typedef enum TokenType { IDENT, DEC, REAL, LPAR, LT, LTE, GT, GTE, COLON,
							ASSIGN, STRCONST, EQ, RPAR, LSBR, RSBR, LCBR, RCBR, DOT, 
							RANGE, COMMA, SEMICOLON, PLUS, MINUS, MUL, DIV, NEQ, 
							KW_BEGIN, KW_END, KW_END_, KW_INTEGER, KW_REAL, KW_IF, KW_ELSE, KW_THEN, KW_MODULE,
							KW_PROCEDURE, KW_FOR, KW_WHILE, KW_RETURN, KW_TO, KW_DO, KW_IMPORT, KW_VAR, COMMENT, EOFILE}; //Типы лексем

typedef pair<Statement, Output> Result; // Пара (Новое Состояние & Выходной сигнал)
typedef pair<Statement, char> Input; // Составной ключ (Текущее Состояние & Входной символ)

typedef pair<Input, Result> Item; //Элемент таблицы переходов

extern map<Input, Result> M; // Таблица переходов КА
extern map<string, TokenType> KW; // Словарь ключевых слов

//Класс-исключение ошибки лексического анализатора
class LexErr
{
public:
	inline LexErr(string str)
	{
		cout << "Lexical error: " << str << endl;
	}
};

//Функция инициализации должна заполнить таблицу переходов конечного автомата
void Init(void);

//Класс "Лексема"
class Token
{
public:
	string _value; // Строковое значение
	TokenType _type; //Тип лексемы
};

//Класс лексического анализатора
class Lex
{
public:
	Lex(const char *fname); //Конструктор
	~Lex(); //Деструктор
	bool GetToken(Token &token); //Функция получения очередной лексемы

public:
    void GetTokens(); //Функция получения всех токенов в массив
    void PrintTokens(); //Функция распечатки всех токенов
    void NextToken(); //Функция сдвигающая индекс текущего токена
    void PrevToken(); //Функция сдвигающая индекс текущего токена назад
	bool LastToken(); // Функция возрашающая 1 если текущий токен последний
    string CurrentTokenValue(); //Функция возвращающая значение текущего токена
    TokenType CurrentTokenType(); //Функция возвращающая тип текущего токена

private:
    vector<Token> _tokens; //Массив токенов
    int _curToken; //Индекс текущего токена
	ifstream _file; //Файловый поток для чтения символов входной цепочки
	bool _wasEOF; //Признак конца файла при предыдущем запросе лексемы
	char getSClass(char symbol); //Функция получения класса символа

};

#endif // !LEX_H
