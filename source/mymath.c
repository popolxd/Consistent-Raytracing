#include "mymath.h"
#include <math.h>
#include <stdlib.h>
#include "raylib.h"
#include <stdint.h>

Vector3 MY3DLIB_MATH_GetMultipliedVec(Vector3 vec, float value)
{
    return (Vector3){vec.x*value, vec.y*value, vec.z*value};
}

Vector3 MY3DLIB_MATH_GetMultipliedVecByVec(Vector3 vec, Vector3 value)
{
    return (Vector3){vec.x*value.x, vec.y*value.y, vec.z*value.z};
}

Vector3 MY3DLIB_MATH_GetSubtractedVec(Vector3 vec, Vector3 values)
{
    return (Vector3){vec.x - values.x, vec.y - values.y, vec.z - values.z};
}

Vector3 MY3DLIB_MATH_GetAddedVec(Vector3 vec, Vector3 values)
{
    return (Vector3){vec.x + values.x, vec.y + values.y, vec.z + values.z};
}

void MY3DLIB_MATH_AddToVec(Vector3* vec, Vector3 values)
{
    vec->x += values.x;
    vec->y += values.y;
    vec->z += values.z;
}

void MY3DLIB_MATH_SubtractFromVec(Vector3* vec, Vector3 values)
{
    vec->x -= values.x;
    vec->y -= values.y;
    vec->z -= values.z;
}

void MY3DLIB_MATH_MultiplyVecBy(Vector3* vec, float value)
{
    vec->x *= value;
    vec->y *= value;
    vec->z *= value;
}

void MY3DLIB_MATH_MultiplyVecByVec(Vector3* vec, Vector3 value)
{
    vec->x *= value.x;
    vec->y *= value.y;
    vec->z *= value.z;
}

Vector3 MY3DLIB_MATH_GetNormalizedVec(Vector3 vec)
{
    float divisor = sqrt(square(vec.x) + square(vec.y) + square(vec.z));
    return (Vector3){vec.x/divisor, vec.y/divisor, vec.z/divisor};
}

float MY3DLIB_MATH_DotProd(Vector3 vec1, Vector3 vec2)
{
    return vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z;
}

// Quaternion MY3DLIB_MATH_MultiplyQuaternions(Quaternion q1, Quaternion q2)
// {
//     Quaternion q;
//     // q.w = q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z;
//     // q.x = q1.w*q2.x + q1.x*q2.w - q1.y*q2.z + q1.z*q2.y;
//     // q.y = q1.w*q2.y + q1.x*q2.z + q1.y*q2.w - q1.z*q2.x;
//     // q.z = q1.w*q2.z - q1.x*q2.y + q1.y*q2.x + q1.z*q2.w;
//     q.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
//     q.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
//     q.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
//     q.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
//     return q;
// }

// Quaternion MY3DLIB_MATH_GetInverseOfQuaternion(Quaternion quater)
// {
//     Quaternion q;
//     q.w = quater.w;
//     q.x = -quater.x;
//     q.y = -quater.y;
//     q.z = -quater.z;
//     return q;
// }

// Quaternion MY3DLIB_MATH_GetRotationalQuaternion(Vector3 axisOfRotation, float angle) // if angle = 0, it doesnt work
// {
//     Quaternion q;
//     q.w = cos(angle/2);
//     q.x = axisOfRotation.x * sin(angle/2);
//     q.y = axisOfRotation.y * sin(angle/2);
//     q.z = axisOfRotation.z * sin(angle/2);
//     return q;
// }

// void MY3DLIB_MATH_RotatePointUsingQuaternions(Vector3 axisOfRotation, float angle, Vector3* pointPos)
// {
//     Quaternion pointQuaternion;
//     pointQuaternion.w = 0;
//     pointQuaternion.x = pointPos->x;
//     pointQuaternion.y = pointPos->y;
//     pointQuaternion.z = pointPos->z;

//     Quaternion rotationalQuaternion = MY3DLIB_MATH_GetRotationalQuaternion(axisOfRotation, angle);
//     Quaternion inverseRotationalQuaternion = MY3DLIB_MATH_GetInverseOfQuaternion(rotationalQuaternion);

//     Quaternion resultPointQuaternion = MY3DLIB_MATH_MultiplyQuaternions(MY3DLIB_MATH_MultiplyQuaternions(inverseRotationalQuaternion, pointQuaternion), rotationalQuaternion);

//     pointPos->x = resultPointQuaternion.x;
//     pointPos->y = resultPointQuaternion.y;
//     pointPos->z = resultPointQuaternion.z;
// }

Vector3 MY3DLIB_MATH_GetRotatedVecXAxis(Vector3 vec, float angle)
{
    return (Vector3){
        vec.x,
        vec.y * cos(angle) - vec.z * sin(angle),
        vec.y * sin(angle) + vec.z * cos(angle)
    };
}

Vector3 MY3DLIB_MATH_GetRotatedVecYAxis(Vector3 vec, float angle)
{
    return (Vector3){
        vec.x * cos(angle) + vec.z * sin(angle),
        vec.y,
        - vec.x * sin(angle) + vec.z * cos(angle)
    };
}

Vector3 MY3DLIB_MATH_GetRotatedVecZAxis(Vector3 vec, float angle)
{
    return (Vector3){
        vec.x * cos(angle) - vec.y * sin(angle),
        vec.x * sin(angle) + vec.y * cos(angle),
        vec.z
    };
}

Vector3 MY3DLIB_MATH_GetReflectedVecAlongNormal(Vector3 vec, Vector3 normal)
{
    return MY3DLIB_MATH_GetSubtractedVec(vec, MY3DLIB_MATH_GetMultipliedVec(normal, 2*MY3DLIB_MATH_DotProd(vec, normal)));
}

float MY3DLIB_MATH_GetRandomFloat(float from, float to) {
    float range = fabs(from - to); // distance between both numbers
    float randNum = ((float)rand() / (float)RAND_MAX) * range;
    return randNum + from;
}

Vector3 MY3DLIB_MATH_GetRandomVecInUnitSphere()
{
    return MY3DLIB_MATH_GetNormalizedVec(
        (Vector3){MY3DLIB_MATH_GetRandomFloat(-1, 1), MY3DLIB_MATH_GetRandomFloat(-1, 1), MY3DLIB_MATH_GetRandomFloat(-1, 1)}
    );
    // return (Vector3){MY3DLIB_MATH_GetRandomFloat(from, to), MY3DLIB_MATH_GetRandomFloat(from, to), MY3DLIB_MATH_GetRandomFloat(from, to)};
}

Vector3 MY3DLIB_MATH_GetFastRandomVecInUnitSphere(uint32_t* seed)
{
    return MY3DLIB_MATH_GetNormalizedVec(
        (Vector3){MY3DLIB_MATH_GetFastRandomFloat(-1, 1, seed), MY3DLIB_MATH_GetFastRandomFloat(-1, 1, seed), MY3DLIB_MATH_GetFastRandomFloat(-1, 1, seed)}
    );
}

float MY3DLIB_MATH_GetFastRandomFloat(float from, float to, uint32_t* seed)
{
    float range = fabs(from - to);
    *seed = MY3DLIB_MATH_PCGHash(*seed);

    float randNum = ((float)MY3DLIB_MATH_PCGHash(*seed) / (float)UINT32_MAX) * range;
    return randNum + from;
}

uint32_t MY3DLIB_MATH_PCGHash(uint32_t seed)
{
    uint32_t state = seed * 747796405u + 2891336453u;
    uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}