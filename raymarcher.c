#include <stdio.h> 

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

float length(vec3 vector)
{
    return sqrt(vector.x * vector.x +
                vector.y * vector.y +
                vector.z * vector.z);
}

void normalize!(vec3 * vector)
{
    float len = length(vector);
    vector.x /= len;
    vector.y /= len;
    vector.z /= len;
}

int main(void)
{
    // Image data
    FILE *img = fopen("out.ppm", "wb");
    int width = 10;
    int height = 10;
    int maxcolor = 255 * sizeof(unsigned char);
    pixel data[height][width];

    // Here we set the pixel color values
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            // Inside here its basically a fragment shader.
            // Meaning it needs a color for each pixel based on the pixel
            // properties.
            // We will make a ray for each pixel
            vec3 rayDir
            
            
            
            
            
            
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
