#include "raylib.h"
#include "raymath.h"

bool CheckCollisionRayPlane(Ray ray, Vector3 planePoint, Vector3 planeNormal, float* distance) {
    float denom = Vector3DotProduct(planeNormal, ray.direction);
    if (fabs(denom) > 0.0001f) {
        Vector3 difference = Vector3Subtract(planePoint, ray.position);
        *distance = Vector3DotProduct(difference, planeNormal) / denom;
        if (*distance >= 0) return true;
    }
    return false;
}

int main(void) {
    const int screenWidth = 1600;
    const int screenHeight = 1200;

    InitWindow(screenWidth, screenHeight, "raylib - 3D Player Cube Movement with Mouse Click");

    Camera3D camera;
    camera.position = Vector3{ 13.0f, 20.0f, 13.0f };
    camera.target = Vector3{ 0.0f, 1.0f, 0.0f };
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    camera.fovy = 55.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 playerPosition = { 0.0f, 1.0f, 0.0f };
    Vector3 targetPosition = playerPosition;
    Vector3 playerSize = { 2.0f, 2.0f, 2.0f };
    float speed = 0.05f;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Ray mouseRay = GetMouseRay(GetMousePosition(), camera);
            float distance = 0.0f;
            Vector3 groundPoint = { 0.0f, 0.0f, 0.0f };
            Vector3 groundNormal = { 0.0f, 1.0f, 0.0f };

            if (CheckCollisionRayPlane(mouseRay, groundPoint, groundNormal, &distance)) {
                Vector3 hitPosition = Vector3Add(mouseRay.position, Vector3Scale(mouseRay.direction, distance));
                targetPosition.x = hitPosition.x;
                targetPosition.z = hitPosition.z;
            }
        }

        playerPosition = Vector3Lerp(playerPosition, targetPosition, speed);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);
        Vector3 playerDirection = Vector3Subtract(targetPosition, playerPosition);
        float rotationAngle = atan2f(playerDirection.x, playerDirection.z);
        Vector3 playerRotation = { 0.0f, rotationAngle * RAD2DEG, 0.0f };

        DrawCubeWiresV(playerPosition, playerSize, BLUE);
        DrawGrid(30, 1.0f);
        EndMode3D();
        DrawText("Click on the ground to move the player cube", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
