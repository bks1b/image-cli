#include <fstream>
#include <ostream>
#include <algorithm>
#include <stdexcept>

#include "util/Command.hpp"

std::vector<std::string> read_args(int argc, char **argv, doubles_t &params, flags_t &flags) {
    std::vector<std::string> first;
    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if (arg.starts_with("--")) {
            std::size_t eq_i = arg.find('=');
            if (eq_i == std::string::npos) {
                eq_i = arg.length();
                arg += "=";
            }
            flags[arg.substr(2, eq_i - 2)] = arg.substr(eq_i + 1);
        } else if (first.size() < 3) first.push_back(arg);
        else {
            try {
                params.push_back(std::stod(arg));
            } catch (const std::invalid_argument &e) {
                throw std::runtime_error("Failed to parse argument as number.");
            }
        }
    }
    return first;
}

void ascii_table(std::vector<Command*> &commands, std::ostream &stream, bool header) {
    Command first("Name", "Description", "Parameters", {});
    commands.insert(commands.begin(), &first);
    ints_t widths = {};
    int total_width = -1;
    for (int i = 0; i < commands[0]->columns.size(); i++) {
        int w = (**std::max_element(commands.begin(), commands.end(), [i](Command *a, Command *b) {
            return a->columns[i].length() < b->columns[i].length();
        })).columns[i].length();
        widths.push_back(w);
        total_width += w + 3;
    }

    if (header) stream << "." << std::string(total_width, '-') << ".\n";
    for (auto cmd : commands) {
        stream << "| ";
        for (int i = 0; i < cmd->columns.size(); i++) stream << cmd->columns[i] << std::string(widths[i] - cmd->columns[i].length(), ' ') << " | ";
        stream << "\n";
        if (cmd->columns[0] == "Name") {
            stream << "|";
            for (int i = 0; i < cmd->columns.size(); i++) {
                stream << std::string(widths[i] + 2, '-') << "|";
            }
            stream << "\n";
        }
    }
    if (header) stream << "'" << std::string(total_width, '-') << "'\n";
}

void write_readme(std::vector<Command*> &commands) {
    std::ifstream readme_in("readme/README.md");
    std::ofstream readme_out("README.md");
    std::string line;
    while (std::getline(readme_in, line)) {
        if (line == "{commands}\r" || line == "{commands}") ascii_table(commands, readme_out, false);
        else readme_out << line << "\n";
    }
    readme_out.close();
    readme_in.close();
}