#pragma once
#include <functional>
#include <raylib.h>

Vector3 normalised(Vector3 a);
Vector3 vec3_add(Vector3 a, Vector3 b);
Vector3 vec3_mult(Vector3 a, float b);
Vector3 vec3_div(Vector3 a, float b);

class v3 {
public:
    int x;
    int y;
    int z;
    v3();
    v3(int X, int Y, int Z);
    bool operator==(v3& other) const;
};

namespace std {
    template<>
    struct hash<v3> {
        std::size_t operator()(const v3& obj) const;
    };
}