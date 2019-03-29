#ifndef LOCALBINARYPATTERN_H
#define LOCALBINARYPATTERN_H

#include <stdint.h>
#include <math.h>

class LocalBinaryPattern
{
public:
    LocalBinaryPattern();
    void rlbp(int* shape, int *histogram);
    float dissimilarity(int *h1, int *h2, int size, int shift);
};

#endif // LOCALBINARYPATTERN_H
