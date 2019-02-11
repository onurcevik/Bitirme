#ifndef CANNY_H
#define CANNY_H
typedef unsigned char BYTE;


class Canny{
private:
    int Height;
    int Width;
    BYTE *img;

    int *verticalDerivativeImg ;
    int *horizontalDerivativeImg;
    int *edgeImg;
    BYTE *nonmaximumSuppresionImg;

    int C=0;    //center
    int index=0;
    float Qangle=0;
    int max_pixel = 0;

public:
    Canny();
    void CannySet(BYTE *img, int Height, int Width);
    ~Canny();


    void horizontalDerivative();
    void verticalDerivative();
    void edgeImage();
    BYTE* nonmaximumSuppresion();


    int getHeight() const;
    void setHeight(int value);
    int getWidth() const;
    void setWidth(int value);
    BYTE *getImg() const;
    void setImg(BYTE *value);
    int *getVerticalDerivativeImg() const;
    void setVerticalDerivativeImg(int *value);
    int *getHorizontalDerivativeImg() const;
    void setHorizontalDerivativeImg(int *value);
    int *getEdgeImg() const;
    void setEdgeImg(int *value);
    BYTE *getNonmaximumSuppresionImg() const;
    void setNonmaximumSuppresionImg(BYTE *value);
};
#endif // CANNY_H
