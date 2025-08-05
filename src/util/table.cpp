#include <iostream>
#include <ostream>
#include <algorithm>

#include "util/Command.hpp"

void ascii_table(std::vector<Command*> &commands, std::ostream &stream, bool header) {
    Command first("Name", "Description", "Parameters", {});
    commands.insert(commands.begin(), &first);
    std::vector<int> widths = {};
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