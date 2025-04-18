#ifndef SAMPLE_SCENE_H
#define SAMPLE_SCENE_H
#include "../../math/vector3d.h"
#include "../../cameras/camera.h"
#include "../../cameras/fish_eye.h"
#include "../../cameras/pinhole.h"
#include "../../cameras/spherical.h"
#include "../../logic_util.h"
#include "../../math/aabb.h"
#include "../../math/ray.h"
#include "../../math/sphere.h"
#include "../../math/traceable.h"
#include "../../math/vector3d.h"
#include "../../managers/scene.h"
#include "../../processing.h"
#include "../../samplers/jittered.h"
#include "../../samplers/sampler.h"
#include "../../colours.h"
void rect_scene(Canvas* canvas);
#endif
