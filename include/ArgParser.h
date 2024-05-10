#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstring>

struct arg {
    std::string name;
    std::string value;
    bool enable = false;
};

class ArgParser {
    private:
        std::string name;
        std::string bin;
        std::string asm_fname;
        int replacement{};
        
    public:
        ArgParser(std::string name) : name(std::move(name)) {}
        
        ArgParser& parse(int argc, char **argv);
        
        const std::string& GetName() const;
        const std::string& GetBin() const;
        const std::string& GetAsmFname() const;
        int GetReplacement() const;
};