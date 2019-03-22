#ifndef BACKGROUNDEXTRACTION_H
#define BACKGROUNDEXTRACTION_H
typedef unsigned char BYTE;

class BackgroundExtraction{
private:
    BYTE *inputGrayImg; //for backgroundextraction
    int width;
    int height;
    int numberOfFrame;
    BYTE *arr;
    BYTE *histogramArr;
    BYTE *backgroundImg;
    BYTE *binaryOutputImg;

public:
    BackgroundExtraction();
    ~BackgroundExtraction();
    BackgroundExtraction(int width, int height, int numberOfFrame);
    //for background extraction
    void setInputImgs(BYTE *inputImgs,int frameNumber);
    int medianCalculator();
    int backgroundExtraction();
    //for binary img
    void setForeground(BYTE *inputImg);
    void histogram();
    void otsu();
    void dilation();
    void erosion();
    void kMeans();



    BYTE *getInputGrayImg() const;
    void setInputGrayImg(BYTE *value);
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
    BYTE *getBackgroundImg() const;
    void setBackgroundImg(BYTE *value);
    BYTE *getBinaryOutputImg() const;
    void setBinaryOutputImg(BYTE *value);
};
#endif // BACKGROUNDEXTRACTION_H
