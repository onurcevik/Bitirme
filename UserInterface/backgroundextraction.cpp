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

BackgroundExtraction::BackgroundExtraction(int width, int height, int numberOfFrame)
{
    this->width = width;
    this->height = height;
    this->numberOfFrame = numberOfFrame;
    inputImg = new BYTE[width*height*numberOfFrame];
    arr = new BYTE[numberOfFrame];
    histogramArr = new BYTE[256];
    outputImg = new BYTE[width*height];

}

void BackgroundExtraction::setInputImgs(BYTE *inputImgs, int frameNumber)
{
    int n = (frameNumber-1)*(width*height);
    for(int i=0; i<height*width; i++)
    {
        inputImg[n+i]=inputImgs[i];
    }
}
int BackgroundExtraction::medianCalculator()
{
    int n = sizeof(arr)/sizeof(arr[0]);
    sort(arr, arr+n);
    return arr[numberOfFrame/2];
}

void BackgroundExtraction::histogram()
{
    int numberofpixel = 0;
    for (int i = 0; i<256; i++)
    {
        for (int j = 0; j <= width * height; j++)
        {
            if (i == outputImg[j])
            {
                numberofpixel++;
            }
        }
        histogramArr[i] = numberofpixel;
        numberofpixel = 0;
    }
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
            result = (tempBuffer[(C - width - 1)] && tempBuffer[(C - width)] && tempBuffer[(C - width + 1)] && tempBuffer[(C - 1)] && tempBuffer[C] && tempBuffer[(C + 1)] && tempBuffer[(C + width - 1)] && tempBuffer[(C + width)] && tempBuffer[(C + width + 1)]);
                        //result = (Buffer[(C - Width)] && Buffer[(C - 1)] && Buffer[C] && Buffer[(C + 1)] && Buffer[(C + Width)]);


            if (result== true)
                outputImg[C] = 255;
            else
                outputImg[C] = 0;

        }

    }

}

void BackgroundExtraction::otsu()
{
    double minVariance=99999999;
    int threshold;
    for(int i=0; i<256; i++)
    {
        //BACKGROUND
        //wieght calculator
        double backgroundWeight=0;
        for(int j=0; j<i; j++)
        {
            backgroundWeight+=histogramArr[j];
        }
        backgroundWeight = backgroundWeight/(width*height);
        //mean calculator
        double sum=0;
        double backgroundMean=0;
        for(int j=0; j<i; j++)
        {
            backgroundMean+=(j*histogramArr[j]);
            sum+=histogramArr[j];
        }
        backgroundMean = backgroundMean / sum;
        //variance calculator
        double backgroundVariance=0;
        for(int j=0; j<i; j++)
        {
            backgroundVariance+=(((j-backgroundMean)*(j-backgroundMean))*histogramArr[j]);
        }
        backgroundVariance = backgroundVariance / sum;

        //FOREGROUND
        double foregorundWeight=0;
        for(int j=i; j<256; j++)
        {
            foregorundWeight+=histogramArr[j];
        }
        foregorundWeight = foregorundWeight/(width*height);
        //mean calculator
        sum=0;
        double foregroundMean=0;
        for(int j=i; j<256; j++)
        {
            foregroundMean+=(j*histogramArr[j]);
            sum+=histogramArr[j];
        }
        foregroundMean = foregroundMean / sum;
        //variance calculator
        double foregroundVariance=0;
        for(int j=i; j<256; j++)
        {
            foregroundVariance+=(((j-foregroundMean)*(j-foregroundMean))*histogramArr[j]);
        }
        foregroundVariance = foregroundVariance / sum;

        double inClassVariance = (backgroundWeight*backgroundVariance) + (foregorundWeight*foregroundVariance);
        if(inClassVariance <= minVariance )
        {
            minVariance = inClassVariance;
            threshold = i;
        }
    }

    //BINARY IMG
    for (int i=0; i<width*height; i++)
    {
        if(outputImg[i] >= threshold)
            outputImg[i] = 255;
        else
            outputImg[i] = 0;
    }

}

int BackgroundExtraction::backgroundExtraction()
{
    //median calculator
    int n;
    for(int i=0; i<width*height; i++)
    {
        for(int j=0; j<numberOfFrame; j++)
        {
            n = j*(width*height);
            int a=inputImg[n+i];
            arr[j]=inputImg[n+i];
        }
         outputImg[i] = medianCalculator();
         int b=outputImg[i];
    }
}

BYTE *BackgroundExtraction::getInputImg() const
{
    return inputImg;
}

void BackgroundExtraction::setInputImg(BYTE *value)
{
    inputImg = value;
}

int BackgroundExtraction::getWidth() const
{
    return width;
}

void BackgroundExtraction::setWidth(int value)
{
    width = value;
}

int BackgroundExtraction::getHeight() const
{
    return height;
}

void BackgroundExtraction::setHeight(int value)
{
    height = value;
}

int BackgroundExtraction::getNumberOfFrame() const
{
    return numberOfFrame;
}

void BackgroundExtraction::setNumberOfFrame(int value)
{
    numberOfFrame = value;
}

BYTE *BackgroundExtraction::getArr() const
{
    return arr;
}

void BackgroundExtraction::setArr(BYTE *value)
{
    arr = value;
}

BYTE *BackgroundExtraction::getHistogramArr() const
{
    return histogramArr;
}

void BackgroundExtraction::setHistogramArr(BYTE *value)
{
    histogramArr = value;
}

BYTE *BackgroundExtraction::getOutputImg() const
{
    return outputImg;
}

void BackgroundExtraction::setOutputImg(BYTE *value)
{
    outputImg = value;
}
