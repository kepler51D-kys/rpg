#pragma once
#include "raylib.h"
#include "renderer.hpp"
#include <cstdint>
#include <unordered_map>
#include <vector>
#include "../vec.hpp"

const int chunkSize = 16;

class chunk {
public:
    uint16_t data[chunkSize*chunkSize*chunkSize];
    std::vector<quad> meshCache;
    chunk();
    uint16_t& operator[](int index);
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
    atlasTexture getTexture(int index);
};
class worldStream {
    int chunkIndex(int x, int y, int z);
    bool topNeighbourSolid(int Cx, int Cy, int x, int y, int z);
    bool leftNeighbourSolid(int Cx, int Cy, int x, int y, int z);
    bool rightNeighbourSolid(int Cx, int Cy, int x, int y, int z);
    void generateCache(int Cx, int Cy);
    atlasTexture getTexture(int index);
    
    Texture2D texture;
    std::unordered_map<v3, chunk> area;
    std::vector<std::vector<quad>> meshes;
public:
    int worldSize;
    int renderDistance;
    worldStream(int dist, int WorldSize);
    ~worldStream();
    void loadChunk(int x, int y, int z);
    void unloadChunk(int x, int y, int z);
    void render(const Vector3& playerPos);
    void renderChunk(int x, int y, int z);
};