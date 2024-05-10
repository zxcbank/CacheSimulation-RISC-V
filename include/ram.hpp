//
// Created by Святослав on 10.05.2024.
//

#define CACHE_SIZE 4096
#define MEM_SIZE 262144
#pragma once
#include <iostream>
#include <vector>


class RAM {
    public:
        std::string name;
        std::vector<int> addr = std::vector<int>(MEM_SIZE, 0);
        
        [[nodiscard]] RAM(std::string name) : name(std::move(name)) {}
        
        int& operator [](int i) {
            
            return addr.at(i);
        }
        
        ~RAM() = default;
};


