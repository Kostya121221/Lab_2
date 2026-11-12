#include "set.h"

#include <fstream>
#include <filesystem>
#include <system_error>
#include <iostream>
#include <sstream>
#include <string>


bool loadSet(const std::string& path, IntSet& set) {
    if (path.empty()) return true;
    std::ifstream input(path);
    if (!input) {
        std::error_code error;
        const bool exists = std::filesystem::exists(path, error);
        return !exists && !error;
    }

    std::string line;
    while (std::getline(input, line)) {
        std::istringstream lineInput(line);
        std::string token;
        if (!(lineInput >> token)) continue;
        int value = 0;
        try {
            std::size_t parsed = 0;
            value = std::stoi(token, &parsed);
            if (parsed != token.size()) return false;
        } catch (...) {
            return false;
        }
        std::string extra;
        if (lineInput >> extra) return false;
        setAdd(&set, value);
    }
    return !input.bad();
}

bool saveSet(const std::string& path, const IntSet& set) {
    if (path.empty()) return true;
    std::ofstream output(path, std::ios::trunc);
    if (!output) return false;
    for (int value : setElements(&set)) {
        output << value << '\n';
    }
    output.flush();
    return output.good();
}

bool executeQuery(IntSet& set, const std::string& query, std::ostream& output, bool& changed) {
    changed = false;
    std::istringstream input(query);
    std::string command;
    int value;
    std::string extra;
    if (!(input >> command >> value) || (input >> extra)) {
        output << "ERROR\n";
        return false;
    }

    if (command == "SETADD") {
        setAdd(&set, value);
        output << "OK\n";
        changed = true;
        return true;
    }
    if (command == "SETDEL") {
        setRemove(&set, value);
        output << "OK\n";
        changed = true;
        return true;
    }
    if (command == "SET_AT") {
        output << (setContains(&set, value) ? "YES" : "NO") << '\n';
        return true;
    }
    output << "ERROR\n";
    return false;
}


int main(int argc, char* argv[]) {
    std::string filePath;
    std::string query;
    bool querySpecified = false;
    for (int i = 1; i < argc; ++i) {
        const std::string argument = argv[i];
        if (argument == "--file" && i + 1 < argc) filePath = argv[++i];
        else if (argument == "--query" && i + 1 < argc) {
            query = argv[++i];
            querySpecified = true;
        }
        else {
            std::cerr << "Unknown or incomplete argument: " << argument << '\n';
            return 2;
        }
    }

    IntSet set{};
    if (!loadSet(filePath, set)) {
        std::cerr << "Cannot load data file or invalid integer data\n";
        setDestroy(&set);
        return 1;
    }
    if (querySpecified) {
        bool changed = false;
        if (!executeQuery(set, query, std::cout, changed)) {
            setDestroy(&set);
            return 2;
        }
        if (changed && !saveSet(filePath, set)) {
            std::cerr << "Cannot save data file\n";
            setDestroy(&set);
            return 1;
        }
        setDestroy(&set);
        return 0;
    }

    std::string line;
    while (std::getline(std::cin, line)) {
        bool changed = false;
        if (!executeQuery(set, line, std::cout, changed)) continue;
        if (changed && !saveSet(filePath, set)) {
            std::cerr << "Cannot save data file\n";
            setDestroy(&set);
            return 1;
        }
    }
    setDestroy(&set);
    return 0;
}
