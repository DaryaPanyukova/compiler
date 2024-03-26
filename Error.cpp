#include "Error.h"

std::ostream& operator<<(std::ostream& os, const Error& err) {
    return os << err.text;
}

Error::Error() {

}

Error::Error(std::string str) : text(std::move(str)) {

}

Error::Error(int row, int col, std::string str) {
    std::string format = "Error:%d:%d - %s\n";
    int size_s = std::snprintf( nullptr, 0, format.c_str(), row, col, str.c_str()) + 1;
    text.resize(size_s);
    std::sprintf(&text[0], format.c_str(), row, col, str.c_str());
}

Error::Error(int row, std::string str) {
    std::string format = "Error:%d - %s\n";
    int size_s = std::snprintf( nullptr, 0, format.c_str(), row, str.c_str()) + 1;
    text.resize(size_s);
    std::sprintf(&text[0], format.c_str(), row, str.c_str());
}

ExpectedSymbol::ExpectedSymbol(int row, int col) : Error() {
    std::string format = "SyntaxError:%d:%d - Unexpected symbol\n";
    int size_s = std::snprintf( nullptr, 0, format.c_str(), row, col) + 1;
    text.resize(size_s);
    std::sprintf(&text[0], format.c_str(), row, col);
}

ExpectedSymbol::ExpectedSymbol(int row, int col, const char *expected, const char *current) {
    std::string format = "SyntaxError:%d:%d - Expected symbol '%s' while '%s' was written\n";
    int size_s = std::snprintf( nullptr, 0, format.c_str(), row, col, expected, current) + 1;
    text.resize(size_s);
    std::sprintf(&text[0], format.c_str(), row, col, expected, current);
}

ExpectedSymbol::ExpectedSymbol(int row, int col, const char *expected) {
    std::string format = "SyntaxError:%d:%d - Unexpected symbol '%s'\n";
    int size_s = std::snprintf( nullptr, 0, format.c_str(), row, col, expected) + 1;
    text.resize(size_s);
    std::sprintf(&text[0], format.c_str(), row, col, expected);
}
