#ifndef __ppm_h__
#define __ppm_h__

#include <string>

void write_ppm(std::string filename, uint8_t **data, int width, int height);

#endif // __ppm_h__
