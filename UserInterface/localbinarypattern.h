#ifndef LOCALBINARYPATTERN_H
#define LOCALBINARYPATTERN_H

#include <stdint.h>
#include <math.h>

class LocalBinaryPattern
{
public:
    LocalBinaryPattern() = default;
    LocalBinaryPattern(uint8_t *image, int width, int height);
    void rlbp(int* shape, int *histogram);
    float dissimilarity(int *h1, int *h2, int size, int shift);
private:
    uint8_t *image;
    int height, width;
};

#endif // LOCALBINARYPATTERN_H
