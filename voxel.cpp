#include "voxel.hpp"
#include "renderer.hpp"
#include "vec.hpp"
#include <cstdio>
#include <cstdlib>
#include <raylib.h>
#include <vector>

#define u8 uint8_t

world::world(int dist, int size) {
    data = new chunk[size*size];
    meshes.resize(size*size);
    worldSize = size;
    renderDistance = dist;
    for (int i = 0; i < worldSize; i++) {
        for (int j = 0; j < worldSize; j++) {
            generateCache(i, j);
        }
    }
}
void world::renderChunk(int Cx, int Cy) {
    int index = Cx*worldSize+Cy;
    for (int i = 0; i < meshes[index].size(); i++) {
        DrawTriangle3D(
            meshes[index][i].data[0],
            meshes[index][i].data[2],
            meshes[index][i].data[3],
            GREEN
        );
        DrawTriangle3D(
            meshes[index][i].data[0],
            meshes[index][i].data[3],
            meshes[index][i].data[1],
            PURPLE
        );
    }
}
// bool world::allNeighboursSolid(int Cx, int Cy, int x, int y, int z) {
//     int chunkX = Cx;
//     int chunkY = Cy;
//     int tX = x - 1;
//     int tY = y;
//     int tZ = z;
    
//     if (tX < 0) {
//         tX += 16;
//         chunkX--;
//     }
//     if (chunkX < 0 || chunkX >= worldSize) {
//         return false;
//     }
//     if (data[chunkX * worldSize + chunkY].data[tX * 256 + tY * 16 + tZ] == 0) {
//         return false;
//     }
    
//     if (y + 1 > 15) {
//         return false;
//     }
//     if (data[Cx * worldSize + Cy].data[x * 256 + (y + 1) * 16 + z] == 0) {
//         return false;
//     }

//     chunkX = Cx;
//     chunkY = Cy;
//     tX = x;
//     tY = y;
//     tZ = z + 1;
    
//     if (tZ > 15) {
//         tZ -= 16;
//         chunkY++;
//     }
//     if (chunkY < 0 || chunkY >= worldSize) {
//         return false;
//     }
//     if (data[chunkX * worldSize + chunkY].data[tX * 256 + tY * 16 + tZ] == 0) {
//         return false;
//     }
    
//     return true;
// }
bool world::topNeighbourSolid(int Cx, int Cy, int x, int y, int z) {
    if (y+1 > 15) {
        return false;
    }
    return data[Cx*worldSize+Cy].data[x*16*16+y*16+z] != 0;
}
bool world::leftNeighbourSolid(int Cx, int Cy, int x, int y, int z) {
    if (x-1 < 0) {
        Cx--;
        x = 15;
    }
    if (Cx < 0) {
        return false;
    }
    return data[Cx*worldSize+Cy].data[x*16*16+y*16+z] != 0;
}
bool world::rightNeighbourSolid(int Cx, int Cy, int x, int y, int z) {
    if (z+1 > 15) {
        Cy++;
        z = 0;
    }
    if (Cy > worldSize) {
        return false;
    }
    return data[Cx*worldSize+Cy].data[x*16*16+y*16+z] != 0;
}
void world::generateCache(int Cx, int Cy) {
    meshes[Cx*worldSize+Cy].clear();

    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            for (int z = 0; z < 16; z++) {
                if (data[Cx*worldSize+Cy].data[x*16*16+y*16+z] != 0) {
                    Vector3 offset = {(float)(Cx-worldSize/2)*16+x,(float)(-16+y),(float)(Cy-worldSize/2)*16+z};
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