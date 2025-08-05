#pragma once

#include "util/Command.hpp"

struct Ascii : Command {
    Ascii();
    virtual void exec(Image &img, const char *path, std::vector<double> args) override;
};