//
// Created by Matwey on 20/01/2022.
//

#include "Logger.h"
#include "Error.h"


void Logger::set_file(const std::string &file_path) {
    auto& out_file = instance().out_file;
    out_file.open(file_path, std::ios::binary | std::ios::ate);
    if (!out_file.is_open()) {
        throw Error("Problems with opening file " + file_path);
    }
}

void Logger::_log_(const std::string &str) {
    std::time_t t = std::time(nullptr);
    std::tm* data = localtime(&t);
    char buffer[80];
    std::string format = "%B %d, %Y %H:%M:%S";
    strftime(buffer, 80, format.c_str(), data);

    out_file << buffer << " message: " << str << std::endl;
}

void Logger::log(const std::string &str) {
    instance()._log_(str);
}

Logger::Logger() {
    out_file.open("log.txt", std::ios::binary | std::ios::ate);
    if (!out_file.is_open()) {
        throw Error("Problems with opening file log.txt");
    }
}
