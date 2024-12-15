#include "./scene.h"
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
