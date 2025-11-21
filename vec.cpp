#include "vec.hpp"
#include <cmath>


Vector3 normalised(Vector3 a) {
    int length = std::sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
    if (length == 0) {
        return (Vector3){0,0,0};
    }
    return (Vector3){
        a.x/length,
        a.y/length,
        a.z/length,
    };
}
Vector3 vec3_add(Vector3 a, Vector3 b) {
    return (Vector3){
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
    };
}
Vector3 vec3_mult(Vector3 a, float b) {
    return (Vector3){
        a.x * b,
        a.y * b,
        a.z * b,
    };
}
Vector3 vec3_div(Vector3 a, float b) {
    return (Vector3){
        a.x / b,
        a.y / b,
        a.z / b,
    };
}