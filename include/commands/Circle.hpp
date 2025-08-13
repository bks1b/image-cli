#pragma once

#include "util/Command.hpp"

struct Circle : Command {
    Circle();
    virtual void exec(Image &img, std::string &path, doubles_t &params, flags_t &flags) override;
};