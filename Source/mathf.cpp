#include "mathf.h"
#include <stdlib.h>
#include "time.h"
float Mathf::Lerp(float a, float b, float t)
{
    return a*(1.0f-t)+(b*t);
}

float Mathf::RandameRange(float min, float max)
{
    
    return rand()%(int)max-min;
}

float Mathf::RandameRange2(float max, float min)
{

    return rand() % (int)max + min;
}