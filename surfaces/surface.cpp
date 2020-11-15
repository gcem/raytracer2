#include "surface.h"

thread_local float t_minT;
thread_local int t_matId, t_meshMatId;
thread_local Ray t_normal;

int Surface::getMaterialId() const {
   return 0; 
}
