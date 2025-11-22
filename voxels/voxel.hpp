#pragma once
#include "raylib.h"
#include "renderer.hpp"
#include <cstdint>
#include <vector>

const int chunkSize = 16;

class chunk {
public:
    uint16_t data[chunkSize*chunkSize*chunkSize];
    chunk();
};
class world {
public:
    Texture2D texture;
    chunk* data;
    std::vector<std::vector<quad>> meshes;
    std::vector<std::vector<int>> textureIds;
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
    atlasTexture getTexture(int index);
};