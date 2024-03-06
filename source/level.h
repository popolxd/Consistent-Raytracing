#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"
#include "player.h"

typedef struct {
    float r, g, b, a;
} LEVEL_FloatColor;

typedef struct {
    Vector3 pos;
    float radius;
} LEVEL_Sphere;

typedef struct {
    Vector3 point;
    Vector3 normal;
} LEVEL_Plane;

typedef struct {
    Vector3 pos;
    Vector3 normal;
    float radius;
} LEVEL_Disk;

typedef union {
    LEVEL_Sphere sphere;
    LEVEL_Plane plane;
    LEVEL_Disk disk;
} LEVEL_ObjectShape;

typedef enum {
    SPHERE,
    PLANE,
    DISK
} LEVEL_ObjectType;

typedef struct {
    LEVEL_ObjectShape shape;
    LEVEL_ObjectType type;

    float mirrorValue;
    Vector3 diffuseColor;
    Vector3 ambientColor;
    Vector3 specularColor;

    float shininess;

} LEVEL_Object;

typedef struct {
    float mirrorValue;
    float finalCollisionDist;
    Vector3 finalNormal;
    Vector3 finalDiffuseColor;
    Vector3 finalAmbientColor;
    Vector3 finalSpecularColor;
    float finalShininess;
} LEVEL_CollisionInfo;

typedef struct {
    LEVEL_Object* objects;
    int objectsLen;

    Vector3* dirLights;
    float* dirLightsIntensities;
    int dirLightsLen;

    Vector3* pointLights;
    float* pointLightsIntensities;
    int pointLightsLen;

    float ambientLightIntensity;

    Vector3 skyColor;

} LEVEL_Level;

void LEVEL_InitLevel1(LEVEL_Level* level, PLAYER_Player* player);
void LEVEL_UpdateLevel1(LEVEL_Level* level, PLAYER_Player* player);
void LEVEL_Raytracing(LEVEL_Level* level, PLAYER_Player* player);

Vector3 LEVEL_ReturnPixel(LEVEL_Level* level, PLAYER_Player* player, Vector3 rayDir, int numOfBouces);
void LEVEL_GetCollisionInfo(LEVEL_Level* level, Vector3 rayOrigin, Vector3 rayDir, LEVEL_CollisionInfo* collisionInfo);
int LEVEL_IsCollision(LEVEL_Level* level, Vector3 rayOrigin, Vector3 rayDir);

Color LEVEL_ConvertAndClampColor(Vector3 color);
#endif