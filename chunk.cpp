#include "voxel.hpp"

chunk::chunk() {
    int setTo = 1;
    for (int i = 0; i < 16*16*16; i++) {
        data[i] = setTo+1;
        setTo++;
        setTo %= 2;
    }
}