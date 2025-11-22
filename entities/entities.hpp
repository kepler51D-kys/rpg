#pragma once
#include "raylib.h"

class entity {
public:
    Vector3 position;
    float height;
    float speed;
    entity(float Speed) {
        position.x = 0;
        position.y = 0;
        position.z = 0;
        height = 4;
        speed = Speed;
    }
};
