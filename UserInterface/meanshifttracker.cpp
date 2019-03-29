#include "meanshifttracker.h"



void MeanShiftTracker::setSelected(bool value)
{
    selected = value;
}

MeanShiftTracker::MeanShiftTracker()
{
    pdfLength = 256/radiusOfBin;
    originalPdf = new double[pdfLength]{0};
    pdfOfTarget = new double[pdfLength]{0};
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
    if(!selected)
    {
        selected = true;
        calcOriginalPdf();
    }
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

    for(int i=x1; i<x2; i++)
    {
        for(int j=y1; j<y2; j++)
        {
            int x = i - centerX;
            int y = j - centerY;
            double gaussian = gaussianKernel(4*x*x/((double)rectWith*rectWith) + 4*y*y/((double)rectHeight*rectHeight));
            originalPdf[frame[j*width+i]/radiusOfBin] += gaussian;
            normalizaationConstant += gaussian;
        }
    }

    for(int u=0; u<pdfLength; u++)
    {
        originalPdf[u] *= (1/(normalizaationConstant));
    }

}

void MeanShiftTracker::calcPdfOfTarget(int centerX, int centerY, uint8_t* frame, double *pdf)
{
    double normalizaationConstant = 0;
    for(int i=centerX-(rectWith/2); i<centerX+(rectWith/2); i++)
    {
        for(int j=centerY-(rectHeight/2); j<centerY+(rectHeight/2); j++)
        {
            int x = i - centerX;
            int y = j - centerY;
            double gaussian = gaussianKernel(4*x*x/((double)rectWith*rectWith) + 4*y*y/((double)rectHeight*rectHeight));
            pdf[frame[j*width+i]/radiusOfBin] += gaussian;
            normalizaationConstant += gaussian;
        }
    }

    for(int u=0; u<pdfLength; u++)
    {
        pdf[u] *= (1/(normalizaationConstant));
    }
}


void MeanShiftTracker::calcGradient(uint8_t *t1)
{
    int centerX = abs(x1+x2)/2;
    int centerY = abs(y1+y2)/2;

    double divisor = 0;
    for(int i=x1; i<x2; i++)
    {
        for(int j=y1; j<y2; j++)
        {
            double weight = 0;
            uint8_t valueR = t1[j*width+i]/radiusOfBin;
            int x = i - centerX;
            int y = j - centerY;
            double gaussian = derivativeOfGaussianKernel(4*x*x/((double)rectWith*rectWith) + 4*y*y/((double)rectHeight*rectHeight));
            weight += sqrt(originalPdf[valueR] / pdfOfTarget[valueR]);
            divisor += weight*gaussian;
            gradiant[0] += weight*gaussian*x;
            gradiant[1] += weight*gaussian*y;
        }
    }
    gradiant[0] /= divisor;
    gradiant[1] /= divisor;
}

double MeanShiftTracker::bhattacharyyaCoefficient(double *p, double *q)
{
    double temp = 0;
    for(int i=0;i<pdfLength;i++)
    {
        temp += sqrt(p[i]*q[i]);
    }
    return temp;
}

double MeanShiftTracker::gaussianKernel(double value)
{
    return (1/sqrt(2*M_PI*2*M_PI))*exp(-0.5*value);
//      return (1/sqrt(2*M_PI))*exp(-0.5*value);
//    return exp(-0.5*value);
//    if(value<=1) return (4-4*value)/(2*M_PI);
//    return 0.0;

}

double MeanShiftTracker::derivativeOfGaussianKernel(double value)
{
    return (1/(2*sqrt(2*M_PI*2*M_PI)))*exp(-0.5*value);
//      return (1/(2*sqrt(2*M_PI)))*exp(-0.5*value);
//    return 0.5*exp(-0.5*value);
//    if(value<=1) return 1;
//    return 0;
}

int MeanShiftTracker::kroneckerDelta(int value)
{
    return value == 0;
}

void MeanShiftTracker::tracking(uint8_t *t1, double *points)
{
    int centerX = abs(x1+x2)/2;
    int centerY = abs(y1+y2)/2;
    int t=0;
    while(t<5){
    calcPdfOfTarget(centerX, centerY, t1, pdfOfTarget);
    double bhattacharyyaValue = bhattacharyyaCoefficient(pdfOfTarget, originalPdf);
    calcGradient(t1);
    gradiant[0] += centerX;
    gradiant[1] += centerY;
    double pdfOfEstimatedArea[pdfLength];
    do
    {
        for(int i=0;i<pdfLength;i++) pdfOfEstimatedArea[i] = 0;
        calcPdfOfTarget(gradiant[0], gradiant[1], t1, pdfOfEstimatedArea);
        gradiant[0] = (centerX + gradiant[0]) / 2;
        gradiant[1] = (centerY + gradiant[1]) / 2;
        if( ((gradiant[0]-centerX)*(gradiant[0]-centerX) + (gradiant[1]-centerY)*(gradiant[1]-centerY)) < 1 ) break;
    }while(bhattacharyyaCoefficient(pdfOfEstimatedArea,originalPdf) <
          bhattacharyyaValue);

    points[0] = gradiant[0];
    points[1] = gradiant[1];
    centerX = gradiant[0];
    centerY = gradiant[1];
    t++;
    }
    if(points[1]-(rectHeight/2)<0)
        points[1]=(rectHeight/2);
    else if(points[1]+(rectHeight/2)>height) points[1]=height-(rectHeight/2);
}
