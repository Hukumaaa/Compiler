#include "lex.h"
#include <map>
#include <string>
#include <iostream>
#include <fstream>

#define M(A,B,X,Y) M.insert(Item(Input(A,B),Result(X,Y)))

map<Input, Result> M; //Таблица переходов КА
map<string, TokenType> KW; // Словарь ключевых слов

//Функция-помощник, удаляет пробелы в конце и начале строки.
string trim(const string &str) // Принимает на вход строку
{
	string::const_iterator it1 = str.begin(); //Итератор it1 указывает на начало строки
	string::const_reverse_iterator it2r = str.rbegin(); //Обратный итератор it2 указывает на конец строки
	while (it1 != str.end() && (*it1 == ' ' || *it1 == '\t' || *it1 == '\r' || *it1 == '\n')) it1++; //Пока пробел и не конец строки, сдвигаем итераторы
	while (it2r != str.rend() && (*it2r == ' ' || *it2r == '\t' || *it2r == '\r' || *it2r == '\n')) it2r--;
	return string(it1, it2r.base()); // Создаем строку на основе 2 итераторов, указывающих на начало и конец строки без пробелов
}

void Init(void)
{
	// Заполнение словаря ключевых слов
	KW.insert(pair<string, TokenType>("begin", KW_BEGIN));
	KW.insert(pair<string, TokenType>("end", KW_END));
	KW.insert(pair<string, TokenType>("end.", KW_END_));
	KW.insert(pair<string, TokenType>("integer", KW_INTEGER));
	KW.insert(pair<string, TokenType>("real", KW_REAL));
	KW.insert(pair<string, TokenType>("if", KW_IF));
	KW.insert(pair<string, TokenType>("else", KW_ELSE));
	KW.insert(pair<string, TokenType>("then", KW_THEN));
	//KW.insert(pair<string, TokenType>("MODULE", KW_MODULE));
	//KW.insert(pair<string, TokenType>("PROCEDURE", KW_PROCEDURE));
	KW.insert(pair<string, TokenType>("for", KW_FOR));
	KW.insert(pair<string, TokenType>("while", KW_WHILE));
	//KW.insert(pair<string, TokenType>("RETURN", KW_RETURN));
	KW.insert(pair<string, TokenType>("to", KW_TO));
	KW.insert(pair<string, TokenType>("do", KW_DO));
	//KW.insert(pair<string, TokenType>("IMPORT", KW_IMPORT));
	KW.insert(pair<string, TokenType>("var", KW_VAR));

	// Заполнение таблицы
	M(S0, 'z', S2, O1); M(S0, 'D', S2, O1); M(S0, 'E', S2, O1); M(S0, '9', S3, O2); M(S0, '<', S12, O5); M(S0, '>', S14, O7); M(S0, '=', S18, O12); M(S0, '"', S32, O11); M(S0, '(', S8, O4); M(S0, ')', S19, O13); M(S0, '[', S20, O14); M(S0, ']', S21, O15); M(S0, '{', S22, O16); M(S0, '}', S23, O17); M(S0, '.', S24, O18); M(S0, ',', S25, O20); M(S0, ':', S16, O9); M(S0, ';', S26, O21); M(S0, '+', S27, O22); M(S0, '-', S28, O23); M(S0, '*', S29, O24); M(S0, '/', S30, O25); M(S0, '#', S31, O26); M(S0, ' ', S0, O0);
	M(S2, 'z', S2, O1); M(S2, 'D', S2, O1); M(S2, 'E', S2, O1); M(S2, '9', S2, O1); M(S2, '<', S1, O1); M(S2, '>', S1, O1); M(S2, '=', S1, O1); M(S2, '"', S1, O1); M(S2, '(', S1, O1); M(S2, ')', S1, O1); M(S2, '[', S1, O1); M(S2, ']', S1, O1); M(S2, '{', S1, O1); M(S2, '}', S1, O1); M(S2, '.', S1, O1); M(S2, ',', S1, O1); M(S2, ':', S1, O1); M(S2, ';', S1, O1); M(S2, '+', S1, O1); M(S2, '-', S1, O1); M(S2, '*', S1, O1); M(S2, '/', S1, O1); M(S2, '#', S1, O1); M(S2, ' ', S1, O1);
	M(S3, 'z', S1, O29); M(S3, 'D', S1, O29); M(S3, 'E', S1, O29); M(S3, '9', S3, O2); M(S3, '<', S1, O2); M(S3, '>', S1, O2); M(S3, '=', S1, O2); M(S3, '"', S1, O2); M(S3, '(', S1, O2); M(S3, ')', S1, O2); M(S3, '[', S1, O2); M(S3, ']', S1, O2); M(S3, '{', S1, O2); M(S3, '}', S1, O2); M(S3, '.', S4, O3); M(S3, ',', S1, O2); M(S3, ':', S1, O2); M(S3, ';', S1, O2); M(S3, '+', S1, O2); M(S3, '-', S1, O2); M(S3, '*', S1, O2); M(S3, '/', S1, O2); M(S3, '#', S1, O2); M(S3, ' ', S1, O2);
	M(S4, 'z', S1, O29); M(S4, 'D', S5, O3); M(S4, 'E', S5, O3); M(S4, '9', S4, O3); M(S4, '<', S1, O3); M(S4, '>', S1, O3); M(S4, '=', S1, O3); M(S4, '"', S1, O3); M(S4, '(', S1, O3); M(S4, ')', S1, O3); M(S4, '[', S1, O3); M(S4, ']', S1, O3); M(S4, '{', S1, O3); M(S4, '}', S1, O3); M(S4, '.', S1, O3); M(S4, ',', S1, O3); M(S4, ':', S1, O3); M(S4, ';', S1, O3); M(S4, '+', S1, O3); M(S4, '-', S1, O3); M(S4, '*', S1, O3); M(S4, '/', S1, O3); M(S4, '#', S1, O3); M(S4, ' ', S1, O3);
	M(S5, 'z', S1, O29); M(S5, 'D', S1, O29); M(S5, 'E', S1, O29); M(S5, '9', S6, O3); M(S5, '<', S1, O29); M(S5, '>', S1, O29); M(S5, '=', S1, O29); M(S5, '"', S1, O29); M(S5, '(', S1, O29); M(S5, ')', S1, O29); M(S5, '[', S1, O29); M(S5, ']', S1, O29); M(S5, '{', S1, O29); M(S5, '}', S1, O29); M(S5, '.', S1, O29); M(S5, ',', S1, O29); M(S5, ':', S1, O29); M(S5, ';', S1, O29); M(S5, '+', S7, O3); M(S5, '-', S7, O3); M(S5, '*', S1, O29); M(S5, '/', S1, O29); M(S5, '#', S1, O29); M(S5, ' ', S1, O29);
	M(S6, 'z', S1, O29); M(S6, 'D', S1, O29); M(S6, 'E', S1, O29); M(S6, '9', S6, O3); M(S6, '<', S1, O3); M(S6, '>', S1, O3); M(S6, '=', S1, O3); M(S6, '"', S1, O3); M(S6, '(', S1, O3); M(S6, ')', S1, O3); M(S6, '[', S1, O3); M(S6, ']', S1, O3); M(S6, '{', S1, O3); M(S6, '}', S1, O3); M(S6, '.', S1, O3); M(S6, ',', S1, O3); M(S6, ':', S1, O3); M(S6, ';', S1, O3); M(S6, '+', S1, O3); M(S6, '-', S1, O3); M(S6, '*', S1, O3); M(S6, '/', S1, O3); M(S6, '#', S1, O3); M(S6, ' ', S1, O3);
	M(S7, 'z', S1, O29); M(S7, 'D', S1, O29); M(S7, 'E', S1, O29); M(S7, '9', S6, O3); M(S7, '<', S1, O29); M(S7, '>', S1, O29); M(S7, '=', S1, O29); M(S7, '"', S1, O29); M(S7, '(', S1, O29); M(S7, ')', S1, O29); M(S7, '[', S1, O29); M(S7, ']', S1, O29); M(S7, '{', S1, O29); M(S7, '}', S1, O29); M(S7, '.', S1, O29); M(S7, ',', S1, O29); M(S7, ':', S1, O29); M(S7, ';', S1, O29); M(S7, '+', S1, O29); M(S7, '-', S1, O29); M(S7, '*', S1, O29); M(S7, '/', S1, O29); M(S7, '#', S1, O29); M(S7, ' ', S1, O29);
	M(S8, 'z', S1, O4); M(S8, 'D', S1, O4); M(S8, 'E', S1, O4); M(S8, '9', S1, O4); M(S8, '<', S1, O4); M(S8, '>', S1, O4); M(S8, '=', S1, O4); M(S8, '"', S1, O4); M(S8, '(', S1, O4); M(S8, ')', S1, O4); M(S8, '[', S1, O4); M(S8, ']', S1, O4); M(S8, '{', S1, O4); M(S8, '}', S1, O4); M(S8, '.', S1, O4); M(S8, ',', S1, O4); M(S8, ':', S1, O4); M(S8, ';', S1, O4); M(S8, '+', S1, O4); M(S8, '-', S1, O4); M(S8, '*', S1, O4); M(S8, '/', S1, O4); M(S8, '#', S1, O4); M(S8, ' ', S1, O4);
	//M(S9, 'z', S9, O27); M(S9, 'D', S9, O27); M(S9, 'E', S9, O27); M(S9, '9', S9, O27); M(S9, '<', S9, O27); M(S9, '>', S9, O27); M(S9, '=', S9, O27); M(S9, '"', S9, O27); M(S9, '(', S9, O27); M(S9, ')', S9, O27); M(S9, '[', S9, O27); M(S9, ']', S9, O27); M(S9, '{', S9, O27); M(S9, '}', S9, O27); M(S9, '.', S9, O27); M(S9, ',', S9, O27); M(S9, ':', S9, O27); M(S9, ';', S9, O27); M(S9, '+', S9, O27); M(S9, '-', S9, O27); M(S9, '*', S10, O27); M(S9, '/', S9, O27); M(S9, '#', S9, O27); M(S9, ' ', S9, O27);
	//M(S10, 'z', S9, O27); M(S10, 'D', S9, O27); M(S10, 'E', S9, O27); M(S10, '9', S9, O27); M(S10, '<', S9, O27); M(S10, '>', S9, O27); M(S10, '=', S9, O27); M(S10, '"', S9, O27); M(S10, '(', S9, O27); M(S10, ')', S11, O27); M(S10, '[', S9, O27); M(S10, ']', S9, O27); M(S10, '{', S9, O27); M(S10, '}', S9, O27); M(S10, '.', S9, O27); M(S10, ',', S9, O27); M(S10, ':', S9, O27); M(S10, ';', S9, O27); M(S10, '+', S9, O27); M(S10, '-', S9, O27); M(S10, '*', S9, O27); M(S10, '/', S9, O27); M(S10, '#', S9, O27); M(S10, ' ', S9, O27);
//	M(S11, 'z', S1, O27); M(S11, 'D', S1, O27); M(S11, 'E', S1, O27); M(S11, '9', S1, O27); M(S11, '<', S1, O27); M(S11, '>', S1, O27); M(S11, '=', S1, O27); M(S11, '"', S1, O27); M(S11, '(', S1, O27); M(S11, ')', S1, O27); M(S11, '[', S1, O27); M(S11, ']', S1, O27); M(S11, '{', S1, O27); M(S11, '}', S1, O27); M(S11, '.', S1, O27); M(S11, ',', S1, O27); M(S11, ':', S1, O27); M(S11, ';', S1, O27); M(S11, '+', S1, O27); M(S11, '-', S1, O27); M(S11, '*', S1, O27); M(S11, '/', S1, O27); M(S11, '#', S1, O27); M(S11, ' ', S1, O27);
	M(S12, 'z', S1, O5); M(S12, 'D', S1, O5); M(S12, 'E', S1, O5); M(S12, '9', S1, O5); M(S12, '<', S1, O5); M(S12, '>', S1, O5); M(S12, '=', S13, O6); M(S12, '"', S1, O5); M(S12, '(', S1, O5); M(S12, ')', S1, O5); M(S12, '[', S1, O5); M(S12, ']', S1, O5); M(S12, '{', S1, O5); M(S12, '}', S1, O5); M(S12, '.', S1, O5); M(S12, ',', S1, O5); M(S12, ':', S1, O5); M(S12, ';', S1, O5); M(S12, '+', S1, O5); M(S12, '-', S1, O5); M(S12, '*', S1, O5); M(S12, '/', S1, O5); M(S12, '#', S1, O5); M(S12, ' ', S1, O5);
	M(S13, 'z', S1, O6); M(S13, 'D', S1, O6); M(S13, 'E', S1, O6); M(S13, '9', S1, O6); M(S13, '<', S1, O6); M(S13, '>', S1, O6); M(S13, '=', S1, O6); M(S13, '"', S1, O6); M(S13, '(', S1, O6); M(S13, ')', S1, O6); M(S13, '[', S1, O6); M(S13, ']', S1, O6); M(S13, '{', S1, O6); M(S13, '}', S1, O6); M(S13, '.', S1, O6); M(S13, ',', S1, O6); M(S13, ':', S1, O6); M(S13, ';', S1, O6); M(S13, '+', S1, O6); M(S13, '-', S1, O6); M(S13, '*', S1, O6); M(S13, '/', S1, O6); M(S13, '#', S1, O6); M(S13, ' ', S1, O6);
	M(S14, 'z', S1, O7); M(S14, 'D', S1, O7); M(S14, 'E', S1, O7); M(S14, '9', S1, O7); M(S14, '<', S1, O7); M(S14, '>', S1, O7); M(S14, '=', S15, O8); M(S14, '"', S1, O7); M(S14, '(', S1, O7); M(S14, ')', S1, O7); M(S14, '[', S1, O7); M(S14, ']', S1, O7); M(S14, '{', S1, O7); M(S14, '}', S1, O7); M(S14, '.', S1, O7); M(S14, ',', S1, O7); M(S14, ':', S1, O7); M(S14, ';', S1, O7); M(S14, '+', S1, O7); M(S14, '-', S1, O7); M(S14, '*', S1, O7); M(S14, '/', S1, O7); M(S14, '#', S1, O7); M(S14, ' ', S1, O7);
	M(S15, 'z', S1, O8); M(S15, 'D', S1, O8); M(S15, 'E', S1, O8); M(S15, '9', S1, O8); M(S15, '<', S1, O8); M(S15, '>', S1, O8); M(S15, '=', S1, O8); M(S15, '"', S1, O8); M(S15, '(', S1, O8); M(S15, ')', S1, O8); M(S15, '[', S1, O8); M(S15, ']', S1, O8); M(S15, '{', S1, O8); M(S15, '}', S1, O8); M(S15, '.', S1, O8); M(S15, ',', S1, O8); M(S15, ':', S1, O8); M(S15, ';', S1, O8); M(S15, '+', S1, O8); M(S15, '-', S1, O8); M(S15, '*', S1, O8); M(S15, '/', S1, O8); M(S15, '#', S1, O8); M(S15, ' ', S1, O8);
	M(S16, 'z', S1, O9); M(S16, 'D', S1, O9); M(S16, 'E', S1, O9); M(S16, '9', S1, O9); M(S16, '<', S1, O9); M(S16, '>', S1, O9); M(S16, '=', S17, O10); M(S16, '"', S1, O9); M(S16, '(', S1, O9); M(S16, ')', S1, O9); M(S16, '[', S1, O9); M(S16, ']', S1, O9); M(S16, '{', S1, O9); M(S16, '}', S1, O9); M(S16, '.', S1, O9); M(S16, ',', S1, O9); M(S16, ':', S1, O9); M(S16, ';', S1, O9); M(S16, '+', S1, O9); M(S16, '-', S1, O9); M(S16, '*', S1, O9); M(S16, '/', S1, O9); M(S16, '#', S1, O9); M(S16, ' ', S1, O9);
	M(S17, 'z', S1, O10); M(S17, 'D', S1, O10); M(S17, 'E', S1, O10); M(S17, '9', S1, O10); M(S17, '<', S1, O10); M(S17, '>', S1, O10); M(S17, '=', S1, O10); M(S17, '"', S1, O10); M(S17, '(', S1, O10); M(S17, ')', S1, O10); M(S17, '[', S1, O10); M(S17, ']', S1, O10); M(S17, '{', S1, O10); M(S17, '}', S1, O10); M(S17, '.', S1, O10); M(S17, ',', S1, O10); M(S17, ':', S1, O10); M(S17, ';', S1, O10); M(S17, '+', S1, O10); M(S17, '-', S1, O10); M(S17, '*', S1, O10); M(S17, '/', S1, O10); M(S17, '#', S1, O10); M(S17, ' ', S1, O10);
	M(S18, 'z', S1, O12); M(S18, 'D', S1, O12); M(S18, 'E', S1, O12); M(S18, '9', S1, O12); M(S18, '<', S1, O12); M(S18, '>', S1, O12); M(S18, '=', S1, O12); M(S18, '"', S1, O12); M(S18, '(', S1, O12); M(S18, ')', S1, O12); M(S18, '[', S1, O12); M(S18, ']', S1, O12); M(S18, '{', S1, O12); M(S18, '}', S1, O12); M(S18, '.', S1, O12); M(S18, ',', S1, O12); M(S18, ':', S1, O12); M(S18, ';', S1, O12); M(S18, '+', S1, O12); M(S18, '-', S1, O12); M(S18, '*', S1, O12); M(S18, '/', S1, O12); M(S18, '#', S1, O12); M(S18, ' ', S1, O12);
	M(S19, 'z', S1, O13); M(S19, 'D', S1, O13); M(S19, 'E', S1, O13); M(S19, '9', S1, O13); M(S19, '<', S1, O13); M(S19, '>', S1, O13); M(S19, '=', S1, O13); M(S19, '"', S1, O13); M(S19, '(', S1, O13); M(S19, ')', S1, O13); M(S19, '[', S1, O13); M(S19, ']', S1, O13); M(S19, '{', S1, O13); M(S19, '}', S1, O13); M(S19, '.', S1, O13); M(S19, ',', S1, O13); M(S19, ':', S1, O13); M(S19, ';', S1, O13); M(S19, '+', S1, O13); M(S19, '-', S1, O13); M(S19, '*', S1, O13); M(S19, '/', S1, O13); M(S19, '#', S1, O13); M(S19, ' ', S1, O13);
	M(S20, 'z', S1, O14); M(S20, 'D', S1, O14); M(S20, 'E', S1, O14); M(S20, '9', S1, O14); M(S20, '<', S1, O14); M(S20, '>', S1, O14); M(S20, '=', S1, O14); M(S20, '"', S1, O14); M(S20, '(', S1, O14); M(S20, ')', S1, O14); M(S20, '[', S1, O14); M(S20, ']', S1, O14); M(S20, '{', S1, O14); M(S20, '}', S1, O14); M(S20, '.', S1, O14); M(S20, ',', S1, O14); M(S20, ':', S1, O14); M(S20, ';', S1, O14); M(S20, '+', S1, O14); M(S20, '-', S1, O14); M(S20, '*', S1, O14); M(S20, '/', S1, O14); M(S20, '#', S1, O14); M(S20, ' ', S1, O14);
	M(S21, 'z', S1, O15); M(S21, 'D', S1, O15); M(S21, 'E', S1, O15); M(S21, '9', S1, O15); M(S21, '<', S1, O15); M(S21, '>', S1, O15); M(S21, '=', S1, O15); M(S21, '"', S1, O15); M(S21, '(', S1, O15); M(S21, ')', S1, O15); M(S21, '[', S1, O15); M(S21, ']', S1, O15); M(S21, '{', S1, O15); M(S21, '}', S1, O15); M(S21, '.', S1, O15); M(S21, ',', S1, O15); M(S21, ':', S1, O15); M(S21, ';', S1, O15); M(S21, '+', S1, O15); M(S21, '-', S1, O15); M(S21, '*', S1, O15); M(S21, '/', S1, O15); M(S21, '#', S1, O15); M(S21, ' ', S1, O15);
	M(S22, 'z', S35, O27); M(S22, 'D', S35, O27); M(S22, 'E', S35, O27); M(S22, '9', S35, O27); M(S22, '<', S35, O27); M(S22, '>', S35, O27); M(S22, '=', S35, O27); M(S22, '"', S35, O27); M(S22, '(', S35, O27); M(S22, ')', S35, O27); M(S22, '[', S35, O27); M(S22, ']', S35, O27); M(S22, '{', S35, O27); M(S22, '}', S23, O27); M(S22, '.', S35, O27); M(S22, ',', S35, O27); M(S22, ':', S35, O27); M(S22, ';', S35, O27); M(S22, '+', S35, O27); M(S22, '-', S35, O27); M(S22, '*', S35, O27); M(S22, '/', S35, O27); M(S22, '#', S35, O27); M(S22, ' ', S35, O27);
	M(S35, 'z', S35, O27); M(S35, 'D', S35, O27); M(S35, 'E', S35, O27); M(S35, '9', S35, O27); M(S35, '<', S35, O27); M(S35, '>', S35, O27); M(S35, '=', S35, O27); M(S35, '"', S35, O27); M(S35, '(', S35, O27); M(S35, ')', S35, O27); M(S35, '[', S35, O27); M(S35, ']', S35, O27); M(S35, '{', S35, O27); M(S35, '}', S23, O27); M(S35, '.', S35, O27); M(S35, ',', S35, O27); M(S35, ':', S35, O27); M(S35, ';', S35, O27); M(S35, '+', S35, O27); M(S35, '-', S35, O27); M(S35, '*', S35, O27); M(S35, '/', S35, O27); M(S35, '#', S35, O27); M(S35, ' ', S35, O27);
	M(S23, 'z', S1, O27); M(S23, 'D', S1, O27); M(S23, 'E', S1, O27); M(S23, '9', S1, O27); M(S23, '<', S1, O27); M(S23, '>', S1, O27); M(S23, '=', S1, O27); M(S23, '"', S1, O27); M(S23, '(', S1, O27); M(S23, ')', S1, O27); M(S23, '[', S1, O27); M(S23, ']', S1, O27); M(S23, '{', S1, O27); M(S23, '}', S1, O27); M(S23, '.', S1, O27); M(S23, ',', S1, O27); M(S23, ':', S1, O27); M(S23, ';', S1, O27); M(S23, '+', S1, O27); M(S23, '-', S1, O27); M(S23, '*', S1, O27); M(S23, '/', S1, O27); M(S23, '#', S1, O27); M(S23, ' ', S1, O27);
	M(S24, 'z', S1, O18); M(S24, 'D', S1, O18); M(S24, 'E', S1, O18); M(S24, '9', S1, O18); M(S24, '<', S1, O18); M(S24, '>', S1, O18); M(S24, '=', S1, O18); M(S24, '"', S1, O18); M(S24, '(', S1, O18); M(S24, ')', S1, O18); M(S24, '[', S1, O18); M(S24, ']', S1, O18); M(S24, '{', S1, O18); M(S24, '}', S1, O18); M(S24, '.', S34, O19); M(S24, ',', S1, O18); M(S24, ':', S1, O18); M(S24, ';', S1, O18); M(S24, '+', S1, O18); M(S24, '-', S1, O18); M(S24, '*', S1, O18); M(S24, '/', S1, O18); M(S24, '#', S1, O18); M(S24, ' ', S1, O18);
	M(S25, 'z', S1, O20); M(S25, 'D', S1, O20); M(S25, 'E', S1, O20); M(S25, '9', S1, O20); M(S25, '<', S1, O20); M(S25, '>', S1, O20); M(S25, '=', S1, O20); M(S25, '"', S1, O20); M(S25, '(', S1, O20); M(S25, ')', S1, O20); M(S25, '[', S1, O20); M(S25, ']', S1, O20); M(S25, '{', S1, O20); M(S25, '}', S1, O20); M(S25, '.', S1, O20); M(S25, ',', S1, O20); M(S25, ':', S1, O20); M(S25, ';', S1, O20); M(S25, '+', S1, O20); M(S25, '-', S1, O20); M(S25, '*', S1, O20); M(S25, '/', S1, O20); M(S25, '#', S1, O20); M(S25, ' ', S1, O20);
	M(S26, 'z', S1, O21); M(S26, 'D', S1, O21); M(S26, 'E', S1, O21); M(S26, '9', S1, O21); M(S26, '<', S1, O21); M(S26, '>', S1, O21); M(S26, '=', S1, O21); M(S26, '"', S1, O21); M(S26, '(', S1, O21); M(S26, ')', S1, O21); M(S26, '[', S1, O21); M(S26, ']', S1, O21); M(S26, '{', S1, O21); M(S26, '}', S1, O21); M(S26, '.', S1, O21); M(S26, ',', S1, O21); M(S26, ':', S1, O21); M(S26, ';', S1, O21); M(S26, '+', S1, O21); M(S26, '-', S1, O21); M(S26, '*', S1, O21); M(S26, '/', S1, O21); M(S26, '#', S1, O21); M(S26, ' ', S1, O21);
	M(S27, 'z', S1, O22); M(S27, 'D', S1, O22); M(S27, 'E', S1, O22); M(S27, '9', S1, O22); M(S27, '<', S1, O22); M(S27, '>', S1, O22); M(S27, '=', S1, O22); M(S27, '"', S1, O22); M(S27, '(', S1, O22); M(S27, ')', S1, O22); M(S27, '[', S1, O22); M(S27, ']', S1, O22); M(S27, '{', S1, O22); M(S27, '}', S1, O22); M(S27, '.', S1, O22); M(S27, ',', S1, O22); M(S27, ':', S1, O22); M(S27, ';', S1, O22); M(S27, '+', S1, O22); M(S27, '-', S1, O22); M(S27, '*', S1, O22); M(S27, '/', S1, O22); M(S27, '#', S1, O22); M(S27, ' ', S1, O22);
	M(S28, 'z', S1, O23); M(S28, 'D', S1, O23); M(S28, 'E', S1, O23); M(S28, '9', S1, O23); M(S28, '<', S1, O23); M(S28, '>', S1, O23); M(S28, '=', S1, O23); M(S28, '"', S1, O23); M(S28, '(', S1, O23); M(S28, ')', S1, O23); M(S28, '[', S1, O23); M(S28, ']', S1, O23); M(S28, '{', S1, O23); M(S28, '}', S1, O23); M(S28, '.', S1, O23); M(S28, ',', S1, O23); M(S28, ':', S1, O23); M(S28, ';', S1, O23); M(S28, '+', S1, O23); M(S28, '-', S1, O23); M(S28, '*', S1, O23); M(S28, '/', S1, O23); M(S28, '#', S1, O23); M(S28, ' ', S1, O23);
	M(S29, 'z', S1, O24); M(S29, 'D', S1, O24); M(S29, 'E', S1, O24); M(S29, '9', S1, O24); M(S29, '<', S1, O24); M(S29, '>', S1, O24); M(S29, '=', S1, O24); M(S29, '"', S1, O24); M(S29, '(', S1, O24); M(S29, ')', S1, O24); M(S29, '[', S1, O24); M(S29, ']', S1, O24); M(S29, '{', S1, O24); M(S29, '}', S1, O24); M(S29, '.', S1, O24); M(S29, ',', S1, O24); M(S29, ':', S1, O24); M(S29, ';', S1, O24); M(S29, '+', S1, O24); M(S29, '-', S1, O24); M(S29, '*', S1, O24); M(S29, '/', S1, O24); M(S29, '#', S1, O24); M(S29, ' ', S1, O24);
	M(S30, 'z', S1, O25); M(S30, 'D', S1, O25); M(S30, 'E', S1, O25); M(S30, '9', S1, O25); M(S30, '<', S1, O25); M(S30, '>', S1, O25); M(S30, '=', S1, O25); M(S30, '"', S1, O25); M(S30, '(', S1, O25); M(S30, ')', S1, O25); M(S30, '[', S1, O25); M(S30, ']', S1, O25); M(S30, '{', S1, O25); M(S30, '}', S1, O25); M(S30, '.', S1, O25); M(S30, ',', S1, O25); M(S30, ':', S1, O25); M(S30, ';', S1, O25); M(S30, '+', S1, O25); M(S30, '-', S1, O25); M(S30, '*', S1, O25); M(S30, '/', S1, O25); M(S30, '#', S1, O25); M(S30, ' ', S1, O25);
	M(S31, 'z', S1, O26); M(S31, 'D', S1, O26); M(S31, 'E', S1, O26); M(S31, '9', S1, O26); M(S31, '<', S1, O26); M(S31, '>', S1, O26); M(S31, '=', S1, O26); M(S31, '"', S1, O26); M(S31, '(', S1, O26); M(S31, ')', S1, O26); M(S31, '[', S1, O26); M(S31, ']', S1, O26); M(S31, '{', S1, O26); M(S31, '}', S1, O26); M(S31, '.', S1, O26); M(S31, ',', S1, O26); M(S31, ':', S1, O26); M(S31, ';', S1, O26); M(S31, '+', S1, O26); M(S31, '-', S1, O26); M(S31, '*', S1, O26); M(S31, '/', S1, O26); M(S31, '#', S1, O26); M(S31, ' ', S1, O26);
	M(S32, 'z', S32, O11); M(S32, 'D', S32, O11); M(S32, 'E', S32, O11); M(S32, '9', S32, O11); M(S32, '<', S32, O11); M(S32, '>', S32, O11); M(S32, '=', S32, O11); M(S32, '"', S33, O11); M(S32, '(', S32, O11); M(S32, ')', S32, O11); M(S32, '[', S32, O11); M(S32, ']', S32, O11); M(S32, '{', S32, O11); M(S32, '}', S32, O11); M(S32, '.', S32, O11); M(S32, ',', S32, O11); M(S32, ':', S32, O11); M(S32, ';', S32, O11); M(S32, '+', S32, O11); M(S32, '-', S32, O11); M(S32, '*', S32, O11); M(S32, '/', S32, O11); M(S32, '#', S32, O11); M(S32, ' ', S32, O11);
	M(S33, 'z', S1, O11); M(S33, 'D', S1, O11); M(S33, 'E', S1, O11); M(S33, '9', S1, O11); M(S33, '<', S1, O11); M(S33, '>', S1, O11); M(S33, '=', S1, O11); M(S33, '"', S1, O11); M(S33, '(', S1, O11); M(S33, ')', S1, O11); M(S33, '[', S1, O11); M(S33, ']', S1, O11); M(S33, '{', S1, O11); M(S33, '}', S1, O11); M(S33, '.', S1, O11); M(S33, ',', S1, O11); M(S33, ':', S1, O11); M(S33, ';', S1, O11); M(S33, '+', S1, O11); M(S33, '-', S1, O11); M(S33, '*', S1, O11); M(S33, '/', S1, O11); M(S33, '#', S1, O11); M(S33, ' ', S1, O11);
	M(S34, 'z', S1, O19); M(S34, 'D', S1, O19); M(S34, 'E', S1, O19); M(S34, '9', S1, O19); M(S34, '<', S1, O19); M(S34, '>', S1, O19); M(S34, '=', S1, O19); M(S34, '"', S1, O19); M(S34, '(', S1, O19); M(S34, ')', S1, O19); M(S34, '[', S1, O19); M(S34, ']', S1, O19); M(S34, '{', S1, O19); M(S34, '}', S1, O19); M(S34, '.', S1, O19); M(S34, ',', S1, O19); M(S34, ':', S1, O19); M(S34, ';', S1, O19); M(S34, '+', S1, O19); M(S34, '-', S1, O19); M(S34, '*', S1, O19); M(S34, '/', S1, O19); M(S34, '#', S1, O19); M(S34, ' ', S1, O19);
}

Lex::Lex(const char *fname) //Конструктор принимает на вход имя файла с входной цепочкой
{
	_file.open(fname); // Пытаемся открыть файл в поток
	if (!_file) throw LexErr("Error open file " + string(fname)); //Если неудача, возбуждаем исключительную ситуацию
	_file.unsetf(ios::skipws);
	//Установка признака конца файла
	_wasEOF = false;
    _curToken = 0;
}

Lex::~Lex()
{
	_file.close(); //Деструктор закрывает файловый поток
}


char Lex::getSClass(char symbol)
{
	//z = a-zA-Z - ED
	//9 = 0-9
	//' ' = ' ','\t','\r','\n'
	switch (symbol)
	{
	case 'E':
		return 'E';
	case 'D':
		return 'D';
	default:
		break;
	}
	if (isalpha(symbol))
		return 'z';
	else if (isdigit(symbol))
		return '9';
	else if (isspace(symbol))
		return ' ';
	else
		return symbol;
}

bool Lex::GetToken(Token &token)
{
	Statement current = S0; //Начальное состояние - S0
	TokenType type;
	string value;
	Result res;
	char c;
	char cl;
	map<Input, Result>::const_iterator it;
	map<string, TokenType>::const_iterator it_kw;
	while (!_file.eof()) //Пока не конец файла
	{
		_file.get(c); // Получаем очередной символ
		cl = getSClass(c); // Получаем класс символа
		it = M.find(Input(current, cl)); // Пытаемся найти ячейку таблицы, соответствующую ключу Текущее Состояние current, Входной Символ сl
		if (it == M.end()) throw LexErr(string("Incorret Symbol") + c); // Если такой ячейки нет, возбуждаем исключительную ситуацию
		res = it->second; // Получаем пару (Новое Состояние & Выходной сигнал)
		if (res.second == O29) throw LexErr("Lexical error"); // Если выходной сигнал O29 - ошибка
		if (res.first == S1 || _file.peek() <= 0) //Если следующее состояние финальное или "виден" конец файла
		{
			if (res.first != S1) //Текущее состояние финальное
			{
				value += c;
				_wasEOF = true;
			}
			switch (res.second) //Выходной сигнал
			{
			case O1:
				type = IDENT;
				break;
			case O2:
				type = DEC;
				break;
			case O3:
				type = REAL;
				break;
			case O4:
				type = LPAR;
				break;
			case O5:
				type = LT;
				break;
			case O6:
				type = LTE;
				break;
			case O7:
				type = GT;
				break;
			case O8:
				type = GTE;
				break;
			case O9:
				type = COLON;
				break;
			case O10:
				type = ASSIGN;
				break;
			case O11:
				type = STRCONST;
				break;
			case O12:
				type = EQ;
				break;
			case O13:
				type = RPAR;
				break;
			case O14:
				type = LSBR;
				break;
			case O15:
				type = RSBR;
				break;
			case O16:
				type = LCBR;
				break;
			case O17:
				type = RCBR;
				break;
			case O18:
				type = DOT;
				break;
			case O19:
				type = RANGE;
				break;
			case O20:
				type = COMMA;
				break;
			case O21:
				type = SEMICOLON;
				break;
			case O22:
				type = PLUS;
				break;
			case O23:
				type = MINUS;
				break;
			case O24:
				type = MUL;
				break;
			case O25:
				type = DIV;
				break;
			case O26:
				type = NEQ;
				break;
			case O27:
				type = COMMENT;
				break;
			default:
				throw LexErr("Lexical error");
			}
			// Формируем лексему
			token._value = trim(value);
			if (type == IDENT)
			{
				it_kw = KW.find(token._value); // Пытаемся найти ключевое слово
				if (it_kw != KW.end())
					type = it_kw->second; // Устанавливаем тип лексемы - найденное ключевое слово
				
			}
			token._type = type;
			if(!_wasEOF)
				_file.seekg(-1, ios::cur); //"Возвращаем захваченный нами лишний символ"
			if (type == COMMENT)
			{
				value = ""; //Сбрасываем строковое значение лексемы
				value += c; //Накапливаем строковое значение
				current = S0; // Меняем текущее состояние на начальное
				continue; //Пропускаем конец итерации и приступаем к поиску следующей лексемы
			}
			return true;
		}
		current = res.first; // Меняем текущее состояние
		value += c; //Накапливаем строковое значение
	}
	//Формируем лексему EOF (end of file)
	token._type = EOFILE;
	token._value = "";
	return false;
}

void Lex::GetTokens()
{
    Token token;

    while (GetToken(token))
    {
        _tokens.push_back(token);
    }
}

void Lex::PrintTokens()
{
    cout << "Table of token:" << endl;

    for (const auto& token : _tokens)
    {
        cout << "Token: Lexeme '" << token._value << "' with type " << token._type << endl;
    }
}

void Lex::NextToken()
{
    if (_curToken < _tokens.size() - 1)
    {
        _curToken++;
    }
}

void Lex::PrevToken()
{
    if (_curToken > 0)
    {
        _curToken--;
    }
}

bool Lex::LastToken()
{
	if (_curToken == _tokens.size()) return true;
	else return false;
 }

string Lex::CurrentTokenValue()
{
    return _tokens[_curToken]._value;
}

TokenType Lex::CurrentTokenType()
{
    return _tokens[_curToken]._type;
}
