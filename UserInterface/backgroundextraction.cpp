#include "backgroundextraction.h"
#include <algorithm>
#include <iostream>
#include <string.h>
#include <QDebug>
#include <math.h>
using namespace std;


BackgroundExtraction::BackgroundExtraction()
{
    inputGrayImg = nullptr;
    arr = nullptr;
    histogramArr = nullptr;
    backgroundImg = nullptr;
    binaryOutputImg = nullptr;
}

BackgroundExtraction::~BackgroundExtraction()
{
    delete []inputGrayImg;
    delete []arr;
    delete []histogramArr;
    delete []backgroundImg;
    delete []binaryOutputImg;
}

BackgroundExtraction::BackgroundExtraction(int width, int height, int numberOfFrame)
{
    this->width = width;
    this->height = height;
    this->numberOfFrame = numberOfFrame;
    inputGrayImg = new BYTE[width*height*numberOfFrame];
    arr = new BYTE[numberOfFrame];
    histogramArr = new BYTE[256];
    backgroundImg = new BYTE[width*height];
    binaryOutputImg = new BYTE[width*height];

}

void BackgroundExtraction::setInputImgs(BYTE *inputImgs, int frameNumber)
{
    int n = (frameNumber-1)*(width*height);
    for(int i=0; i<height*width; i++)
    {
        inputGrayImg[n+i]=inputImgs[i];
    }
}
int BackgroundExtraction::medianCalculator()
{
    sort(arr, arr+numberOfFrame);
    return arr[numberOfFrame/2];
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

            arr[j]=inputGrayImg[n+i];

        }
         backgroundImg[i] = medianCalculator();
    }
}

void BackgroundExtraction::setForeground(BYTE *inputImg)
{
    for(int i=0; i<height*width; i++)
    {
//        if(fabs(backgroundImg[i]-inputImg[i])==0)
//            binaryOutputImg[i]=0;
//        else
//            binaryOutputImg[i]=inputImg[i];
        binaryOutputImg[i]=fabs(backgroundImg[i]-inputImg[i]);
    }
}
void BackgroundExtraction::histogram()
{
    histogramArr[256] = {0};

    for (int j = 0; j <= width * height; j++)
    {
        histogramArr[binaryOutputImg[j]]++;
    }
}

void BackgroundExtraction::otsu()
{
    //histogram
    double minVariance=99999999;
    int threshold;
    histogram();
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
        if(binaryOutputImg[i] >= threshold)
            binaryOutputImg[i] = 255;
        else
            binaryOutputImg[i] = 0;
    }

}



void BackgroundExtraction::dilation()
{
    BYTE *tempBuffer = new BYTE[width*height];
    //memcpy(tempBuffer,binaryOutputImg, sizeof tempBuffer);
    for(int i=0; i<height*width; i++)
    {
        tempBuffer[i]=binaryOutputImg[i];
    }
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
               binaryOutputImg[C] = 255;
            else
                binaryOutputImg[C] = 0;

        }

    }

}

void BackgroundExtraction::erosion()
{
    BYTE *tempBuffer = new BYTE[width*height];
    //memcpy(tempBuffer,binaryOutputImg, sizeof tempBuffer);
    for(int i=0; i<height*width; i++)
    {
        tempBuffer[i]=binaryOutputImg[i];
    }
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
                binaryOutputImg[C] = 255;
            else
                binaryOutputImg[C] = 0;

        }

    }

}




BYTE *BackgroundExtraction::getInputGrayImg() const
{
    return inputGrayImg;
}

void BackgroundExtraction::setInputGrayImg(BYTE *value)
{
    inputGrayImg = value;
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

BYTE *BackgroundExtraction::getBackgroundImg() const
{
    return backgroundImg;
}

void BackgroundExtraction::setBackgroundImg(BYTE *value)
{
    backgroundImg = value;
}


BYTE *BackgroundExtraction::getBinaryOutputImg() const
{
    return binaryOutputImg;
}

void BackgroundExtraction::setBinaryOutputImg(BYTE *value)
{
    binaryOutputImg = value;
}
