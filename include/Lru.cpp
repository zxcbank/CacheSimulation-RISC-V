//
// Created by Святослав on 11.05.2024.
//

#include "Lru.hpp"
#define CACHE_LINE_SIZE 32
int LRU::findLeastRecentlyUsed(int index) {
    int minIndex = 0;
    
    for (int i = 0; i < CACHE_WAY; i++) {
        if (cache[index][i].getTime() < cache[index][minIndex].getTime()) {
            minIndex = i;
        }
    }
    
    return minIndex;
}

LRU::LRU() {
    for (int i = 0; i < CACHE_SETS; i++) {
        cache[i] = std::vector<CacheLine>(CACHE_WAY);
    }
}

bool LRU::checkLRU(int addr, int time_) {
    int index = (addr >> 5) & 31;
    int tag = (addr >> 10) & 255;
    
    for (int i = 0; i < CACHE_WAY; i++) {
        if (cache[index][i].getTag() == tag) {
            cache[index][i].setTime(time_);
            return true;
        }
    }
    
    int minWay = findLeastRecentlyUsed(index);
    cache[index][minWay].setTag(tag);
    cache[index][minWay].setTime(time_);
    return false;
}

float LRU::hitRate() const {
    if (cacheAccess == 0) {
        return 0;
    }
    return 100 * static_cast<float>(cacheHit) / cacheAccess;
}

