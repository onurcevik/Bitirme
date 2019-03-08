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
    BYTE *histogramArr;
    BYTE *outputImg;

public:
    BackgroundExtraction();
    ~BackgroundExtraction();
    BackgroundExtraction(int width, int height, int numberOfFrame);
    void setInputImgs(BYTE *inputImgs,int frameNumber);
    int medianCalculator();
    void histogram();
    void dilation();
    void erosion();
    void otsu();
    int backgroundExtraction();


    BYTE *getInputImg() const;
    void setInputImg(BYTE *value);
    int getWidth() const;
    void setWidth(int value);
    int getHeight() const;
    void setHeight(int value);
    int getNumberOfFrame() const;
    void setNumberOfFrame(int value);
    BYTE *getArr() const;
    void setArr(BYTE *value);
    BYTE *getHistogramArr() const;
    void setHistogramArr(BYTE *value);
    BYTE *getOutputImg() const;
    void setOutputImg(BYTE *value);
};
#endif // BACKGROUNDEXTRACTION_H
