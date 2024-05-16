//
// Created by Святослав on 11.05.2024.
//
#pragma once
#include <vector>
#include <cstdint>

#define CACHE_SIZE 4096
#define CACHE_SETS 32
#define CACHE_WAY 4

class CacheLine {
    private:
        int tag;
        int time;
    public:
        explicit CacheLine(int tag, int data, int time_) : tag(tag), time(time_) {}
        CacheLine() : tag(-1), time(-1) {}
        
        ~CacheLine() = default;
        [[nodiscard]] int getTag() const {
            return tag;
        }
        [[nodiscard]] int getTime() const {
            return time;
        }
         void setTime(int time_) {
            this->time = time_;
        }
        
         void setTag(int tag_) {
            this->tag = tag_;
        }
};

class LRU {
    private:
        std::vector<std::vector<CacheLine>> cache = std::vector<std::vector<CacheLine>>(CACHE_SETS);
    public:
        int cacheHit = 0;
        int cacheAccess = 0;
        LRU();
        bool checkLRU(int addr, int time_);
        int findLeastRecentlyUsed(int index);
        ~LRU() = default;
        
        [[nodiscard]]float hitRate() const;
};
