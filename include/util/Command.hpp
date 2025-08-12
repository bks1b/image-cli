#pragma once

#include "images/Image.hpp"

struct Command {
    Command(std::string name, std::string desc, std::string params_desc, doubles_t params);
    virtual void exec(Image &img, std::string &path, doubles_t &params, flags_t &flags);
    std::vector<std::string> columns;
    doubles_t def_params;
};