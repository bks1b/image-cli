#pragma once

#include <ostream>

#include "util/Command.hpp"

std::vector<std::string> read_args(int argc, char **argv, doubles_t &params, flags_t &flags);

void ascii_table(std::vector<Command*> &commands, std::ostream &stream, bool header);

void write_readme(std::vector<Command*> &commands);