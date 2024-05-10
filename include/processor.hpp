//
// Created by Святослав on 10.05.2024.
//
#pragma once
#include <vector>
#include <iostream>
#include <map>
#include "regesters.hpp"
#include "fstream"
#include "RiskVcommand.hpp"
#include "LRU.hpp"
#include "PLRU.hpp"

class Processor {
        std::ifstream asm_file;
        std::vector<int> registers = std::vector<int>(32, 0);
        RAM memory = RAM("dd4");
        std::map<std::string, int> labels;
        LRU lru_cache;
        
        PLRU plru_cache;
        
        int replacement;
        
    public:
        std::string name;
        [[nodiscard]] explicit Processor(std::string name, int repl) : name(std::move(name)), replacement(repl) {
        
        }
        void execute(const std::string& filename);
        LRU& getLRU() {
            return lru_cache;
        }
        PLRU& getPLRU() {
            return plru_cache;
        }
        void getStats() const ;
        ~Processor() = default;
};

bool is_emply(const std::string& reg);