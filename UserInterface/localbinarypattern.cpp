#include "localbinarypattern.h"

LocalBinaryPattern::LocalBinaryPattern(uint8_t *image, int width, int height)
{
    this->image = image;
    this->width = width;
    this->height = height;
}

void LocalBinaryPattern::rlbp(int * shape, int *histogram)
{
    // rotate inverse local binary pattern algorithm

    const uint8_t indexs[59] = {0, 1, 2, 3, 7, 8, 15, 16, 24, 31, 32,
                            48, 56, 63, 64, 65, 67, 96, 97, 99, 112,
                            113, 115, 120, 121, 123, 124, 126, 127,
                            128, 129, 130, 131, 135, 136, 143, 144,
                            152, 159, 160, 176, 184, 191, 192, 193,
                            195, 224, 225, 227, 240, 241, 243, 248,
                            249, 251, 252, 254, 255, 4}; //4 means 59th index


    auto index = [](auto *indexs, auto number){
        for(int i=0;i<59;i++)
        {
            if(indexs[i]==number) return i;
        }
        return 58;
    };

    for (int i=0;i<59;i++) histogram[i] = 0;

    int x1 = shape[0];
    int y1 = shape[1];
    int x2 = shape[2];
    int y2 = shape[3];


    // directions of values
    // {x1,y1,x2,y2....x8,y8}
    int directions[16] = {-1,-1,-1,0,-1,1,0,1,1,1,1,0,1,-1,0,-1};
    for(unsigned int i=x1;i<x2;i++)
    {
        for(unsigned int j=y1;j<y2;j++)
        {
            int sum = 0;
            for(int k=0;k<8;k++)
            {
                if(image[i+directions[2*k]+(j+directions[2*k+1])*width] <
                        image[i+j*width]) continue;
                sum += pow(2,k);
            }
            histogram[index(indexs, sum)]++;
        }
    }
}

float LocalBinaryPattern::dissimilarity(int *h1, int *h2, int size, int shift)
{
    // dissimilarity metric with chi-square method
    float dissimilarity = 0;
    for(int i=0;i<size;i++)
    {
        float temp = pow((h1[i]-h2[(i+shift)%size]),2)/(h1[i]+h2[(i+shift)%size]);
        if(isnan(temp)) continue;
        dissimilarity += temp;
    }
    return dissimilarity;
}

