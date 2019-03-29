#include "localbinarypattern.h"

LocalBinaryPattern::LocalBinaryPattern()
{

}
/*
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

    int x = shape[0];
    int y = shape[1];
    float r = shape[2];

    // following values store points of rectangle that circle be in
    unsigned int x1 = x - r;
    unsigned int y1 = y - r;
    unsigned int x2 = x + r;
    unsigned int y2 = y + r;

    // directions of values
    // {x1,y1,x2,y2....x8,y8}
    int directions[16] = {-1,-1,-1,0,-1,1,0,1,1,1,1,0,1,-1,0,-1};
    for(unsigned int i=x1;i<x2;i++)
    {
        for(unsigned int j=y1;j<y2;j++)
        {
            if(sqrt(pow((i - x),2) + pow((j - y),2)) > r) continue;
            int sum = 0;
            for(int k=0;k<8;k++)
            {
                if(grayImage[i+directions[2*k]+(j+directions[2*k+1])*imageHeader->getWidth()] <
                        grayImage[i+j*imageHeader->getWidth()]) continue;
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
*/
