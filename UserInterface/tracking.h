#ifndef TRACKING_H
#define TRACKING_H
typedef unsigned char BYTE;



class Tracking{
private:
    //butun goruntu
    int Height;
    int Width;
    BYTE *img;
    int x1;
    int y1;
    int x2;
    int y2;
    int l;
    int m1;
    int m2;
    int n1;
    int n2;
    //belirlenen alanın cevresi yani nesnenin aranacak oldugu bolge
    int searchHeight;
    int searchWidth;
    BYTE *searchImg;
    //secilen obje yani belirlenen goruntu
    int maskHeight;
    int maskWidth;
    BYTE *maskImg;

    int index=0;
    int C=0;
    int gap[2]={1,1};
    int newStartPoint=0;
    int newCoordinate[4]={0,0,0,0};

public:
    void TrackingSet(BYTE *img, int Height, int Width,int x1,int y1,int x2,int y2,int l);
    Tracking();
    ~Tracking();

    void cropSearchImg();
    void cropMaskImg();
    void createSearchMask(int sampleWidth, int sampleHeight);
    void searchObject();
    int *newArea();

    int getHeight() const;
    void setHeight(int value);
    int getWidth() const;
    void setWidth(int value);
    BYTE *getImg() const;
    void setImg(BYTE *value);
    int getSearchHeight() const;
    void setSearchHeight(int value);
    int getSearchWidth() const;
    void setSearchWidth(int value);
    BYTE *getSearchImg() const;
    void setSearchImg(BYTE *value);
    int getMaskHeight() const;
    void setMaskHeight(int value);
    int getMaskWidth() const;
    void setMaskWidth(int value);
    BYTE *getMaskImg() const;
    void setMaskImg(BYTE *value);
};
#endif // TRACKING_H
