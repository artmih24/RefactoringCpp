// файл CppCode.h - объявление класса CppCode

#pragma once
#include <string>
#include <vector>
#include <clocale>
#include <sstream>
using namespace std;

#include "CppMethod.h"


#include <iostream> // <-- для отладки, потом убрать

/// <summary>Класс кода</summary>
class CppCode {
private:
    string cppCode;                 // строка, содержащая код

public:
    vector<string> lexemes;         // список лексем кода

    vector<CppMethod> methods;      // список методов кода

    /// <summary>Конструктор по умолчанию</summary>
    CppCode();

    /// <summary>Конструктор класса</summary>
    CppCode(string cppCode);

    /// <summary>Дополнительные действия для разбиения кода на лексемы</summary>
    string Modify();

    /// <summary>Разбиение кода на лексемы</summary>
    vector<string> GetLexemes();

    /// <summary>Получение типов данных, описанных в коде</summary>
    /// <returns>- вектор пользовательских типов данных</returns>
    vector<string> GetTypes(string type);

    /// <summary>Получение вектора методов</summary>
    /// <returns>- вектор методов в коде</returns>
    vector<CppMethod> GetMethods();

    void UpdateMethods(vector<CppMethod> methods);

    void UpdateLexemes();

    string ToString();
};