//
// Created by Святослав on 12.05.2024.
//
#pragma once
#include <vector>
#include <cstdint>

#define CACHE_SETS 32
#define CACHE_WAY 4

class bitCacheLine {
    private:
        int tag;
        bool active = false;
    public:
        explicit bitCacheLine(uint64_t tag, uint64_t data, bool act_) : tag(tag), active(act_) {}
        bitCacheLine() : tag(-1), active(false) {}
        ~bitCacheLine() = default;
        
        [[nodiscard]] uint64_t getTag() const {
            return tag;
        }
        [[nodiscard]] bool getAct() const {
            return active;
        }
        void setAct(bool act_) {
            this->active = act_;
        }
        
        void setTag(uint64_t tag_) {
            this->tag = tag_;
        }
};

class cashset {
    private:
        std::vector<bitCacheLine> cache = std::vector<bitCacheLine>(CACHE_WAY);
    public:
        cashset() = default;
        ~cashset() = default;
        
        [[nodiscard]] bitCacheLine& operator[](int index) {
            return cache[index];
        }
};

class PLRU {
    private:
        std::vector<cashset> cache = std::vector<cashset>(CACHE_SETS);
    public:
        int cacheHit = 0;
        int cacheAccess = 0;
        
        PLRU() = default;
        bool checkPLRU(int addr, int time_);
        int findLeastRecentlyUsed(int index);
        ~PLRU() = default;
        
        [[nodiscard]]float hitRate() const;
        
};
