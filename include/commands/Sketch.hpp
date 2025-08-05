#pragma once

#include "util/Command.hpp"

struct Sketch : Command {
    Sketch();
    virtual void exec(Image &img, const char *path, std::vector<double> args) override;
};