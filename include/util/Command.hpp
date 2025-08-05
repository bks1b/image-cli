#pragma once

#include "images/Image.hpp"

struct Command {
    Command(std::string name, std::string desc, std::string arg_desc, std::vector<double> args);
    virtual void exec(Image &img, const char *path, std::vector<double> args);
    std::vector<std::string> columns;
    std::vector<double> def_args;
};