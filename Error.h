//
// Created by Matwey on 13/01/2022.
//

#ifndef COMPILER_ERROR_H
#define COMPILER_ERROR_H

#include <iostream>
#include <exception>

class Error : public std::exception {
public:
    Error();
    Error(std::string str);
    Error(int row, int col, std::string str);
    Error(int row, std::string str);

protected:
    std::string text;

    friend std::ostream& operator<<(std::ostream& os, const Error& err);
};

class ExpectedSymbol : public Error {
public:
    ExpectedSymbol(int row, int col);
    ExpectedSymbol(int row, int col, const char* expected);
    ExpectedSymbol(int row, int col, const char* expected, const char* current);
};



#endif //COMPILER_ERROR_H
