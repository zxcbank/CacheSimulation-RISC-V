//
// Created by Святослав on 12.05.2024.
//

#include "Plru.hpp"

bool PLRU::checkPLRU(int addr, int time_) {
    int index = (addr >> 5) & 31;
    int tag = (addr >> 10) & 255;
    
    for (int i = 0; i < CACHE_WAY; i++) {
        if (cache[index][i].getTag() == tag) {
            
            cache[index][i].setAct(true);
            bool flag = false;
            for (int j = 0; j < CACHE_WAY; j++) {
                if (!cache[index][j].getAct()) {
                    flag = true;
                    break;
                }
            }
            if (!flag) {
                for (int j = 0; j < CACHE_WAY; j++) {
                    if (i != j) {
                        cache[index][j].setAct(false);
                    }
                }
            }
            return true;
        }
    }
    
    int minWay = findLeastRecentlyUsed(index);
    
    cache[index][minWay].setTag(tag);
    cache[index][minWay].setAct(true);
    cache[index][minWay].setData(0); //забыл
    bool flag = false;
    for (int j = 0; j < CACHE_WAY; j++) {
        if (!cache[index][j].getAct()) {
            flag = true;
            break;
        }
    }
    if (!flag) {
        for (int j = 0; j < CACHE_WAY; j++) {
            if (minWay != j) {
                cache[index][j].setAct(false);
            }
        }
    }
    return false;
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
