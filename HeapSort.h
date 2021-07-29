#pragma once

#include <vector>

#include "ZobristHash.h"
#include "Defines.h"

using std::swap;

namespace Chess{
    void Heapify(std::vector<hash>& hashVector, int size,int i){
        int largest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;

        if(l < size && hashVector[l] > hashVector[largest])
            largest = l;
        if(r < size && hashVector[r] > hashVector[largest])
            largest = r;
        if(largest != i){
            swap(hashVector[largest], hashVector[i]);
            Heapify(hashVector, size, largest);
        }
    }
    void BuildHeap(std::vector<hash>& hashVector, int size){
        for(int i = size / 2 - 1; i >=0; i--)
            Heapify(hashVector, size, i);

    }
    void HeapSort(std::vector<hash>& hashVector){
        int size = hashVector.size();
        BuildHeap(hashVector, size);
        for(int i = size - 1; i >= 0; i--){
            swap(hashVector[0], hashVector[i]);
            Heapify(hashVector, i, 0);
        }
    }
}
