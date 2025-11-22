#include "voxel.hpp"
#include "renderer.hpp"
#include "../vec.hpp"
#include <cstdio>
#include <cstdlib>
#include <raylib.h>
#include <vector>
#include "rlgl.h"

#define u8 uint8_t

world::world(int dist, int size) {
    data = new chunk[size*size];
    meshes.resize(size*size);
    textureIds.resize((size*size));
    worldSize = size;
    renderDistance = dist;

    texture = LoadTexture("assets/textures.png");

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
atlasTexture world::getTexture(int index) {
    return  (atlasTexture){
        {{ (float)(index-1)/idAmounts, 0.0f },
            { (float)index/idAmounts, 0.0f },
            { (float)index/idAmounts, 1.0f },
            { (float)(index-1)/idAmounts, 1.0f }}
    };
}
void world::renderChunk(int Cx, int Cy) {
    int index = Cx * worldSize + Cy;

    if (index >= meshes.size() || meshes[index].empty()) {
        return;
    }
    
    rlSetTexture(texture.id);
    rlBegin(RL_QUADS);
    
    for (int i = 0; i < meshes[index].size(); i++) {
        quad newQuad = meshes[index][i];
        // atlasTexture texture = {{
        //     { 0.0f, 0.0f },
        //     { 1.0f/idAmounts, 0.0f },
        //     { 1.0f/idAmounts, 1.0f },
        //     { 0.0f, 1.0f }
        // }};
        atlasTexture texture = getTexture(textureIds[Cx*worldSize+Cy][i]);

        rlColor4ub(255, 255, 255, 255);

        rlTexCoord2f(texture.data[0].x, texture.data[0].y);
        rlVertex3f(newQuad.data[0].x, newQuad.data[0].y, newQuad.data[0].z);

        rlTexCoord2f(texture.data[1].x, texture.data[1].y);
        rlVertex3f(newQuad.data[1].x, newQuad.data[1].y, newQuad.data[1].z);

        rlTexCoord2f(texture.data[2].x, texture.data[2].y);
        rlVertex3f(newQuad.data[2].x, newQuad.data[2].y, newQuad.data[2].z);

        rlTexCoord2f(texture.data[3].x, texture.data[3].y);
        rlVertex3f(newQuad.data[3].x, newQuad.data[3].y, newQuad.data[3].z);
    }
    rlEnd();
    rlSetTexture(0);
}
void world::generateCache(int Cx, int Cy) {
    meshes[Cx*worldSize+Cy].clear();
    Vector3 chunkOffset = {
        (float)(Cx - (float)worldSize/2) * chunkSize,
        -chunkSize,
        (float)(Cy - (float)worldSize/2) * chunkSize
    };
    for (int x = 0; x < chunkSize; x++) {
        for (int y = 0; y < chunkSize; y++) {
            for (int z = 0; z < chunkSize; z++) {
                if (data[Cx*worldSize+Cy].data[x*chunkSize*chunkSize+y*chunkSize+z] != 0) {
                    Vector3 offset = vec3_add(chunkOffset,Vector3((float)x,(float)y,(float)z));

                    if (!topNeighbourSolid(Cx,Cy,x,y,z)) {
                        quad newquad = {
                            vec3_add(topQuad.data[0],offset),
                            vec3_add(topQuad.data[1],offset),
                            vec3_add(topQuad.data[2],offset),
                            vec3_add(topQuad.data[3],offset),
                        };
                        meshes[Cx*worldSize+Cy].push_back(newquad);
                        textureIds[Cx*worldSize+Cy].push_back(data[Cx*worldSize+Cy].data[x*chunkSize*chunkSize+y*chunkSize+z]-1);
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
                        textureIds[Cx*worldSize+Cy].push_back(data[Cx*worldSize+Cy].data[x*chunkSize*chunkSize+y*chunkSize+z]-1);
                    }
                    if (!rightNeighbourSolid(Cx,Cy,x,y,z)) {
                        quad newquad = {
                            vec3_add(rightQuad.data[0],offset),
                            vec3_add(rightQuad.data[1],offset),
                            vec3_add(rightQuad.data[2],offset),
                            vec3_add(rightQuad.data[3],offset),
                        };
                        meshes[Cx*worldSize+Cy].push_back(newquad);
                        textureIds[Cx*worldSize+Cy].push_back(data[Cx*worldSize+Cy].data[x*chunkSize*chunkSize+y*chunkSize+z]-1);
                    }
                }
            }
        }
    }
}
void world::render(const Vector3& playerPos) {
    int playerChunkX = playerPos.x / chunkSize;
    int playerChunkZ = playerPos.z / chunkSize;
    for (int x = std::max(-worldSize/2,playerChunkX-renderDistance); x < std::min(worldSize/2,playerChunkX+renderDistance); x++) {
        for (int z = std::max(-worldSize/2,playerChunkZ-renderDistance); z < std::min(worldSize/2,playerChunkZ+renderDistance); z++) {
            float chunkWorldX = x * chunkSize;
            float chunkWorldZ = z * chunkSize;

            float distX = std::abs(chunkWorldX - playerPos.x);
            float distZ = std::abs(chunkWorldZ - playerPos.z);
            
            int coordX = x + worldSize/2;
            int coordZ = z + worldSize/2;
            
            if (distX <= renderDistance * chunkSize && distZ <= renderDistance * chunkSize) {
                renderChunk(coordX,coordZ);
            }
        }
    }
}

bool world::topNeighbourSolid(int Cx, int Cy, int x, int y, int z) {
    if (++y >= chunkSize) {
        return false;
    }
    return data[Cx*worldSize+Cy].data[x*chunkSize*chunkSize+y*chunkSize+z] != 0;
}
bool world::leftNeighbourSolid(int Cx, int Cy, int x, int y, int z) {
    if (--x < 0) {
        Cx--;
        x = 15;
    }
    if (Cx < 0) {
        return false;
    }
    return data[Cx*worldSize+Cy].data[x*chunkSize*chunkSize+y*chunkSize+z] != 0;
}
bool world::rightNeighbourSolid(int Cx, int Cy, int x, int y, int z) {
    if (++z >= chunkSize) {
        Cy++;
        z = 0;
    }
    if (Cy >= worldSize) {
        return false;
    }
    return data[Cx*worldSize+Cy].data[x*chunkSize*chunkSize+y*chunkSize+z] != 0;
}