#pragma once

#include "util/Command.hpp"

struct Sketch : Command {
    Sketch();
    virtual void exec(Image &img, std::string &path, doubles_t &params, flags_t &flags) override;
};