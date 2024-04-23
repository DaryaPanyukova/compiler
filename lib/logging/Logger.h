//
// Created by Matwey on 20/01/2022.
//

#ifndef COMPILER_LOGGER_H
#define COMPILER_LOGGER_H

#include <fstream>
#include <ctime>

class Logger {
public:
    static Logger& instance() {
        static Logger item = Logger();
        return item;
    }
    static void set_file(const std::string &file_path);
    static void log(const std::string& str);

private:
    Logger();

    void _log_(const std::string& str);

    std::ofstream out_file;
};


#endif //COMPILER_LOGGER_H
