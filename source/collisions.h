#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "raylib.h"

double MY3DLIB_COLLISIONS_RaySphereIntersection(Vector3 rayOrigin, Vector3 rayDir, Vector3 spherePos, float radius);
double MY3DLIB_COLLISIONS_RaySphereIntersectionEx(Vector3 rayOrigin, Vector3 rayDir, Vector3 spherePos, float radius, Vector3* normal);

double MY3DLIB_COLLISIONS_RayPlaneIntersection(Vector3 rayOrigin, Vector3 rayDir, Vector3 planePoint, Vector3 planeNormal);
double MY3DLIB_COLLISIONS_RayPlaneIntersectionEx(Vector3 rayOrigin, Vector3 rayDir, Vector3 planePoint, Vector3 planeNormal, Vector3* normal);

double MY3DLIB_COLLISIONS_RayDiskIntersection(Vector3 rayOrigin, Vector3 rayDir, Vector3 diskOrigin, Vector3 diskNormal, float radius);
double MY3DLIB_COLLISIONS_RayDiskIntersectionEx(Vector3 rayOrigin, Vector3 rayDir, Vector3 diskOrigin, Vector3 diskNormal, float radius, Vector3* normal);

#endif