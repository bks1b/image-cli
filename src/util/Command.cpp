#include "util/Command.hpp"

Command::Command(std::string name, std::string desc, std::string params_desc, doubles_t params): columns({ name, desc, params_desc }), def_params(params) {}

void Command::exec(Image &img, std::string &path, doubles_t &params, flags_t &flags) {}