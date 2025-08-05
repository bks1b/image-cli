#include "util/Command.hpp"

Command::Command(std::string name, std::string desc, std::string arg_desc, std::vector<double> args): columns({ name, desc, arg_desc }), def_args(args) {}

void Command::exec(Image &img, const char *path, std::vector<double> args) {}