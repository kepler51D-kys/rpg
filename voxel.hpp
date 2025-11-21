#pragma once
#include "raylib.h"
#include "renderer.hpp"
#include <cstdint>
#include <vector>

class chunk {
public:
    uint16_t data[16*16*16];
    chunk();
};
class world {
public:
    Texture2D texture;
    chunk* data;
    std::vector<std::vector<quad>> meshes;
    int worldSize;
    int renderDistance;
    world(int dist, int size);
    ~world();
    bool allNeighboursSolid(int Cx, int Cy, int x, int y, int z);
    void generateCache(int Cx, int Cy);
    void render(const Vector3& playerPos);
    bool topNeighbourSolid(int Cx, int Cy, int x, int y, int z);
    bool leftNeighbourSolid(int Cx, int Cy, int x, int y, int z);
    bool rightNeighbourSolid(int Cx, int Cy, int x, int y, int z);
    void renderChunk(int Cx, int Cy);
};