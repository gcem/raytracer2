#pragma once

#include <limits>

#define MAXFLOAT_CONST (std::numeric_limits<float>::max())
#define MINFLOAT_CONST (std::numeric_limits<float>::lowest())

#define BRUTEFORCE_THRESHOLD 4
#define SUBDIVISION_THRESHOLD 12

#define TRIANGLE_EPSILON 0.00001f

// we could check the hardware in runtime but this is ok for ineks
#define THREAD_CNT 8
