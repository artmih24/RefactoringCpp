// файл CppMethod.h - объ€вление класса CppMethod

#pragma once

#include <cstring>
#include <vector>
using namespace std;

#include "Parameter.h"

class CppMethod {
private:
    vector<string> lexemes;

public:
    vector<Parameter> parameters;
    vector<string> body;
    string type,
        name;

    CppMethod();
    CppMethod(vector<string> lexemes);

    vector<Parameter> GetMethodParameters();

    vector<string> GetMethodBody();

    string GetMethodType();

    string GetMethodName();

    vector<string> ToLexemes();
};
