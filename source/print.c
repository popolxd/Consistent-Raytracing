#include "print.h"
#include "raylib.h"
#include <stdio.h>

void MY3DLIB_PRINT_PrintVec(Vector3 vec)
{
    printf("%f %f %f\n", vec.x, vec.y, vec.z);
}

void MY3DLIB_PRINT_PrintColor(Color color)
{
    printf("%d %d %d %d\n", color.r, color.g, color.b, color.a);
}