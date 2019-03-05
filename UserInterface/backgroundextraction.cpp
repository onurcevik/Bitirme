#include "backgroundextraction.h"
#include <algorithm>
#include <iostream>
#include <string.h>
using namespace std;
BackgroundExtraction::BackgroundExtraction()
{
    inputImg = nullptr;
    outputImg = nullptr;
}

BackgroundExtraction::~BackgroundExtraction()
{
    delete []inputImg;
    delete []outputImg;
}

void BackgroundExtraction::set(BYTE *inputImg, int width, int height, int numberOfFrame)
{
    this->inputImg = inputImg;
    this->width = width;
    this->height = height;
    this->numberOfFrame = numberOfFrame;
    arr = new BYTE[numberOfFrame];
    outputImg = new BYTE[width*height];

}
int BackgroundExtraction::medianCalculator()
{
    int n = sizeof(arr)/sizeof(arr[0]);
    sort(arr, arr+n);
    return arr[numberOfFrame/2];
}

void BackgroundExtraction::dilation()
{
    BYTE *tempBuffer = new BYTE[width*height];
    memcpy(tempBuffer,outputImg, sizeof(tempBuffer));
    int C;
    bool result=0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            C = (i*width + j);
            //result=(Buffer[(C - Width -1)] || Buffer[(C - Width)] || Buffer[(C - Width +1)] || Buffer[(C - 1)] || Buffer[C] || Buffer[(C + 1)] || Buffer[(C + Width - 1)] || Buffer[(C + Width)]|| Buffer[(C + Width + 1)]);
            result = ( tempBuffer[(C - width)] || tempBuffer[(C - 1)] || tempBuffer[C] || tempBuffer[(C + 1)]  || tempBuffer[(C + width)] );


            if (result== true)
                outputImg[C] = 255;
            else
                outputImg[C] = 0;

        }

    }

}

void BackgroundExtraction::erosion()
{
    BYTE *tempBuffer = new BYTE[width*height];
    memcpy(tempBuffer,outputImg, sizeof(tempBuffer));
    int C;
    bool result=0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            C = (i*width + j);
            result = (tempBuffer[(C - Width - 1)] && tempBuffer[(C - Width)] && tempBuffer[(C - Width + 1)] && tempBuffer[(C - 1)] && tempBuffer[C] && tempBuffer[(C + 1)] && tempBuffer[(C + Width - 1)] && tempBuffer[(C + Width)] && tempBuffer[(C + Width + 1)]);
                        //result = (Buffer[(C - Width)] && Buffer[(C - 1)] && Buffer[C] && Buffer[(C + 1)] && Buffer[(C + Width)]);


            if (result== true)
                outputImg[C] = 255;
            else
                outputImg[C] = 0;

        }

    }

}
}
