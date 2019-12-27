// TODO: solve bug in getting the color. Now any distance will already be
// accepted;



#include <stdio.h> 
#include <math.h> 

#define MAXDIST 1000
#define FOV 1.5708
#define BIGNUM 100

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


typedef struct object
{
    enum {SPHERE} type;
    vec3 pos;
    float r; 
    pixel color;
} object;

vec3 mult(vec3 vec, float n)
{
    vec3 result;
    result.x = vec.x * n;
    result.y = vec.y * n;
    result.z = vec.z * n;
    return result;
}

vec3 add(vec3 a, vec3 b)
{
    vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
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

float dist(object o, vec3 pos)
{
    float d;
    switch(o.type)
    {
        case SPHERE:
            d = sqrt(((pos.x - o.pos.x) * (pos.x - o.pos.x),
                           (pos.y - o.pos.y) * (pos.y - o.pos.y),
                           (pos.z - o.pos.z) * (pos.z - o.pos.z)));
            d -= o.r;
            break;
        default:
            printf("[-] dist() -> SPHERE");
            d = -1;
    }
}

int main(void)
{
    // Image data
    FILE *img = fopen("out.ppm", "wb");
    int width = 200;
    int height = 200;
    int maxcolor = 255 * sizeof(unsigned char);
    pixel data[height][width];
    object sphere = {.pos = {.x = 0.0, .y = 0.0, .z = 0.0},
        .r = 1.0,
        .color = {.r = 255, .g = 0, .b = 0}};
    int objnum = 1;
    object objs[] = {sphere};

    // Camera position. May or may not implement a moving camera.
    vec3 campos =
                    {
                        .x = 0,
                        .y = 0,
                        .z = 10
                    };

    // Top-left of the screen (where rays will be captured).
    // Here is also defined the FOV.
    vec3 tls =
                {
                    .x =  ((float) width)  / 2,
                    .y =  ((float) height) / 2,
                    .z = (((float) width)  / 2) / tan(FOV / 2)
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
            vec3 pos2D = {.x = x, .y = y};
            vec3 pos = add(tls, pos2D);
            unsigned char reached = 0; // false
            float distance = 0;
            float prevmin = BIGNUM;
            pixel final;
            while (!reached && distance > MAXDIST)
            {
                distance = length(pos);
                if (distance > MAXDIST)
                {
                    final.r = 255;
                    final.g = 0;
                    final.b = 127;

                    break;
                }
                for (int i = 0; i < objnum; i++)
                {
                    float d = dist(objs[i], pos);
                    if (d < prevmin)
                    {
                        prevmin = d;
                        final = objs[i].color;
                    }
                }
                pos = add(pos, pos);
            }
            data[y][x] = final;
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
