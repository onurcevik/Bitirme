#include "meanshifttracker.h"



MeanShiftTracker::MeanShiftTracker()
{
    originalPdf = new double[256/radiusOfBin]{0};
    pdfOfTarget = new double[256/radiusOfBin]{0};
}

MeanShiftTracker::~MeanShiftTracker()
{
    delete[] originalPdf;
    delete[] pdfOfTarget;
}

void MeanShiftTracker::setArea(int x1, int y1, int x2, int y2)
{
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    rectWith = abs(x1-x2);
    rectHeight = abs(y1-y2);
}

void MeanShiftTracker::setFrame(int width, int height, uint8_t* frame)
{
    this->width = width;
    this->height = height;
    this->frame = frame;
}

void MeanShiftTracker::calcOriginalPdf()
{
    int centerX = abs(x1+x2)/2;
    int centerY = abs(y1+y2)/2;
    double normalizaationConstant = 0;

    for(int i=x1; i<x2+1; i++)
    {
        for(int j=y1; j<y2+1; j++)
        {
            int x = i - centerX;
            int y = j - centerY;
            double gaussian = gaussianKernel(x*x/(rectWith*rectWith/4) + y*y/(rectHeight*rectHeight/4));
            originalPdf[frame[j*width+i]/radiusOfBin] += gaussian;
            normalizaationConstant += gaussian;
        }
    }

    for(int u=0; u<256/radiusOfBin; u++)
    {
        originalPdf[u] *= (1/normalizaationConstant);
    }

}

void MeanShiftTracker::calcPdfOfTarget(int centerX, int centerY, uint8_t* frame, double *pdf)
{
    double normalizaationConstant = 0;
    int halfHorizon = abs(x2-x1)/2;
    int halfVertical = abs(y2-y1)/2;
    for(int i=centerX-halfHorizon; i<centerX+halfHorizon+1; i++)
    {
        for(int j=centerY-halfVertical; j<centerY+halfVertical+1; j++)
        {
            int x = i - abs(x1+x2)/2;
            int y = j - abs(y1+y2)/2;
            double gaussian = gaussianKernel(x*x/(rectWith*rectWith/4) + y*y/(rectHeight*rectHeight/4));
            pdf[frame[j*width+i]/radiusOfBin] += gaussian;
            normalizaationConstant += gaussian;
        }
    }

    for(int u=0; u<256/radiusOfBin; u++)
    {
        pdf[u] *= (1/normalizaationConstant);
    }
}


void MeanShiftTracker::calcGradient(uint8_t *t1)
{
    int centerX = abs(x1+x2)/2;
    int centerY = abs(y1+y2)/2;

    double divisor = 0;
    for(int i=x1; i<x2+1; i++)
    {
        for(int j=y1; j<y2+1; j++)
        {
            uint8_t value = t1[j*width+i]/radiusOfBin;
            int x = i - centerX;
            int y = j - centerY;
            double gaussian = derivativeOfGaussianKernel(x*x/(rectWith*rectWith/4) + y*y/(rectHeight*rectHeight/4));
            double weight = sqrt(originalPdf[value] / pdfOfTarget[value]);
            divisor += weight*gaussian;
            gradiant[0] += divisor*x/rectWith;
            gradiant[1] += divisor*y/rectHeight;
        }
    }
    gradiant[0] /= divisor;
    gradiant[1] /= divisor;
}

double MeanShiftTracker::bhattacharyyaCoefficient(double *p, double *q)
{
    double temp = 0;
    for(int i=0;i<256/radiusOfBin;i++)
    {
        temp += sqrt(p[i]*q[i]);
    }
    return temp;
}

double MeanShiftTracker::gaussianKernel(double value)
{
    return (1/sqrt(2*M_PI))*exp(-0.5*value);
}

double MeanShiftTracker::derivativeOfGaussianKernel(double value)
{
    return (1/(2*sqrt(2*M_PI)))*exp(-0.5*value);
}

int MeanShiftTracker::kroneckerDelta(int value)
{
    return value == 0;
}

void MeanShiftTracker::tracking(uint8_t *t0, uint8_t *t1, double *points)
{
    frame = t0;
    calcOriginalPdf();
    double sum = 0;
    for(int i=0;i<256/radiusOfBin;i++) sum += originalPdf[i];
    int centerX = abs(x1+x2)/2;
    int centerY = abs(y1+y2)/2;
    calcPdfOfTarget(centerX, centerY, t1, pdfOfTarget);
    sum = 0;
    for(int i=0;i<256/radiusOfBin;i++) sum += pdfOfTarget[i];
    calcGradient(t1);

    double pdfOfEstimatedArea[256/radiusOfBin];
    do
    {
        for(int i=0;i<256/radiusOfBin;i++) pdfOfEstimatedArea[i] = 0;
        calcPdfOfTarget(gradiant[0], gradiant[1], t1, pdfOfEstimatedArea);
        sum = 0;
        for(int i=0;i<256/radiusOfBin;i++) sum += pdfOfEstimatedArea[i];
        gradiant[0] = (centerX + gradiant[0]) / 2;
        gradiant[1] = (centerY + gradiant[1]) / 2;
    }while(bhattacharyyaCoefficient(pdfOfEstimatedArea,originalPdf) <
          bhattacharyyaCoefficient(pdfOfTarget, originalPdf));

    points[0] = gradiant[0];
    points[1] = gradiant[1];
}
