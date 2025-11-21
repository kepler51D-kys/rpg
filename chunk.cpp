#include "vec.hpp"
#include "voxel.hpp"
#include "renderer.hpp"
chunk::chunk() {
    for (int i = 0; i < 16*16*16; i++) {
        data[i] = 1;
    }
}