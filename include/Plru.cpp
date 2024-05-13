//
// Created by Святослав on 12.05.2024.
//

#include "Plru.hpp"

void PLRU::checkPLRU(int index, int tag, int time_) {
    cacheAccess++;
    for (int i = 0; i < CACHE_WAY; i++) {
        if (cache[index][i].getTag() == tag) {
            cacheHit++;
            cache[index][i].setAct(true);
            return;
        }
    }
    int minWay = findLeastRecentlyUsed(index);
    
    cache[index].filled++;
    if (cache[index].filled == CACHE_WAY) {
        cache[index].reset();
    }
    cache[index][minWay].setTag(tag);
    cache[index][minWay].setAct(true);
    cache[index][minWay].setData(0); //забыл
}

int PLRU::findLeastRecentlyUsed(int index) {
    int minIndex = 0;
    for (int i = 0; i < CACHE_WAY; i++) {
        if (!cache[index][i].getAct()) {
            minIndex = i;
        }
    }
    return minIndex;
}

float PLRU::hitRate() const {
    if (cacheAccess == 0) {
        return 0;
    }
    return 100 * static_cast<float>(cacheHit) / cacheAccess;
}
