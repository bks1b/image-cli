#include <iostream>
#include <algorithm>

#include "util/main.hpp"

#include "commands/Sketch.hpp"
#include "commands/Ascii.hpp"

int main(int argc, char **argv) {
    Sketch sketch;
    Ascii ascii;
    std::vector<Command*> commands = { &sketch, &ascii };

    doubles_t params;
    flags_t flags;
    auto first = read_args(argc, argv, params, flags);
    if (first.size() > 0 && first[0] == "readme") {
        write_readme(commands);
        return 0;
    }
    auto found = first.size() < 1 ? commands.end() : std::find_if(commands.begin(), commands.end(), [&first](Command *cmd) {
        return cmd->columns[0] == first[0];
    });
    if (found == commands.end()) {
        if (first.size() > 0) std::cout << "Command not found.\n";
        ascii_table(commands, std::cout, true);
        return 0;
    }
    if (first.size() < 3) throw std::runtime_error(std::string(first.size() == 2 ? "Out" : "In") + "put path expected.");

    if (params.size() > (**found).def_params.size()) throw std::runtime_error("At most " + std::to_string((**found).def_params.size()) + " command arguments expected.");
    for (int i = params.size(); i < (**found).def_params.size(); i++) params.push_back((**found).def_params[i]);
    Image img;
    img.read(first[1]);
    (**found).exec(img, first[2], params, flags);
    return 0;
}