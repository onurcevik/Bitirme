#ifndef BACKGROUNDEXTRACTION_H
#define BACKGROUNDEXTRACTION_H
typedef unsigned char BYTE;

class BackgroundExtraction{
private:
    BYTE *inputImg;
    int width;
    int height;
    int numberOfFrame;
    BYTE *arr;
    BYTE *outputImg;

public:
    BackgroundExtraction();
    ~BackgroundExtraction();
    void set(BYTE *inputImg, int width, int height, int numberOfFrame);
    int medianCalculator();
    void dilation();
    void erosion();
    void otsu();


}
#endif // BACKGROUNDEXTRACTION_H
