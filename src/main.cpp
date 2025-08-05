#include <iostream>
#include <fstream>
#include <algorithm>

#include "util/table.hpp"
#include "commands/Sketch.hpp"
#include "commands/Ascii.hpp"

void write_readme(std::vector<Command*> &commands) {
    std::ifstream readme_in("readme/README.md");
    std::ofstream readme_out("README.md");
    std::string line;
    while (std::getline(readme_in, line)) {
        if (line == "{commands}\r") ascii_table(commands, readme_out, false);
        else readme_out << line << "\n";
    }
    readme_out.close();
    readme_in.close();
}

int main(int argc, char *argv[]) {
    Sketch sketch;
    Ascii ascii;
    std::vector<Command*> commands = { &sketch, &ascii };
    if (argc > 1 && argv[1] == std::string("readme")) {
        write_readme(commands);
        return 0;
    }
    auto found = argc == 1 ? commands.end() : std::find_if(commands.begin(), commands.end(), [argv](Command *cmd) {
        return cmd->columns[0] == argv[1];
    });
    if (found == commands.end()) {
        if (argc > 1) std::cout << "Command not found.\n";
        ascii_table(commands, std::cout, true);
        return 0;
    }
    if (argc < 4) {
        std::cout << (argc == 3 ? "Out" : "In") << "put path expected.\n";
        return 0;
    }
    std::vector<double> args = (**found).def_args;
    if (argc - 4 > args.size()) {
        std::cout << "At most " << args.size() << " command arguments expected.\n";
        return 0;
    }
    for (int i = 4; i < argc; i++) {
        try {
            args[i - 4] = std::stod(argv[i]);
        } catch (const std::invalid_argument &e) {
            std::cout << "Failed to parse argument as number.\n";
            return 0;
        }
    }
    Image img;
    try {
        img.read(argv[2]);
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << "\n";
        return 0;
    }
    (**found).exec(img, argv[3], args);
    return 0;
}