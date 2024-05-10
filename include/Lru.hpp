//
// Created by Святослав on 11.05.2024.
//
#pragma once
#include <vector>
#include <cstdint>

#define CACHE_SIZE 4096 // byte
#define CACHE_SETS 32
#define CACHE_WAY 4

class CacheLine {
    private:
        uint64_t tag;
        int time;
        uint64_t data;
    public:
        explicit CacheLine(uint64_t tag, uint64_t data, int time_) : tag(tag), data(data), time(time_) {}
        CacheLine() : tag(0), data(0), time(0) {}
        ~CacheLine() = default;
        
        [[nodiscard]] uint64_t getTag() const {
            return tag;
        }
        [[nodiscard]] uint64_t getData() const {
            return data;
        }
        [[nodiscard]] int getTime() const {
            return time;
        }
         void setTime(int time_) {
            this->time = time_;
        }
        
         void setData(uint64_t data_) {
            this->data = data_;
        }
        
         void setTag(uint64_t tag_) {
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
        void checkLRU(int index, int tag, int time_);
        int findLeastRecentlyUsed(int index);
        ~LRU() = default;
        
        [[nodiscard]]float hitRate() const;
};
