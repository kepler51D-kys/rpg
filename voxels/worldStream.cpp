#include "voxel.hpp"
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <rlgl.h>
#include "../vec.hpp"
worldStream::worldStream(int dist, int WorldSize) {
    texture = LoadTexture("assets/textures.png");
    if (texture.id == 0) {
        exit(-1);
    }
    area.reserve(dist*dist*2);
    
    int len = WorldSize*WorldSize*chunkSize*chunkSize*chunkSize;
    uint16_t map[len];
    for (int i = 0; i < len; i++) {
        map[i] = 1;
    }
    std::fstream outputFile("worlds/testworld.world");
    if (!outputFile.is_open()) {
        exit(-1);
    }
    outputFile.write(reinterpret_cast<char*>(map), len);
}
void loadChunk() {

}
void worldStream::renderChunk(int x, int y, int z) {
    int index = x * worldSize + y;

    if (index >= meshes.size() || meshes[index].empty()) {
        return;
    }
    
    rlSetTexture(texture.id);
    rlBegin(RL_QUADS);
    
    for (int i = 0; i < meshes[index].size(); i++) {
        quad newQuad = meshes[index][i];
        atlasTexture texture = getTexture(newQuad.textureId);

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

void worldStream::generateCache(int Cx, int Cy) {
    meshes[Cx*worldSize+Cy].clear();
    Vector3 chunkOffset = {
        (float)(Cx - (float)worldSize/2) * chunkSize,
        -chunkSize,
        (float)(Cy - (float)worldSize/2) * chunkSize
    };
    for (int x = 0; x < chunkSize; x++) {
        for (int y = 0; y < chunkSize; y++) {
            for (int z = 0; z < chunkSize; z++) {
                if (area[v3(Cx,0, Cy)][x*chunkSize*chunkSize+y*chunkSize+z] != 0) {
                    Vector3 offset = vec3_add(chunkOffset,Vector3((float)x,(float)y,(float)z));

                    if (!topNeighbourSolid(Cx,Cy,x,y,z)) {
                        quad newquad = {
                            vec3_add(topQuad.data[0],offset),
                            vec3_add(topQuad.data[1],offset),
                            vec3_add(topQuad.data[2],offset),
                            vec3_add(topQuad.data[3],offset),
                        };
                        newquad.textureId = area[v3(Cx,0, Cy)][x*chunkSize*chunkSize+y*chunkSize+z]-1;
                        meshes[Cx*worldSize+Cy].push_back(newquad);
                    }
                    if (!leftNeighbourSolid(Cx,Cy,x,y,z)) {
                        quad newquad = {
                            vec3_add(leftQuad.data[0],offset),
                            vec3_add(leftQuad.data[1],offset),
                            vec3_add(leftQuad.data[2],offset),
                            vec3_add(leftQuad.data[3],offset),
                        };
                        newquad.textureId = area[v3(Cx,0, Cy)][x*chunkSize*chunkSize+y*chunkSize+z]-1;
                        meshes[Cx*worldSize+Cy].push_back(newquad);
                    }
                    if (!rightNeighbourSolid(Cx,Cy,x,y,z)) {
                        quad newquad = {
                            vec3_add(rightQuad.data[0],offset),
                            vec3_add(rightQuad.data[1],offset),
                            vec3_add(rightQuad.data[2],offset),
                            vec3_add(rightQuad.data[3],offset),
                        };
                        newquad.textureId = area[v3(Cx,0, Cy)][x*chunkSize*chunkSize+y*chunkSize+z]-1;
                        meshes[Cx*worldSize+Cy].push_back(newquad);
                    }
                }
            }
        }
    }
}
int worldStream::chunkIndex(int x, int y, int z) {
    return x*worldSize*worldSize+y*worldSize+z;
}

bool worldStream::topNeighbourSolid(int Cx, int Cy, int x, int y, int z) {
    if (++y >= chunkSize) {
        return false;
    }
    return area[v3(Cx,0, Cy)][chunkIndex(x, y, z)];
}
bool worldStream::leftNeighbourSolid(int Cx, int Cy, int x, int y, int z) {
    if (--x < 0) {
        Cx--;
        x = 15;
    }
    if (Cx < 0) {
        return false;
    }
    return area[v3(Cx,0, Cy)][chunkIndex(x, y, z)];
}
bool worldStream::rightNeighbourSolid(int Cx, int Cy, int x, int y, int z) {
    if (++z >= chunkSize) {
        Cy++;
        z = 0;
    }
    if (Cy >= worldSize) {
        return false;
    }
    return area[v3(Cx,0, Cy)][chunkIndex(x, y, z)];
}