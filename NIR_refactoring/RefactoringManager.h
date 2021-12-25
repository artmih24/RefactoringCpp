// файл RefactoringManager.h - объявление класса RefactoringManager

#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "CppCode.h"
#include <iostream>
using namespace std;

/// <summary>Класс, предназначенный для проведения рефакторинга</summary>
class RefactoringManager {
private:
    string filePath,            // полный путь к исходному файлу
        outFilePath,            // полный путь к новому файлу
        fileContent;            // строка, содержащая код

    ifstream fileStream;        // поток чтения из файла

    ofstream outFileStream;     // поток записи в файл

    stringstream stringStream,  // поток для построчного считывания
        outStringStream;        // поток для построчной записи

    CppCode code;               // код на С/С++, рефакторинг которого требуется провести

public:
    /// <summary>Конструктор объекта класса RefactoringManager</summary>
    /// <param name="filePath">- путь к файлу с исходным кодом</param>
    RefactoringManager(string filePath);

    /// <summary>Проведение рефакторинга</summary>
    void Refactoring();

    /// <summary>Проведение рефакторинга методом удаления присваивний параметрам</summary>
    /// <param name="code">- исходный код программы</param>
    void RemoveAssignmentsToParametersRefactoring(CppCode code);

    /// <summary>Удаление присваиваний параметрам метода</summary>
    /// <param name="method">- изначальный метод</param>
    /// <returns>новый метод</returns>
    CppMethod RemoveAssignmentsToParameters(CppMethod method);

    /// <summary>Проведение рефакторинга методом удаления параметров</summary>
    /// <param name="code">- исходный код программы</param>
    void RemoveParametersRefactoring(CppCode code);

    /// <summary>Удаление параметров метода</summary>
    /// <param name="method">- изначальный метод</param>
    /// <returns>новый метод</returns>
    CppMethod RemoveParameters(CppMethod method);

    /// <summary>Составление названия файла, в котором будет содержаться измененный код</summary>
    /// <param name="filePath">- полное название исходного файла</param>
    /// <returns>название файла, в котором будет содержаться измененный код</returns>
    string GetOutputFileName(string filePath);

    /// <summary>Чтение файла</summary>
    /// <param name="filePath">- полное название файла</param>
    void GetFileContent(string filePath);

    /// <summary>Запись в файл</summary>
    /// <param name="filePath">- полное название файла</param>
    void WriteFile(string filePath);
};