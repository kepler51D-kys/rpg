#include "renderer.hpp"

const int idAmounts = 3;

quad topQuad = {
    Vector3(0, 1, 1),
    Vector3(1, 1, 1),
    Vector3(1, 1, 0),
    Vector3(0, 1, 0),
};
quad leftQuad = {
    Vector3(0, 0, 0),
    Vector3(0, 0, 1),
    Vector3(0, 1, 1),
    Vector3(0, 1, 0),
};
quad rightQuad = {
    Vector3(0, 0, 1),
    Vector3(1, 0, 1),
    Vector3(1, 1, 1),
    Vector3(0, 1, 1),
};