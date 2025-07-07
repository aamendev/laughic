# Laughic

Some graphics algorithms to draw circles, ellipses, rectangles, lines, triangles (with barry-centeric coordinates) and applying textures.

## showcase

### General
Simple drawing functions that enable basic shapes and simple alpha blending.
![showcase img](./imgs/showcase)

### Fractals
Some builtin fractal functions.
![showcase2 img](./imgs/fractal_showcase)
![showcase2 img2](./imgs/julia_n079_p01889)

### Ray Tracing
Basic raytracing supporting reflections, specular and diffuse material and shadow casting.
This one is very generic and can easily be built on by further brdfs structs and light types provided that they follow the provided contracts.
![showcase raytrace](./imgs/raytracer/new_light)

### Image Processing
Some builtin image processing abilities.
#### Blur
![showcase blur](./imgs/filters/binomial_2)
#### Sobel
![showcase sobel](./imgs/filters/sobel1)
#### Canny
![showcase Cany](./imgs/filters/canny)

#### GreyScale
![showcase grey](./imgs/processing/grey_scale)
#### Colour Manipulation
Colors can be manipulated through numerous ways (e.g. inverting colours, clamping a certain channel to min or max values, or filtering regions within certain ranges of any colour channel)
##### colour inversion
![showcase invertColours](./imgs/processing/invert)
##### clamping min blue value
![showcase invertColours](./imgs/processing/changeMinBlue)
##### recontrasting
![showcase col](./imgs/processing/recontr)
##### increment red in all regions
![showcase col](./imgs/processing/reddishStuff)
##### remove red regions
![showcase col](./imgs/processing/redFilter)

### Path and Style
Ability to draw curves (path) while applying arbitrary modifications alongside it (style). A basic building block for future npr.
![showcase ps](./imgs/npr/curve2)
