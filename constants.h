#pragma once

#include <limits>

#define MAXFLOAT_CONST (std::numeric_limits<float>::max())
#define MINFLOAT_CONST (std::numeric_limits<float>::lowest())

#define BRUTEFORCE_THRESHOLD 8
#define SUBDIVISION_THRESHOLD 50

#define TRIANGLE_EPSILON 0.00001f

// we could check the hardware in runtime but this is ok for ineks
#define THREAD_CNT 8

// number of triangles
#define BOX_MULTITHREAD_THRESHOLD 200

// width should be a multiple of 16 on ineks to make use of 64-byte cache
// lines.
// height should be adjusted for locality.
#define TILE_WIDTH 4
#define TILE_HEIGHT 8