#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdio.h>
#include <math.h>

/**
 * Ray struct
 */
struct Rays {
    float pos[3];
    float dir[3];
};

/**
 * Creates ray struct
 * 
 * @param vec pixel location vector
 * @return ray object
 */
struct Rays getRay(float vec[3]){
    struct Rays ray;

    ray.pos[0] = 0.0;
    ray.pos[1] = 0.0;
    ray.pos[2] = 0.0;

    ray.dir[0] = vec[0] - ray.pos[0];
    ray.dir[1] = vec[1] - ray.pos[1];
    ray.dir[2] = vec[2] - ray.pos[2];

    return ray;
}

/**
 * Sphere struct
 */
struct Balls {
    float cen[3];
    float rad;
    int col[3];
};

/**
 * Creates ball struct
 * 
 * @param cen vector of center of ball
 * @param rad radius of ball
 * @param col color of ball
 * @return ball object
 */
struct Balls getBall(float cen[3], float rad, int col[3]){
    struct Balls sphere;

    sphere.cen[0] = cen[0];
    sphere.cen[1] = cen[1];
    sphere.cen[2] = cen[2];
    sphere.rad = rad;
    sphere.col[0] = col[0];
    sphere.col[1] = col[1];
    sphere.col[2] = col[2];

    return sphere;
}

/**
 * Computes dot product of two vectors
 * 
 * @param vec1 vector 1
 * @param vec2 vector 2
 * @return dot product of input vectors
 */
float dotProduct(float vec1[3], float vec2[3]){
    return vec1[0]*vec2[0] + vec1[1]*vec2[1] + vec1[2]*vec2[2];
}

/**
 * Computes vector from subbing vec2 from vec1
 * 
 * @param vec1 vector 1
 * @param vec2 vector 2
 * @param vec3 output vector
 * @return result vector
 */
float * vectorSub(float vec1[3], float vec2[3], float vec3[3]){
    vec3[0] = vec1[0] - vec2[0];
    vec3[1] = vec1[1] - vec2[1];
    vec3[2] = vec1[2] - vec2[2];
    return vec3;
}

/**
 * Computes t value of intersection of a ray and sphere
 * 
 * @param ray ray struct
 * @param sphere ball struct
 * @return dtime value
 */
float sphereIntersect(struct Rays ray, struct Balls sphere){
    float res1, res2, t;
    float newVec[3]; 
    vectorSub(ray.pos, sphere.cen, newVec);

    //calculate discriminate
    float disc = pow( dotProduct(ray.dir, newVec), 2) - dotProduct(ray.dir, ray.dir) * ( dotProduct( newVec, newVec ) - pow( sphere.rad, 2 ));
    //if discriminate is negative, return arbitrary negative
    if (disc < 0){
        return -1;
    }

    //check both solutions (could be the same)
    res1 = ( -dotProduct( ray.dir, newVec ) + sqrt( disc ) ) / dotProduct( ray.dir, ray.dir );
    res2 = ( -dotProduct( ray.dir, newVec ) - sqrt( disc ) ) / dotProduct( ray.dir, ray.dir );
    
    //if either res is negative, return arbitrary negative
    if (res1 < 0 || res2 < 0) {
        return -1;
    }

    //set lowest of the two results
    if (res1 < res2) {
        t = res1;
    } else {
        t = res2;
    }

    return t;
}

/**
 * Normalizes vector with 3 dimensions
 * 
 * @param vec pixel location vector
 * @return normalized vector
 */
float * normalize(float vec[3]){
    float magnitude = sqrt(pow(vec[0],2.0) + pow(vec[1], 2.0) + pow(vec[2],2.0));
    vec[0] = vec[0] / magnitude;
    vec[1] = vec[1] / magnitude;
    vec[2] = vec[2] / magnitude;
    return vec;
}

int main(){
    //vars for image array
    int width = 512;
    int height = width;
    int chan = 3;
    int arrLen = width*height*chan;
    unsigned char* image = (unsigned char*) malloc(arrLen*sizeof(int));

    //var for image plane and camera
    float pixWidth = 2.0/width;
    float pixTopLeft[3] = {-1 + (pixWidth/2.0), 1 - (pixWidth/2.0), -2.0};

    //other vars
    struct Rays ray;
    int index;
    struct Balls sphere;
    float cen[3] = {2.0, 2.0, -16.0};
    int col[3] = {255, 255, 255};
    float rad = 5.3547;
    sphere = getBall(cen, rad, col);
    int missCol[3] = {128, 0, 0};

    //main loop, loops each pixel in image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            //index of pixel in image array
            index = (x + (width * y)) * 3;

            //location of pixel on image plane
            float vec[3];
            vec[0] = pixTopLeft[0] + pixWidth * x;
            vec[1] = pixTopLeft[1] - pixWidth * y;
            vec[2] = pixTopLeft[2];

            //get ray struct for pixel
            ray = getRay(normalize(vec));

            //print info on specific pixels:
            //bot left, top right, mid
            if((x == 0 && y == height-1) || (x == width-1 && y == 0) || (x == (width/2)-1 && y == (height/2)-1)){
                if(x == 0){
                    printf("Bottom left pixel\n");
                } else if (x == width-1) {
                    printf("Top right pixel\n");
                } else if (x == (width/2)-1) {
                    printf("Middle pixel\n");
                }
                printf("%d %d\n", x, y);
                printf("RayPosition %f %f %f\n", ray.pos[0], ray.pos[1], ray.pos[2]);
                printf("RayDirection %f %f %f\n\n", ray.dir[0], ray.dir[1], ray.dir[2]);
            }

            //check for sphere intersection and color pixels if collide
            if (sphereIntersect(ray, sphere) > 0){
                image[index] = sphere.col[0];
                image[index+1] = sphere.col[1];
                image[index+2] = sphere.col[2];
            } else {
                image[index] = missCol[0];
                image[index+1] = missCol[1];
                image[index+2] = missCol[2];
            }
        }
    }

    //write image to file and free mem
    stbi_write_png("sphere.png", width, height, chan, image, width*chan);
    free(image);

    return 0;
}