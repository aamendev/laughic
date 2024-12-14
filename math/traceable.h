#ifndef TRACEABLE_H
#define TRACEABLE_H
#include "ray.h"
#include "../types.h"
typedef struct Traceable
{
    void* data;
    u32 col;
    int (*intersects)(Ray* r, void* data);
} Traceable;

#endif
