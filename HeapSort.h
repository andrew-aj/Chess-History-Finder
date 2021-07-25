//
// Created by hicke on 7/25/2021.
//

#ifndef CHESSHISTORYFINDER_HEAPSORT_H
#define CHESSHISTORYFINDER_HEAPSORT_H

#include <vector>

#include "ZobristHash.h"
#include "Defines.h"

using std::swap;

namespace Chess{
    void Heapify(vector<hash>& hashVector, int i){
        int largest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;

        if(l < hashVector.size() && hashVector[l] > hashVector[largest])
            largest = l;
        if(r < hashVector.size() && hashVector[r] > hashVector[largest])
            largest = r;
        if(largest != i){
            swap(hashVector[largest], hashVector[i]);
            Heapify(hashVector, largest);
        }

    }

    void HeapSort(vector<hash>& hashVector){
        for(int i = hashVector.size(); i >= 0; i--)
            Heapify(hashVector, i);
    }
}



#endif //CHESSHISTORYFINDER_HEAPSORT_H
