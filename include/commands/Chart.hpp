#pragma once

#include "util/Command.hpp"

struct Chart : Command {
    Chart();
    virtual void exec(Image &img, std::string &path, doubles_t &params, flags_t &flags) override;
};