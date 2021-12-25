// NIR_refactoring.cpp : Этот файл содержит функцию "main". 
// Здесь начинается и заканчивается выполнение программы.
//

#include <clocale>
#include <iostream>
using namespace std;

#include "RefactoringManager.h"

/// <summary>Ввод пути к файлу исходного кода</summary>
/// <returns>путь к файлу исходного кода</returns>
string GetFilePath() {
    string filePath = "";
    cout << "Введите полное имя файла (включающее полный путь)\n";
    cin >> filePath;
    return filePath;
}

/// <summary>Главная функция программы</summary>
/// <param name="argc">- количество аргументов командной строки</param>
/// <param name="argv">- аргументы командной строки</param>
/// <returns>0</returns>
int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    string filePath = "";
    filePath = argc > 1 ? argv[1] : GetFilePath();
    RefactoringManager refactoringManager = RefactoringManager(filePath);
    refactoringManager.Refactoring();
    system("pause");
    return 0;
}