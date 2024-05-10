
#include "ArgParser.h"

ArgParser& ArgParser::parse(int argc, char** argv) {
    for (int i = 0; i < argc; ++i) {
       if (strcmp(argv[i], "--replacement") == 0) {
           if (i + 1 < argc) {
               replacement = std::stoi(argv[++i]);
           } else {
               std::cout << "No replacement argument \n";
           }
       }
       if (strcmp(argv[i], "--asm") == 0) {
           if (i + 1 < argc) {
               asm_fname = argv[++i];
           } else {
               std::cout << "No asm argument \n";
           }
       }
       if (strcmp(argv[i], "--bin") == 0) {
           if (i + 1 < argc) {
               bin = argv[++i];
           } else {
               std::cout << "No bin argument \n";
           }
       }
    }
    
    return *this;
}

const std::string& ArgParser::GetName() const {
    return name;
}

const std::string& ArgParser::GetBin() const {
    return bin;
}

const std::string& ArgParser::GetAsmFname() const {
    return asm_fname;
}

int ArgParser::GetReplacement() const {
    return replacement;
}
