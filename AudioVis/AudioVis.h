#pragma once

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

//#define DEBUG_DRAW

// How many samples per pass we buffer
#define BUFFER_SIZE 16384

// How many "Buckets" of frequencies we want to divide the total number of samples into
#define RAW_BUCKET_COUNT 200

// How many usable buckets of samples do we want to average out of the raw buckets above
// Note RAW_BUCKET_COUNT should be divisible by this value
#define OUTPUT_BUCKET_COUNT 4

// How fast we want to rotate our model in degrees/zec
#define ROTATION_SPEED 1