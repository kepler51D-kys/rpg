#include "voxel.hpp"
#include "renderer.hpp"
#include "vec.hpp"
#include <cstdio>
#include <cstdlib>
#include <raylib.h>
#include <vector>
#include "rlgl.h"

#define u8 uint8_t

world::world(int dist, int size) {
    data = new chunk[size*size];
    meshes.resize(size*size);
    worldSize = size;
    renderDistance = dist;
    
    // printf("not gay\n");

    texture = LoadTexture("gay.png");
    // printf("gay\n");

    for (int i = 0; i < worldSize; i++) {
        for (int j = 0; j < worldSize; j++) {
            generateCache(i, j);
        }
    }
    if (texture.id == 0) {
        printf("you darn fudged it bruv\n");
    }
}
world::~world() {
    UnloadTexture(texture);
}
void world::renderChunk(int Cx, int Cy) {
    // Texture coordinates
    Vector2 texCoords[4] = {
        { 0.0f, 0.0f },  // Top-left
        { 0.25f, 0.0f },  // Top-right
        { 0.25f, 1.0f },  // Bottom-right
        { 0.0f, 1.0f }   // Bottom-left
    };
    
    int index = Cx * worldSize + Cy;
    
    // Early return if no meshes
    if (index >= meshes.size() || meshes[index].empty()) {
        return;
    }
    
    // Only set texture once for all quads in this chunk
    rlSetTexture(texture.id);
    rlBegin(RL_QUADS);
    
    for (int i = 0; i < meshes[index].size(); i++) {
        quad newQuad = meshes[index][i];
        
        // Make sure your quad vertices are in counter-clockwise order for proper facing
        rlColor4ub(255, 255, 255, 255);
        
        // Vertex 0: Bottom-left (adjust based on your coordinate system)
        rlTexCoord2f(texCoords[0].x, texCoords[0].y);
        rlVertex3f(newQuad.data[0].x, newQuad.data[0].y, newQuad.data[0].z);
        
        // Vertex 1: Bottom-right
        rlTexCoord2f(texCoords[1].x, texCoords[1].y);
        rlVertex3f(newQuad.data[1].x, newQuad.data[1].y, newQuad.data[1].z);
        
        // Vertex 2: Top-right
        rlTexCoord2f(texCoords[2].x, texCoords[2].y);
        rlVertex3f(newQuad.data[2].x, newQuad.data[2].y, newQuad.data[2].z);
        
        // Vertex 3: Top-left
        rlTexCoord2f(texCoords[3].x, texCoords[3].y);
        rlVertex3f(newQuad.data[3].x, newQuad.data[3].y, newQuad.data[3].z);
    }
    
    rlEnd();
    rlSetTexture(0);
}
bool world::topNeighbourSolid(int Cx, int Cy, int x, int y, int z) {
    if (++y >= 16) {
        return false;
    }
    return data[Cx*worldSize+Cy].data[x*16*16+y*16+z] != 0;
}
bool world::leftNeighbourSolid(int Cx, int Cy, int x, int y, int z) {
    if (--x < 0) {
        Cx--;
        x = 15;
    }
    if (Cx < 0) {
        return false;
    }
    return data[Cx*worldSize+Cy].data[x*16*16+y*16+z] != 0;
}
bool world::rightNeighbourSolid(int Cx, int Cy, int x, int y, int z) {
    if (++z >= 16) {
        Cy++;
        z = 0;
    }
    if (Cy >= worldSize) {
        return false;
    }
    return data[Cx*worldSize+Cy].data[x*16*16+y*16+z] != 0;
}
void world::generateCache(int Cx, int Cy) {
    meshes[Cx*worldSize+Cy].clear();
    Vector3 chunkOffset = {
        (float)(Cx - worldSize/2) * 16,
        -16.0f,
        (float)(Cy - worldSize/2) * 16
    };
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            for (int z = 0; z < 16; z++) {
                if (data[Cx*worldSize+Cy].data[x*16*16+y*16+z] != 0) {
                    Vector3 offset = vec3_add(chunkOffset,Vector3((float)x,(float)y,(float)z));

                    if (!topNeighbourSolid(Cx,Cy,x,y,z)) {
                        quad newquad = {
                            vec3_add(topQuad.data[0],offset),
                            vec3_add(topQuad.data[1],offset),
                            vec3_add(topQuad.data[2],offset),
                            vec3_add(topQuad.data[3],offset),
                        };
                        meshes[Cx*worldSize+Cy].push_back(newquad);
                    }
                    if (!leftNeighbourSolid(Cx,Cy,x,y,z)) {
                        // printf("gay\n");
                        quad newquad = {
                            vec3_add(leftQuad.data[0],offset),
                            vec3_add(leftQuad.data[1],offset),
                            vec3_add(leftQuad.data[2],offset),
                            vec3_add(leftQuad.data[3],offset),
                        };
                        meshes[Cx*worldSize+Cy].push_back(newquad);
                    }
                    if (!rightNeighbourSolid(Cx,Cy,x,y,z)) {
                        quad newquad = {
                            vec3_add(rightQuad.data[0],offset),
                            vec3_add(rightQuad.data[1],offset),
                            vec3_add(rightQuad.data[2],offset),
                            vec3_add(rightQuad.data[3],offset),
                        };
                        meshes[Cx*worldSize+Cy].push_back(newquad);
                    }
                }
            }
        }
    }
}
void world::render(const Vector3& playerPos) {
    int playerChunkX = playerPos.x / 16;
    int playerChunkZ = playerPos.z / 16;
    // printf("-------------\n");
    int count = 0;
    for (int x = std::max(-worldSize/2,playerChunkX-renderDistance); x < std::min(worldSize/2,playerChunkX+renderDistance); x++) {
        for (int z = std::max(-worldSize/2,playerChunkZ-renderDistance); z < std::min(worldSize/2,playerChunkZ+renderDistance); z++) {
            count++;
            float chunkWorldX = x * 16;
            float chunkWorldZ = z * 16;

            float distX = std::abs(chunkWorldX - playerPos.x);
            float distZ = std::abs(chunkWorldZ - playerPos.z);
            
            int coordX = x + worldSize/2;
            int coordZ = z + worldSize/2;
            
            if (distX <= renderDistance * 16 && distZ <= renderDistance * 16) {
                renderChunk(coordX,coordZ);
            }
            // printf("tt %d\n",data[coordX*worldSize+coordZ].visibleBlocks.size());
        }
    }
    // printf("%d\n",count);
}