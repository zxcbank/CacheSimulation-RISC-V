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
        uint64_t tag;
        bool active = false;
        uint64_t data;
    public:
        explicit bitCacheLine(uint64_t tag, uint64_t data, bool act_) : tag(tag), data(data), active(act_) {}
        bitCacheLine() : tag(0), data(0), active(false) {}
        ~bitCacheLine() = default;
        
        [[nodiscard]] uint64_t getTag() const {
            return tag;
        }
        [[nodiscard]] uint64_t getData() const {
            return data;
        }
        [[nodiscard]] bool getAct() const {
            return active;
        }
        void setTime(bool act_) {
            this->active = act_;
        }
        
        void setData(uint64_t data_) {
            this->data = data_;
        }
        
        void setTag(uint64_t tag_) {
            this->tag = tag_;
        }
};

class cashset {
    private:
        std::vector<bitCacheLine> cache = std::vector<bitCacheLine>(CACHE_WAY);
    public:
        int filled = 0;
        cashset() = default;
        ~cashset() = default;
        
        [[nodiscard]] bitCacheLine& operator[](int index) {
            return cache[index];
        }
        void reset () {
            filled = 1;
            for (int i = 0; i < CACHE_WAY; i++) {
                cache[i].setTag(0);
                cache[i].setData(0);
                cache[i].setTime(false);
            }
        }
};

class PLRU {
    private:
        std::vector<cashset> cache = std::vector<cashset>(CACHE_SETS);
    public:
        int cacheHit = 0;
        int cacheAccess = 0;
        
        PLRU() = default;
        void checkPLRU(int index, int tag, int time_);
        int findLeastRecentlyUsed(int index);
        ~PLRU() = default;
        
        [[nodiscard]]float hitRate() const;
        
};
