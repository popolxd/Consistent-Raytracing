#include "level.h"
#include "player.h"
#include "mymath.h"
#include "collisions.h"
#include "main.h"
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

void LEVEL_InitLevel1(LEVEL_Level* level, PLAYER_Player* player)
{
    level->objectsLen = 2;
    level->objects = (LEVEL_Object*)malloc(sizeof(LEVEL_Object)*level->objectsLen);

    level->objects[0].shape.sphere = (LEVEL_Sphere){(Vector3){0, 0, 5}, 1};
    level->objects[0].type = SPHERE;
    level->objects[0].diffuseColor = (Vector3){0.3, 0.3, 0.8};
    level->objects[0].ambientColor = (Vector3){0.3, 0.3, 0.8};
    level->objects[0].specularColor = (Vector3){0.3, 0.3, 0.8};
    level->objects[0].shininess = 4;
    level->objects[0].mirrorValue = 1; // recommended to be 0 or 1

    level->objects[1].shape.plane = (LEVEL_Plane){(Vector3){0, 1, 0}, (Vector3){0, -1, 0}};
    level->objects[1].type = PLANE;
    level->objects[1].diffuseColor = (Vector3){0.5, 0.5, 0.5};
    level->objects[1].ambientColor = (Vector3){0.5, 0.5, 0.5};
    level->objects[1].specularColor = (Vector3){0.9, 0.9, 0.9};
    level->objects[1].shininess = 4;
    level->objects[1].mirrorValue = 0;

    // directional lightsources
    level->dirLightsLen = 1;
    level->dirLights = (Vector3*)malloc(sizeof(Vector3)*level->dirLightsLen);
    level->dirLightsIntensities = (float*)malloc(sizeof(float)*level->dirLightsLen);

    level->dirLights[0] = MY3DLIB_MATH_GetNormalizedVec((Vector3){1, 1, 1});
    level->dirLightsIntensities[0] = 0.25;

    // point light sources
    level->pointLightsLen = 1;
    level->pointLights = (Vector3*)malloc(sizeof(Vector3)*level->pointLightsLen);
    level->pointLightsIntensities = (float*)malloc(sizeof(float)*level->pointLightsLen);

    level->pointLights[0] = (Vector3){0, -5, 8};
    level->pointLightsIntensities[0] = 10;

    level->ambientLightIntensity = 0.2;

    level->skyColor = (Vector3){0.6, 0.6, 0.6};

    PLAYER_InitPlayer(player, (Vector3){0, 0, 0}, 1, 1);
}

void LEVEL_UpdateLevel1(LEVEL_Level* level, PLAYER_Player* player)
{
    PLAYER_MovePlayer(player);
    LEVEL_Raytracing(level, player);
}

void LEVEL_Raytracing(LEVEL_Level* level, PLAYER_Player* player)
{

    Vector3 rayDir, pixelColor;
    float heightOverWidth = (float)SCREEN_HEIGHT/SCREEN_WIDTH;
    float tanOfFov = tan(player->fov/2);

    for (int i = 0; i < player->numOfRays.y; i++) {
        for (int j = 0; j < player->numOfRays.x; j++) {

            // equal dist between rays and normalized dir
            rayDir = MY3DLIB_MATH_GetNormalizedVec((Vector3){
                -tanOfFov + ((j+0.5)/player->numOfRays.x)*2*tanOfFov,
                -heightOverWidth*tanOfFov + ((i+0.5)/player->numOfRays.y)*2*heightOverWidth*tanOfFov,
                1
            });

            rayDir = MY3DLIB_MATH_GetRotatedVecYAxis(MY3DLIB_MATH_GetRotatedVecXAxis(rayDir, player->rotation.x), player->rotation.y); // getting ray into correct direction
            pixelColor = LEVEL_ReturnPixel(level, player, rayDir, 5);

            DrawRectangle(j*player->raySize, i*player->raySize, player->raySize, player->raySize, LEVEL_ConvertAndClampColor(pixelColor));
        }
    }
}

Vector3 LEVEL_ReturnPixel(LEVEL_Level* level, PLAYER_Player* player, Vector3 rayDir, int numOfBounces)
{
    Vector3 finalColorOfPixel = (Vector3){0, 0, 0};
    Vector3 collisionPoint, lightToCollisionPointVec, currentPointLightDir;
    int isShadowRayCollision;
    float diffuseDotProd, specularDotProd, distanceSquared, currentPointLightIntensity;
    LEVEL_CollisionInfo collisionInfo;
    float contribution = 1;

    Vector3 currentRayOrigin = player->pos;
    Vector3 currentRayDir = rayDir;

    for (int iter = 0; iter < numOfBounces; iter++) {
        LEVEL_GetCollisionInfo(level, currentRayOrigin, currentRayDir, &collisionInfo);

        if (collisionInfo.finalCollisionDist != INFINITY) {
            collisionPoint = MY3DLIB_MATH_GetAddedVec(currentRayOrigin, MY3DLIB_MATH_GetMultipliedVec(currentRayDir, collisionInfo.finalCollisionDist));
            MY3DLIB_MATH_AddToVec(&collisionPoint, MY3DLIB_MATH_GetMultipliedVec(collisionInfo.finalNormal, EPSILON));

            if (collisionInfo.mirrorValue == 1) {
                currentRayOrigin = collisionPoint;
                currentRayDir = MY3DLIB_MATH_GetReflectedVecAlongNormal(currentRayDir, collisionInfo.finalNormal);
                contribution *= collisionInfo.mirrorValue;
                continue;
            }

            // directional light source
            for (int i = 0; i < level->dirLightsLen; i++) { 
                isShadowRayCollision = LEVEL_IsCollision(level, collisionPoint, MY3DLIB_MATH_GetMultipliedVec(level->dirLights[i], -1));

                if (!isShadowRayCollision) {
                    diffuseDotProd = MY3DLIB_MATH_DotProd(MY3DLIB_MATH_GetMultipliedVec(level->dirLights[i], -1), collisionInfo.finalNormal);
                    MY3DLIB_MATH_AddToVec(&finalColorOfPixel, MY3DLIB_MATH_GetMultipliedVec(
                        collisionInfo.finalDiffuseColor, level->dirLightsIntensities[i]*max(diffuseDotProd, 0)*(1 - collisionInfo.mirrorValue)*contribution
                    )); // diffuse light
                
                    specularDotProd = MY3DLIB_MATH_DotProd(MY3DLIB_MATH_GetReflectedVecAlongNormal(level->dirLights[i], collisionInfo.finalNormal), MY3DLIB_MATH_GetMultipliedVec(rayDir, -1)); // specular light
                    MY3DLIB_MATH_AddToVec(&finalColorOfPixel, MY3DLIB_MATH_GetMultipliedVec(
                        collisionInfo.finalSpecularColor, level->dirLightsIntensities[i]*pow(max(specularDotProd, 0), collisionInfo.finalShininess)*(1 - collisionInfo.mirrorValue)*contribution
                    )); // specular light
                }
            }

            for (int i = 0; i < level->pointLightsLen; i++) {
                lightToCollisionPointVec = MY3DLIB_MATH_GetSubtractedVec(collisionPoint, level->pointLights[i]);
                currentPointLightDir = MY3DLIB_MATH_GetNormalizedVec(lightToCollisionPointVec);
                distanceSquared = square(lightToCollisionPointVec.x) + square(lightToCollisionPointVec.y) + square(lightToCollisionPointVec.z);
                currentPointLightIntensity = level->pointLightsIntensities[i] / distanceSquared;

                isShadowRayCollision = LEVEL_IsCollision(level, collisionPoint, MY3DLIB_MATH_GetMultipliedVec(currentPointLightDir, -1));

                if (!isShadowRayCollision) {
                    diffuseDotProd = MY3DLIB_MATH_DotProd(MY3DLIB_MATH_GetMultipliedVec(currentPointLightDir, -1), collisionInfo.finalNormal);
                    MY3DLIB_MATH_AddToVec(&finalColorOfPixel, MY3DLIB_MATH_GetMultipliedVec(
                        collisionInfo.finalDiffuseColor, currentPointLightIntensity*max(diffuseDotProd, 0)*(1 - collisionInfo.mirrorValue)*contribution
                    )); // diffuse light

                    specularDotProd = MY3DLIB_MATH_DotProd(MY3DLIB_MATH_GetReflectedVecAlongNormal(currentPointLightDir, collisionInfo.finalNormal), MY3DLIB_MATH_GetMultipliedVec(rayDir, -1)); // specular light
                    MY3DLIB_MATH_AddToVec(&finalColorOfPixel, MY3DLIB_MATH_GetMultipliedVec(
                        collisionInfo.finalSpecularColor, currentPointLightIntensity*pow(max(specularDotProd, 0), collisionInfo.finalShininess)*(1 - collisionInfo.mirrorValue)*contribution
                    )); // specular light
                }
            }

            MY3DLIB_MATH_AddToVec(&finalColorOfPixel, MY3DLIB_MATH_GetMultipliedVec(collisionInfo.finalAmbientColor, level->ambientLightIntensity*(1 - collisionInfo.mirrorValue)*contribution)); // ambient light

            if (collisionInfo.mirrorValue == 0) {
                return finalColorOfPixel;
            } else {
                currentRayOrigin = collisionPoint;
                currentRayDir = MY3DLIB_MATH_GetReflectedVecAlongNormal(currentRayDir, collisionInfo.finalNormal);
                contribution *= collisionInfo.mirrorValue;
            }

        } else {
            MY3DLIB_MATH_AddToVec(&finalColorOfPixel, MY3DLIB_MATH_GetMultipliedVec(level->skyColor, contribution));
            return finalColorOfPixel;
        }
    }

    return finalColorOfPixel;
}

void LEVEL_GetCollisionInfo(LEVEL_Level* level, Vector3 rayOrigin, Vector3 rayDir, LEVEL_CollisionInfo* collisionInfo)
{

    collisionInfo->finalCollisionDist = INFINITY;
    Vector3 normal;

    float collisionDist;
    for (int i = 0; i < level->objectsLen; i++) {
        switch (level->objects[i].type) {
            case SPHERE:
                collisionDist = COLLISIONS3D_RaySphereIntersectionEx(rayOrigin, rayDir, level->objects[i].shape.sphere.pos, level->objects[i].shape.sphere.radius, &normal);
                break;
            case PLANE:
                collisionDist = COLLISIONS3D_RayPlaneIntersectionEx(rayOrigin, rayDir, level->objects[i].shape.plane.point, level->objects[i].shape.plane.normal, &normal);
                break;
            case DISK:
                collisionDist = COLLISIONS3D_RayDiskIntersectionEx(rayOrigin, rayDir, level->objects[i].shape.disk.pos, level->objects[i].shape.disk.normal, level->objects[i].shape.disk.radius, &normal);
                break;
        }

        if (collisionDist < collisionInfo->finalCollisionDist) {
            collisionInfo->finalCollisionDist = collisionDist;
            collisionInfo->finalNormal = normal;
            collisionInfo->finalDiffuseColor = level->objects[i].diffuseColor;
            collisionInfo->finalAmbientColor = level->objects[i].ambientColor;
            collisionInfo->finalSpecularColor = level->objects[i].specularColor;
            collisionInfo->finalShininess = level->objects[i].shininess;
            collisionInfo->mirrorValue = level->objects[i].mirrorValue;
        }
    }
}

int LEVEL_IsCollision(LEVEL_Level* level, Vector3 rayOrigin, Vector3 rayDir)
{
    float collisionDist;
    for (int i = 0; i < level->objectsLen; i++) {
        switch (level->objects[i].type) {
            case SPHERE:
                collisionDist = COLLISIONS3D_RaySphereIntersection(rayOrigin, rayDir, level->objects[i].shape.sphere.pos, level->objects[i].shape.sphere.radius);
                break;
            case PLANE:
                collisionDist = COLLISIONS3D_RayPlaneIntersection(rayOrigin, rayDir, level->objects[i].shape.plane.point, level->objects[i].shape.plane.normal);
                break;
            case DISK:
                collisionDist = COLLISIONS3D_RayDiskIntersection(rayOrigin, rayDir, level->objects[i].shape.disk.pos, level->objects[i].shape.disk.normal, level->objects[i].shape.disk.radius);
                break;
        }

        if (collisionDist != INFINITY) return 1;
    }
    return 0;
}

Color LEVEL_ConvertAndClampColor(Vector3 color)
{
    return (Color){
        min(max(color.x, 0), 1) * 255,
        min(max(color.y, 0), 1) * 255,
        min(max(color.z, 0), 1) * 255,
        255
    };
}