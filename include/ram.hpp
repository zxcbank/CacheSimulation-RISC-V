//
// Created by Святослав on 10.05.2024.
//

#define CACHE_SIZE 4096
#define MEM_SIZE 262144
#pragma once
#include <iostream>
#include <vector>
#include <cstdint>

class RAM {
    public:
        std::string name;
        std::vector<int8_t> addr = std::vector<int8_t>(MEM_SIZE, 0);
        
        [[nodiscard]] RAM(std::string name) : name(std::move(name)) {}
        
        int8_t& operator [](int i) {
            
            return addr.at(i);
        }
        
        ~RAM() = default;
};


