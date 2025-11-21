#pragma once
#include "raylib.h"

#define Vector3(x,y,z) (Vector3){x,y,z}

// typedef Vector3 quad[4];
typedef struct {
    Vector3 data[4];
} quad;
extern quad topQuad;
extern quad leftQuad;
extern quad rightQuad;