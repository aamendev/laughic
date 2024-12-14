#include "./scene.h"
#include <stdio.h>

void simple_tracer(SceneData* scene)
{
    float x, y;
    float* means = malloc((scene->traceable_count + 1) * sizeof(float));
    u32* cols = malloc((scene->traceable_count + 1) * sizeof(u32));

    for (int i = 0; i < scene->traceable_count; i++)
    {
        cols[i] = scene->traceables[i]->col;
    }
    cols[scene->traceable_count] = scene->default_colour;

    for (int j = 0; j < scene->canvas->height; j++)
    {
        for (int i = 0; i < scene->canvas->width; i++)
        {
            for (int i = 0; i < scene->traceable_count; i++) means[i] = 0;
           for (int samplex = 0; samplex < scene->samples; samplex++)
           {
               for (int sampley = 0; sampley < scene->samples; sampley++)
               {
                   x = scene->pixelSize * 
                       (i - 0.5 * (scene->canvas->width - 1) + 
                        (samplex + rand_float()) / scene->samples);
                   y = scene->pixelSize * 
                       (j - 0.5 * (scene->canvas->height - 1) + 
                        (sampley + rand_float()) / scene->samples);
                   scene->ray->base.x = x;
                   scene->ray->base.y = y;
                   for (int i = 0; i < scene->traceable_count; i++)
                   {
                       if (scene->traceables[i]->intersects
                               (scene->ray, scene->traceables[i]->data))
                       {
                            means[i]++; 
                            break;
                       }
                   }
               }
           }
           float sum = 0;
           for (int i = 0; i < scene->traceable_count; i++)
           {
                means[i] /= (scene->samples * scene->samples);
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
