// TODO: obj struct, dist function, constants (#defines: FOV, BIGNUM,
// MAXDIST), add an actual object, add a bright color if it passes through
// MAXDIST, etc...



#include <stdio.h> 
#include <math.h> 

#define MAXDIST 1000

typedef struct pixel
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} pixel;

typedef struct vec3
{
    int x;
    int y;
    int z;
} vec3;

vec3 mult(vec3 vec, float n)
{
    vec3 result;
    result.x = vec.x * n;
    result.y = vec.y * n;
    result.z = vec.z * n;
    return result;
}

float length(vec3 vector)
{
    return sqrt(vector.x * vector.x +
                vector.y * vector.y +
                vector.z * vector.z);
}

void normalize(vec3 * vector)
{
    float len = length(*vector);
    vector->x /= len;
    vector->y /= len;
    vector->z /= len;
}

int main(void)
{
    // Image data
    FILE *img = fopen("out.ppm", "wb");
    int width = 10;
    int height = 10;
    int maxcolor = 255 * sizeof(unsigned char);
    pixel data[height][width];

    // Camera position. May or may not implement a moving camera.
    vec3 campos = vec3
                    {
                        .x = 0,
                        .y = 0,
                        .z = 10
                    };

    // Top-left of the screen (where rays will be captured).
    // Here is also defined the FOV.
    vec3 tls = vec3
                {
                    .x =  float(width)  / 2,
                    .y =  float(height) / 2,
                    .z = (float(width)  / 2) / tan(FOV / 2)
                };
    tls = add(tls, campos);


    // Here we set the pixel color values
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            // Inside here its basically a fragment shader.
            // Meaning it needs a color for each pixel based on the pixel
            // properties.
            // We will make a ray for each pixel

            // March

            // The front face of our camera will point to the negative
            // z axis.

            vec3 pos = add(tls, add(x, y));
            bool reached = false;
            float distance = 0;
            float prevmin = BIGNUM;
            while (!reached && distance > MAXDIST)
            {
                for (int i = 0; i < objnum; i++)
                {
                    float d = dist(objs[i]);
                    if (d < prevmin)
                    {
                        prevmin = d;
                        data[y][x] = objs[i].color;
                    }
                }
                pos = mult(
            }

            // red
            data[y][x].r = ((x % 3) == 0) ? 0xff : 0x0;
            // green
            data[y][x].g = ((x % 3) == 1) ? 0xff : 0x0;
            // blue
            data[y][x].b = ((x % 3) == 2) ? 0xff : 0x0;
        }
    }

    // Magic number, width, height, maxcolor, data (rgbrgbrgb // format)
    fprintf(img, "P6\n%d\n%d\n%d\n", width, height, maxcolor);
    fwrite(data,
            sizeof(pixel),
            sizeof(data) / sizeof(pixel),
            img);
    fclose(img);
    return 0;
}
