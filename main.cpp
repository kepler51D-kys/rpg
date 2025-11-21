#include "raylib.h"
#include "vec.hpp"
#include <cstdio>
#include <stdint.h>
// #include <vector>
// #include "raymath.h"
#include "voxel.hpp"
#include "rlgl.h"

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

int main(void) {

    Vector3 camOffset = {-8.125,15.625,8.125};
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "game");

    // printf("hellllll\n");
    world mainWorld = world(3,24);
    // printf("hellllll\n");


    Camera camera = { 0 };
    camera.position = camOffset;
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 30.0f;
    camera.projection = CAMERA_ORTHOGRAPHIC;

    entity player = entity(0.3);
    
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Vector3 movement = {0,0,0};
        if (IsKeyDown(KEY_W)) {
            movement.z -= 1;
            movement.x += 1;
        }
        if (IsKeyDown(KEY_S)) {
            movement.z += 1;
            movement.x -= 1;
        }
        if (IsKeyDown(KEY_A)) {
            movement.x -= 1;
            movement.z -= 1;
        }
        if (IsKeyDown(KEY_D)) {
            movement.x += 1;
            movement.z += 1;
        }
        movement = normalised(movement);
        movement = vec3_mult(movement, player.speed);
        player.position = vec3_add(player.position, movement);

        camera.target = player.position;
        camera.position = vec3_add(player.position, camOffset);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                DrawCapsule(
                    player.position,
                    (Vector3){player.position.x,player.position.y+player.height,player.position.z},
                    1,
                    10,
                    10,
                    GREEN
                );
                mainWorld.render(player.position);
            EndMode3D();
            DrawFPS(0, 0);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}