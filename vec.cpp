#include "vec.hpp"
#include <cmath>
#include <functional>


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
v3::v3() {
    x = 0;
    y = 0;
    z = 0;
}
v3::v3(int X, int Y, int Z) {
    x = X;
    y = Y;
    z = Z;
}
bool v3::operator==(v3& other) const {
    return x == other.x && y == other.y && z == other.z;
}
// namespace std {
//     template<>
//     struct hash<v3> {
//         std::size_t operator()(const v3& obj) const {
//             std::size_t h1 = std::hash<int>{}(obj.x);
//             std::size_t h2 = std::hash<int>{}(obj.y);
//             std::size_t h3 = std::hash<int>{}(obj.z);
//             return h1 ^ (h2 << 1) ^ (h3 << 2);
//         }
//     };
// }
std::size_t std::hash<v3>::operator()(const v3& obj) const {
    std::size_t h1 = std::hash<int>{}(obj.x);
    std::size_t h2 = std::hash<int>{}(obj.y);
    std::size_t h3 = std::hash<int>{}(obj.z);
    return h1 ^ (h2 << 1) ^ (h3 << 2);
}