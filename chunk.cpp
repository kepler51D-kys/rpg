#include "vec.hpp"
#include "voxel.hpp"
#include "renderer.hpp"
chunk::chunk() {
    int setTo = 1;
    for (int i = 0; i < 16*16*16; i++) {
        data[i] = setTo;
        // setTo++;
        // setTo %= 2;
    }
}