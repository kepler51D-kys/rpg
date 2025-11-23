#include "raylib.h"
#include "vec.hpp"
#include <stdint.h>
#include "voxels/voxel.hpp"
#include "rlgl.h"
#include "entities/entities.hpp"

int main(void) {

    Vector3 camOffset = {-8.125,15.625,8.125};
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "game");

    // printf("hellllll\n");
    // world mainWorld = world(4,16);
    worldStream mainWorld = worldStream(4,16);
    // printf("hellllll\n");


    Camera camera = { 0 };
    camera.position = camOffset;
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 40.0f;
    camera.projection = CAMERA_ORTHOGRAPHIC;

    entity player = entity(0.3);
    
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Vector3 movement = {0,0,0};
        if (IsKeyDown(KEY_E)) {
            camera.fovy -= 5;
        }
        if (IsKeyDown(KEY_Q)) {
            camera.fovy += 5;
        }
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
        if (IsKeyDown(KEY_SPACE)) {
            movement = vec3_mult(movement,2);
        }
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
                    6,
                    6,
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