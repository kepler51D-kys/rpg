#include "voxel.hpp"

chunk::chunk() {
    int setTo = 0;
    for (int i = 0; i < 16*16*16; i++) {
        data[i] = setTo+1;
        setTo++;
        setTo %= 3;
    }
}
uint16_t& chunk::operator[](int index) {
    return data[index];
}