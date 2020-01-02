// TODO: Sphere is too small. Add other types of objects. Lighting? Maybe.

// World coordinate scheme:
//    +Y
//    ^
//    | screen plane is xy.
//    |
//    *----> +X
//   /    
//  /
// L +Z




#include <stdio.h> 
#include <math.h> 
#include <stdbool.h>

#define MAXDIST 500
#define MINDIST 1.0
#define FOV 1.5708
#define BIGNUM 1000

typedef struct pixel
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} pixel;

typedef struct vec3
{
    double x;
    double y;
    double z;
} vec3;


typedef struct object
{
    enum {SPHERE} type;
    vec3 pos;
    double r; 
    pixel color;
} object;

vec3 mult(vec3 vec, double n)
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

double length(vec3 vector)
{
    return sqrt(vector.x * vector.x +
                vector.y * vector.y +
                vector.z * vector.z);
}

vec3 normalize(vec3 vector)
{
    vec3 r;
    double len = length(vector);
    r.x = vector.x / len;
    r.y = vector.y / len;
    r.z = vector.z / len;
    return r;
}

double dist(object o, vec3 pos)
{
    double d;
    switch(o.type)
    {
        case SPHERE:
            d = sqrt((pos.x - o.pos.x) * (pos.x - o.pos.x) +
                     (pos.y - o.pos.y) * (pos.y - o.pos.y) +
                     (pos.z - o.pos.z) * (pos.z - o.pos.z));
            d -= o.r;
            break;
        default:
            printf("[-] dist() -> SPHERE");
            d = -1;
    }
    return d;
}

int main(void)
{
    //printf("[+] Opening for write file out.ppm.\n");
    // Image data
    FILE *img = fopen("out.ppm", "wb");

    //printf("[+] Initializing variables");
    int width = 100;
    int height = 100;
    int maxcolor = 255 * sizeof(unsigned char); // used for the image format.
    //printf(".");
    pixel data[height][width];
    object sphere = {
                        .pos = {.x = 0.0, .y = 0.0, .z = -10.0},
                        .r = 1.0,
                        .color = {.r = 0, .g = 255, .b = 0}
                    };
    int objnum = 1;
    object objs[] = {sphere};
    //printf(".");

    // Camera position. May or may not implement a moving camera.
    vec3 campos = {
                      .x = 0,
                      .y = 0,
                      .z = 0
                  };
    // Factor is used to make the screen in the world space smaller, as it's
    // valid as long as it keeps the screen in the same aspect ratio.
    double factor = 8;
    // Bottom-left-screen vector + x + y = world space pixel position.
    vec3 bls = {.x = -width/(factor * 2), .y = -height/(factor * 2), .z = -1};
    // bls = add(bls, campos);
    //printf(" Done!\n");
    //printf("[+] Calculating pixel values...");

    // Here we set the pixel color values
    double minimal = 1000;
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
            vec3 pos2D = {.x = x/factor, .y = y/factor, .z = 0.0};
            vec3 pos = add(bls, pos2D);
            vec3 step = mult(normalize(pos), 0.5);
            printf("DEBUG:\nAt x: %d y: %d\nstep: x: %f y: %f z: %f\n", x, y, step.x, step.y, step.z);
            bool reached = false;
            double distance = 0;
            double prevmin = BIGNUM;
            pixel final;
            // printf("DEBUG: start of while loop\n");
            double d;
            while (!reached)
            {
                distance = length(pos);
                // printf("x: %d,y: %d, distance: %d, md: %d\n", x, y, distance, MAXDIST);
                // printf("Current pixel: x=%d, y=%d\nPOS: x=%f y=%f z=%f dist:%f\n",
                //        x, y, pos.x, pos.y, pos.z, distance);
                if (distance > MAXDIST)
                {
                    // Did not encounter any object ("skybox" color).
                    // (pink).
                    final.r = 255;
                    final.g = 0;
                    final.b = 127;
                    reached = true;
                }
                for (int i = 0; i < objnum; i++)
                {
                    // Check the distance with each object.
                    d = dist(objs[i], pos);
                    // printf("DEBUG: d:%f prevmin:%f i:%d\n", d, prevmin, i);
                    if (d < prevmin)
                    {
                        // This if is DEBUG
                        if(d < minimal)
                        {
                            minimal = d;
                        }
                        // printf("DEBUG: prevmin updatede\n");
                        if(d < MINDIST)
                        {
                            printf("hit somethin\n");
                            // Ray hit the object.
                            final = objs[i].color;
                            reached = true;
                        }
                        prevmin = d;
                    }
                    //printf("DEBUG: prevmin: %f\n", prevmin);
                }
                pos = add(pos, step);
                // printf("DEBUG: x: %d y: %d\n", x, y);
                // printf("DEBUG: minimal distance to obj: %f\n", d);
                // printf("DEBUG: vector distance from ro: %f\n", distance);
            }
            // printf("DEBUG: end of while loop\n");
            data[y][x] = final;
        }
    }
    //printf(" Done!\n");

    //printf("[+] Writing data\n");
    // Magic number, width, height, maxcolor, data (rgbrgbrgb // format)
    fprintf(img, "P6\n%d\n%d\n%d\n", width, height, maxcolor);
    fwrite(data,
            sizeof(pixel),
            sizeof(data) / sizeof(pixel),
            img);
    fclose(img);
    //printf("DONE!\n");
    printf("minimal distance to object: %f\n", minimal);
    printf("expected fov: %f\n",atan((width / (2 * factor)) / bls.z));
    return 0;
}
