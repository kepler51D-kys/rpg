#pragma once
#include "raylib.h"

#define Vector3(x,y,z) (Vector3){x,y,z}

extern const int idAmounts;

typedef struct {
    Vector3 data[4];
} quad;

typedef struct {
    Vector2 data[4];
} atlasTexture;

extern quad topQuad;
extern quad leftQuad;
extern quad rightQuad;