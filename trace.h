#pragma once

#include "ray.h"
#include "parser.h"
#include <vector>

void trace(std::vector<Ray> &rays, Scene &scene, unsigned char *image);