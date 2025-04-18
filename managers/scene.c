#include "./scene.h"
#include <math.h>
#include <stdio.h>

void simple_tracer(SceneData* scene)
{
    scene->sampler->generate_samples(scene->sampler->data);
    float x, y;
    float* means = malloc((scene->traceable_count + 1) * sizeof(float));
    u32* cols = malloc((scene->traceable_count + 1) * sizeof(u32));

    for (int i = 0; i < scene->traceable_count; i++)
    {
        cols[i] = scene->traceables[i]->col;
        scene->traceables[i]->id = i;
    }
    cols[scene->traceable_count] = scene->default_colour;

    int samples = scene->sampler->data->sample_count;

    for (int j = 0; j < scene->canvas->height; j++)
    {
        for (int i = 0; i < scene->canvas->width; i++)
        {
            for (int k = 0; k < scene->traceable_count; k++) means[k] = 0;

            for (int k = 0; k < samples * samples; k++)
            {
                Point2D next_sample = scene->sampler->get_next(scene->sampler->data);
                x = scene->pixelSize * 
                    (i - 0.5 * (scene->canvas->width - 1) + next_sample.x);
                y = scene->pixelSize * 
                    (j - 0.5 * (scene->canvas->height - 1) + next_sample.y);
                scene->ray->base.x = x;
                scene->ray->base.y = y;
                int currMin = 0;
                float currMinVal = INFINITY;
                for (int i = 0; i < scene->traceable_count; i++)
                {
                    float t = scene->traceables[i]->intersects
                            (scene->ray, scene->traceables[i]->data);
                    if (t >= 0)
                    {
                        printf("Value: %f\n ", t);
                        currMin = i * (t < currMinVal) + currMin * !(t < currMinVal);
                        currMinVal = t * (t < currMinVal) + currMinVal * !(t < currMinVal);
                    }
                }
                 means[currMin]++; 
            }
            float sum = 0;
            for (int i = 0; i < scene->traceable_count; i++)
            {
                means[i] /= (samples * samples);
                sum += means[i];
            }
            means[scene->traceable_count] = 1 - sum;
            u32 col = weighted_sum(&cols[0], &means[0], scene->traceable_count + 1);
            fill_rect(scene->canvas, i, j, scene->pixelSize, scene->pixelSize, col);
        }
    }
    free(cols);
    free(means);
}

void perspective_tracer(SceneData *scene)
{
    switch(scene->optimized)
    {
        case 0 :
            {
                unoptimized_perspective_tracer(scene);
            }break;
        case 1:
            {
                optimized_perspective_tracer(scene);
            }break;
    }
}

void optimized_perspective_tracer(SceneData* scene)
{
    struct BVH* bvh = malloc(sizeof(struct BVH));
    if (bvh == NULL)
    {
        printf("BVH Error\n");
        exit(1);
    }
    scene->sampler->generate_samples(scene->sampler->data);
    float* means = malloc((scene->traceable_count + 1) * sizeof(float));
    u32* cols = malloc((scene->traceable_count + 1) * sizeof(u32));

    for (int i = 0; i < scene->traceable_count; i++)
    {
        cols[i] = scene->traceables[i]->col;
        scene->traceables[i]->id = i;
    }
    construct_bvh(scene, bvh);
    cols[scene->traceable_count] = scene->default_colour;
    Point2D pp;

    scene->ray->base = scene->cam->data->eye;
    int samples = scene->sampler->data->sample_count;

    struct BVH** stack = 
        malloc(scene->traceable_count * sizeof(struct BVH*));

    for (int j = 0; j < scene->canvas->height; j++)
    {
        for (int i = 0; i < scene->canvas->width; i++)
        {
            for (int k = 0; k < scene->traceable_count; k++) means[k] = 0;

            for (int k = 0; k < samples * samples; k++)
            {
                Point2D next_sample = scene->sampler->get_next(scene->sampler->data);
                pp.x = scene->pixelSize * 
                    (i - 0.5 * (scene->canvas->width - 1) + next_sample.x);
                pp.y = scene->pixelSize * 
                    (j - 0.5 * (scene->canvas->height - 1) + next_sample.y);
                scene->ray->direction =
                    scene->cam->ray_direction(scene->cam->data, &pp);

                // DFS
                struct BVH* current = bvh;
                int count = -1;
                stack[0] = current;
                count++;
                int currMin = -1;
                float currMinVal = INFINITY;
                float inter_param = -1.0f;
                while (count > -1)
                {
                    current = stack[count--];
                    inter_param = current->root->intersects
                            (scene->ray, current->root->data);
                    int cond = (inter_param >= 0 && inter_param < currMinVal);
                    if (cond)
                    {
                        if (current->left == NULL && current->right == NULL)
                        {
                            if (inter_param < currMinVal)
                            {
                                currMin = current->root->id;
                                currMinVal = inter_param;
                            }
                           // break;
                        }
                        else 
                        { 
                            stack[++count] = current->right;
                            stack[++count] = current->left;
                        }
                    }
                    else {
                    }
                }
                if (currMin > -1)
                {
                means[currMin]++; 
                }
            }
            float sum = 0;
            for (int k = 0; k < scene->traceable_count; k++)
            {
                means[k] /= (samples * samples);
                sum += means[k];
            }
            means[scene->traceable_count] = 1 - sum;
            u32 col = weighted_sum(&cols[0], &means[0], scene->traceable_count + 1);
            fill_rect(scene->canvas, i, j, scene->pixelSize, scene->pixelSize, col);
        }
    }
    free(stack);
    free(cols);
    free(means);
    free(bvh);
}

void unoptimized_perspective_tracer(SceneData* scene)
{
    scene->sampler->generate_samples(scene->sampler->data);
    float* means = malloc((scene->traceable_count + 1) * sizeof(float));
    u32* cols = malloc((scene->traceable_count + 1) * sizeof(u32));

    for (int i = 0; i < scene->traceable_count; i++)
    {
        cols[i] = scene->traceables[i]->col;
    }
    cols[scene->traceable_count] = scene->default_colour;
    Point2D pp;

    scene->ray->base = scene->cam->data->eye;
    int samples = scene->sampler->data->sample_count;

    for (int j = 0; j < scene->canvas->height; j++)
    {
        for (int i = 0; i < scene->canvas->width; i++)
        {
            for (int k = 0; k < scene->traceable_count; k++) means[k] = 0;

            for (int k = 0; k < samples * samples; k++)
            {
                Point2D next_sample = scene->sampler->get_next(scene->sampler->data);
                pp.x = scene->pixelSize * 
                    (i - 0.5 * (scene->canvas->width - 1) + next_sample.x);
                pp.y = scene->pixelSize * 
                    (j - 0.5 * (scene->canvas->height - 1) + next_sample.y);
                scene->ray->direction =
                    scene->cam->ray_direction(scene->cam->data, &pp);
                int currMin = -1;
                float currMinVal = INFINITY;
                float inter_param = -1.0f;
                for (int tr = 0; tr < scene->traceable_count; tr++)
                {
                    inter_param = scene->traceables[tr]->intersects
                            (scene->ray, scene->traceables[tr]->data);
                    int cond = (inter_param >= 0 && inter_param < currMinVal);
                    if (cond)
                    {
                        currMin = tr; 
                        currMinVal = inter_param;
                    }
                }
                if (currMin > -1)
                {
                    means[currMin]++;
                }
            }
            float sum = 0;
            for (int tr = 0; tr < scene->traceable_count; tr++)
            {
                means[tr] /= (samples * samples);
                sum += means[tr];
            }
            means[scene->traceable_count] = 1 - sum;
            u32 col = weighted_sum(&cols[0], &means[0], scene->traceable_count + 1);
            fill_rect(scene->canvas, i, j, scene->pixelSize, scene->pixelSize, col);
        }
    }
    free(cols);
    free(means);
}

void construct_bvh(SceneData* sc, struct BVH* bv)
{
    struct BVH* bvhs = malloc(sc->traceable_count * sizeof(struct BVH));
    for (int i = 0; i < sc->traceable_count; i++)
    {
        sc->traceables[i]->get_bvh(sc->traceables[i], &bvhs[i]);
    }
    int power = 1;
    int n = sc->traceable_count;
    int i;
    int final = 0;
    while (n > power)
    {
        for (i = 0; i < n; i += power<<1)
        {
            if (i < n-power)
            {
                combine_bvh(&bvhs[i], &bvhs[i + power]);
                final = i + 1;
            }
            else 
            {
                combine_bvh(&bvhs[0], &bvhs[i]);
                final = (i - (power<<1)) + 1;
            }
        }
        n = final;
        power <<= 1;
    }
    *bv = bvhs[0];
}

void combine_bvh(struct BVH* b0, struct BVH* b1)
{
    static int new_id = 5;
    if (b1 == NULL) return;
    if (b0 == NULL)
    {
        exit(1);
        printf("SERIOUSERROR!\n");
    }
    struct BVH* newLeft = malloc(sizeof(struct BVH));
    AABB leftBoundingBox; 
    b0->root->get_bounding_extents(b0->root, 
            &leftBoundingBox.min_coord, &leftBoundingBox.max_coord);
    AABB rightBoundingBox; 
    b1->root->get_bounding_extents(b1->root, 
            &rightBoundingBox.min_coord, &rightBoundingBox.max_coord);

    Vector3d new_min = 
    {
        fmin(leftBoundingBox.min_coord.x, rightBoundingBox.min_coord.x),
        fmin(leftBoundingBox.min_coord.y, rightBoundingBox.min_coord.y),
        fmin(leftBoundingBox.min_coord.z, rightBoundingBox.min_coord.z),
    };

    Vector3d new_max = 
    {
        fmax(leftBoundingBox.max_coord.x, rightBoundingBox.max_coord.x),
        fmax(leftBoundingBox.max_coord.y, rightBoundingBox.max_coord.y),
        fmax(leftBoundingBox.max_coord.z, rightBoundingBox.max_coord.z),
    };

    AABB* bounding_box = malloc(sizeof(AABB));
    bounding_box->max_coord = new_max;
    bounding_box->min_coord = new_min;

    Traceable* bounding = malloc(sizeof(Traceable));
    char* name = malloc(30 * sizeof(char));
    sprintf(name, "bounding: %d, %d", b0->root->id, b1->root->id);
    *bounding = AABB_TRACE(bounding_box, WHITE, name);
    bounding->id = new_id++;

    newLeft->root = b0->root;
    newLeft->left = b0->left;
    newLeft->right =b0->right;

    b0->left = newLeft;
    b0->right = b1;
    b0->root = bounding;
}
