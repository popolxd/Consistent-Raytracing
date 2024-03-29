#ifndef MYMATH_H
#define MYMATH_H

#include "raylib.h"
#include <stdint.h>

#define square(x) ((x)*(x))

Vector3 MY3DLIB_MATH_GetMultipliedVec(Vector3 vec, float value);
Vector3 MY3DLIB_MATH_GetMultipliedVecByVec(Vector3 vec, Vector3 value);
Vector3 MY3DLIB_MATH_GetSubtractedVec(Vector3 vec, Vector3 values);
Vector3 MY3DLIB_MATH_GetAddedVec(Vector3 vec, Vector3 values);

void MY3DLIB_MATH_AddToVec(Vector3* vec, Vector3 values);
void MY3DLIB_MATH_SubtractFromVec(Vector3* vec, Vector3 values);
void MY3DLIB_MATH_MultiplyVecBy(Vector3* vec, float value);
void MY3DLIB_MATH_MultiplyVecByVec(Vector3* vec, Vector3 value);

Vector3 MY3DLIB_MATH_GetNormalizedVec(Vector3 vec);
float MY3DLIB_MATH_DotProd(Vector3 vec1, Vector3 vec2);
Vector3 MY3DLIB_MATH_CrossProd(Vector3 vec1, Vector3 vec2);

Vector3 MY3DLIB_MATH_GetRotatedVecXAxis(Vector3 vec, float angle);
Vector3 MY3DLIB_MATH_GetRotatedVecYAxis(Vector3 vec, float angle);
Vector3 MY3DLIB_MATH_GetRotatedVecZAxis(Vector3 vec, float angle);

Vector3 MY3DLIB_MATH_GetReflectedVecAlongNormal(Vector3 vec, Vector3 normal);
Vector3 MY3DLIB_MATH_GetReflectedVecInRandomDir(Vector3 vec, Vector3 normal);
Vector3 MY3DLIB_MATH_GetVecInBetween(Vector3 vec1, Vector3 vec2, float distFromPoint1);

float MY3DLIB_MATH_GetRandomFloat(float from, float to);
float MY3DLIB_MATH_GetFastRandomFloat(float from, float to, uint32_t* seed);
uint32_t MY3DLIB_MATH_PCGHash(uint32_t seed);
#endif