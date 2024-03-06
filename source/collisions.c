#include "collisions.h"
#include "raylib.h"
#include "mymath.h"
#include <math.h>

double MY3DLIB_COLLISIONS_RaySphereIntersection(Vector3 rayOrigin, Vector3 rayDir, Vector3 spherePos, float radius)
{
    double a = square(rayDir.x) + square(rayDir.y) + square(rayDir.z); // never 0
    double b = 2 * (rayDir.x * (rayOrigin.x - spherePos.x) + rayDir.y * (rayOrigin.y - spherePos.y) + rayDir.z * (rayOrigin.z - spherePos.z));
    double c = square(rayOrigin.x - spherePos.x) + square(rayOrigin.y - spherePos.y) + square(rayOrigin.z - spherePos.z) - square(radius);

    double discriminant = square(b) - 4*a*c;
    double t;

    if (discriminant < 0) return INFINITY;
    
    if (discriminant == 0) {
        t = -b/(2*a);
        if (t >= 0) return t;
        return INFINITY;
    }

    t = (-b - sqrt(discriminant)) / (2*a);
    if (t >= 0) return t;

    t = (-b + sqrt(discriminant)) / (2*a);
    if (t >= 0) return t;

    return INFINITY;
}

double MY3DLIB_COLLISIONS_RaySphereIntersectionEx(Vector3 rayOrigin, Vector3 rayDir, Vector3 spherePos, float radius, Vector3* normal)
{

    double a = square(rayDir.x) + square(rayDir.y) + square(rayDir.z); // never 0
    double b = 2 * (rayDir.x * (rayOrigin.x - spherePos.x) + rayDir.y * (rayOrigin.y - spherePos.y) + rayDir.z * (rayOrigin.z - spherePos.z));
    double c = square(rayOrigin.x - spherePos.x) + square(rayOrigin.y - spherePos.y) + square(rayOrigin.z - spherePos.z) - square(radius);

    double discriminant = square(b) - 4*a*c;
    double t;

    if (discriminant < 0) return INFINITY;
    
    if (discriminant == 0) {
        t = -b/(2*a);
        if (t >= 0) {
            *normal = MY3DLIB_MATH_GetMultipliedVec(MY3DLIB_MATH_GetSubtractedVec(MY3DLIB_MATH_GetAddedVec(rayOrigin, MY3DLIB_MATH_GetMultipliedVec(rayDir, t)), spherePos), 1/radius);
            return t;
        }
        return INFINITY;
    }

    t = (-b - sqrt(discriminant)) / (2*a);
    if (t >= 0) {
        *normal = MY3DLIB_MATH_GetMultipliedVec(MY3DLIB_MATH_GetSubtractedVec(MY3DLIB_MATH_GetAddedVec(rayOrigin, MY3DLIB_MATH_GetMultipliedVec(rayDir, t)), spherePos), 1/radius);
        return t;
    }

    t = (-b + sqrt(discriminant)) / (2*a);
    if (t >= 0) {
        *normal = MY3DLIB_MATH_GetMultipliedVec(MY3DLIB_MATH_GetSubtractedVec(MY3DLIB_MATH_GetAddedVec(rayOrigin, MY3DLIB_MATH_GetMultipliedVec(rayDir, t)), spherePos), 1/radius);
        return t;
    }

    return INFINITY;
}

double MY3DLIB_COLLISIONS_RayPlaneIntersection(Vector3 rayOrigin, Vector3 rayDir, Vector3 planePoint, Vector3 planeNormal)
{
    float denom = MY3DLIB_MATH_DotProd(rayDir, planeNormal);
    if (denom == 0) return INFINITY;

    double t = MY3DLIB_MATH_DotProd(MY3DLIB_MATH_GetSubtractedVec(planePoint, rayOrigin), planeNormal) / denom;

    if (t >= 0) return t;
    return INFINITY;
}

double MY3DLIB_COLLISIONS_RayPlaneIntersectionEx(Vector3 rayOrigin, Vector3 rayDir, Vector3 planePoint, Vector3 planeNormal, Vector3* normal)
{
    float denom = MY3DLIB_MATH_DotProd(rayDir, planeNormal);
    if (denom == 0) return INFINITY;

    double t = MY3DLIB_MATH_DotProd(MY3DLIB_MATH_GetSubtractedVec(planePoint, rayOrigin), planeNormal) / denom;

    if (t >= 0) {
        // just need to check dotprod of rayDir with normal
        if (MY3DLIB_MATH_DotProd(rayDir, planeNormal) < 0) *normal = planeNormal;
        else *normal = MY3DLIB_MATH_GetMultipliedVec(planeNormal, -1);
        return t;
    }
    return INFINITY;
}

double MY3DLIB_COLLISIONS_RayDiskIntersection(Vector3 rayOrigin, Vector3 rayDir, Vector3 diskOrigin, Vector3 diskNormal, float radius)
{
    float denom = MY3DLIB_MATH_DotProd(rayDir, diskNormal);
    if (denom == 0) return INFINITY;

    double t = MY3DLIB_MATH_DotProd(MY3DLIB_MATH_GetSubtractedVec(diskOrigin, rayOrigin), diskNormal) / denom;

    Vector3 pointOfIntersection = MY3DLIB_MATH_GetAddedVec(rayOrigin, MY3DLIB_MATH_GetMultipliedVec(rayDir, t));

    if (t >= 0 
    && square(pointOfIntersection.x - diskOrigin.x) + square(pointOfIntersection.y - diskOrigin.y) + square(pointOfIntersection.z - diskOrigin.z) <= square(radius)) { // dont need to take sqrt(), it will produce the same answer wheen comparing distance^2 to radius^2
        return t;
    }
    return INFINITY;
}

double MY3DLIB_COLLISIONS_RayDiskIntersectionEx(Vector3 rayOrigin, Vector3 rayDir, Vector3 diskOrigin, Vector3 diskNormal, float radius, Vector3* normal)
{
    float denom = MY3DLIB_MATH_DotProd(rayDir, diskNormal);
    if (denom == 0) return INFINITY;

    double t = MY3DLIB_MATH_DotProd(MY3DLIB_MATH_GetSubtractedVec(diskOrigin, rayOrigin), diskNormal) / denom;

    Vector3 pointOfIntersection = MY3DLIB_MATH_GetAddedVec(rayOrigin, MY3DLIB_MATH_GetMultipliedVec(rayDir, t));

    if (t >= 0 
    && square(pointOfIntersection.x - diskOrigin.x) + square(pointOfIntersection.y - diskOrigin.y) + square(pointOfIntersection.z - diskOrigin.z) <= square(radius)) { // dont need to take sqrt(), it will produce the same answer wheen comparing distance^2 to radius^2
        // just need to check dotprod of rayDir with normal
        if (MY3DLIB_MATH_DotProd(rayDir, diskNormal) < 0) *normal = diskNormal;
        else *normal = MY3DLIB_MATH_GetMultipliedVec(diskNormal, -1);
        return t;
    }
    return INFINITY;
}