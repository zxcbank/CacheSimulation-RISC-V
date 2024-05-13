//
// Created by Святослав on 11.05.2024.
//

#include "Lru.hpp"

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

void LRU::checkLRU(int index, int tag, int time_) {
    cacheAccess++;
    for (int i = 0; i < CACHE_WAY; i++) {
        if (cache[index][i].getTag() == tag) {
            cacheHit++;
            cache[index][i].setTime(time_);
            return;
        }
    }
    int minWay = findLeastRecentlyUsed(index);
    cache[index][minWay].setTag(tag);
    cache[index][minWay].setTime(time_);
    cache[index][minWay].setData(0); //забыл
}

float LRU::hitRate() const {
    if (cacheAccess == 0) {
        return 0;
    }
    return 100 * static_cast<float>(cacheHit) / cacheAccess;
}

