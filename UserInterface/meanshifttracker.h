#ifndef MEANSHIFTTRACKER_H
#define MEANSHIFTTRACKER_H
#include <stdint.h>

class MeanShiftTracker
{
    double *pdfOfTarget;
public:
    MeanShiftTracker() = default;
    void calcPdfOfTarget();
    double* calcGradient();
    double bhattacharyyaCoefficient();
    double gaussianKernel();
    double derivativeOfGaussianKernel();
    int kroneckerDelta();
    double* tracking(uint8_t t0, uint8_t t1);
};

#endif // MEANSHIFTTRACKER_H
