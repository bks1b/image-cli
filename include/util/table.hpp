#pragma once

#include <ostream>

#include "util/Command.hpp"

void ascii_table(std::vector<Command*> &commands, std::ostream &stream, bool header);