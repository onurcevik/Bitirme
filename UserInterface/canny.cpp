#include "canny.h"
#include <iostream>
#define PI 3.14159265
#include <math.h>

Canny::Canny()
{
     img = nullptr;
     verticalDerivativeImg  = nullptr;
     horizontalDerivativeImg = nullptr;
     edgeImg  = nullptr;
     nonmaximumSuppresionImg = nullptr;

}


void Canny::CannySet(BYTE *img, int Height, int Width)
{
    this->Height=Height;
    this->Width=Width;
    this->img=img;

    verticalDerivativeImg =new int[(Width - 2)*(Height - 2)];
    horizontalDerivativeImg =new int[(Width - 2)*(Height - 2)];
    edgeImg = new int[(Width - 2)*(Height - 2)];
    nonmaximumSuppresionImg = new BYTE[(Width)*(Height)];

}
Canny::~Canny()
{
    delete []img;
    delete []verticalDerivativeImg;
    delete []horizontalDerivativeImg;
    delete []edgeImg;
    delete []nonmaximumSuppresionImg;
}


void Canny::horizontalDerivative()
{
    index=0;
    for (int i = 1; i < Height - 1; i++)
        {
            for (int j = 1; j < Width - 1; j++)
            {
                C = (i*Width + j);
                // 1 0 -1 -  2 0 -2 -  1 0 -1  maskesini gezidiriyoruz		//kenar yönü dikeyde
                horizontalDerivativeImg[index] = (1 * img[(C - Width - 1)] + 0 * img[(C - Width)] + (-1) * img[(C - Width + 1)]
                                + 2* img[(C - 1)] + 0 * img[C] + (-2) * img[(C + 1)]
                                + 1* img[(C + Width - 1)] + 0 * img[(C + Width)] + (-1) * img[(C + Width + 1)]);
                index++;
            }
        }
}


void Canny::verticalDerivative()
{
    index=0;

    for (int i = 1; i < Height - 1; i++)
        {
            for (int j = 1; j < Width - 1; j++)
            {
                C = (i*Width + j);
                // 1 2 1  -  0 0 0 -  -1 -2 -1  maskesini gezidiriyoruz		//kenar yönü yatayda
                verticalDerivativeImg[index] = (1 * img[(C - Width - 1)] + 2 * img[(C - Width)] + 1 * img[(C - Width + 1)]
                                + 0 * img[(C - 1)] + 0 * img[C] + 0 * img[(C + 1)]
                                + (-1)* img[(C + Width - 1)] + (-2) * img[(C + Width)] + (-1) * img[(C + Width + 1)]);
                index++;
            }
        }

}

void Canny::edgeImage()
{

    index=0;

    for (int i = 0; i < Height - 2; i++)
        {
            for (int j = 0; j < Width -2; j++)
            {
                index = (i*(Width-2) + j);
                // 2 maske sonucunu topluyoruz  not: 255den daha buyuk degeler olabilir max 1020 olabilir
                edgeImg[index]  = abs(verticalDerivativeImg[index]) + abs(horizontalDerivativeImg[index]);

            }
        }

}

BYTE* Canny::nonmaximumSuppresion()
{

    for (int i = 0; i < Height - 2; i++)
        {
            for (int j = 0; j < Width - 2; j++)
            {
                C = (i*(Width-2) + j);

                Qangle = atan2( verticalDerivativeImg[C] , horizontalDerivativeImg[C])*180/PI;		//q=tan^-1((dI/dy)/(dI/dx))		gradient direction

                // buldugumuz aciya gore buyukluk kýyaslamasý yapacagýz ve eger buyukse aynen kalacak degilse 0 atanacak boylece non-maximum suppresion matrisimizi elde edecegiz.
                if ((0 <= Qangle && Qangle < 22.5) || (337.5 <= Qangle && Qangle<= 360) || (157.5 <= Qangle && Qangle < 202.5) || (0 > Qangle && Qangle> -22.5) || (-157.5 >= Qangle && Qangle > -202.5) || (-337.5 >= Qangle && Qangle >= -360))	// acý bu araliktaysa yatayda
                {
                    if (j == 0)
                    {
                        if (edgeImg[C] > edgeImg[C + 1])
                           edgeImg[C] = edgeImg[C];
                        else
                           edgeImg[C] = 0;
                    }
                    else if (j == (Width - 2)-1)
                    {
                        if (edgeImg[C] > edgeImg[C - 1])
                           edgeImg[C] = edgeImg[C];
                        else
                            edgeImg[C] = 0;
                    }
                    else
                    {
                        if (edgeImg[C] > edgeImg[C - 1] && edgeImg[C] > edgeImg[C + 1])
                             edgeImg[C] = edgeImg[C];
                        else
                            edgeImg[C] = 0;
                    }

                }

                else if ((22.5 <= Qangle && Qangle < 67.5) || (202.5 <= Qangle && Qangle < 247.5) || (-112.5 >= Qangle && Qangle > -157.5) || (-292.5 >= Qangle && Qangle > -337.5))		// acý bu araliktaysa caprazda(45 derece)
                {

                    if ((j == 0 && i == 0) || (i == (Height - 2-1) && j == (Width - 2)-1))
                    {
                        edgeImg[C] = edgeImg[C];
                    }

                    else if ((i == 0) || (j == (Width - 2)-1))
                    {
                        if (edgeImg[C] > edgeImg[C + (Width - 2) - 1])
                             edgeImg[C] = edgeImg[C];
                        else
                           edgeImg[C] = 0;
                    }

                    else if ((j == 0) || (i == (Height - 2)-1))
                    {
                        if (edgeImg[C] > edgeImg[(C - (Width - 2) + 1)])
                           edgeImg[C] = edgeImg[C];
                        else
                           edgeImg[C] = 0;
                    }
                    else
                    {
                        if (edgeImg[C] > edgeImg[(C - (Width - 2) + 1)] && edgeImg[C] > edgeImg[C + (Width - 2) - 1])
                            edgeImg[C] = edgeImg[C];
                        else
                            edgeImg[C] = 0;
                    }

                }

                else if ((67.5 <= Qangle && Qangle < 112.5) || (247.5 <= Qangle && Qangle < 292.5) || (-67.5 >= Qangle && Qangle> -112.5) || (-247.5 >= Qangle && Qangle> -292.5))	// acý bu aralikta ise dikeyde (90 derece)
                {
                    if (i == 0)
                    {
                        if (edgeImg[C] > edgeImg[C + (Width - 2)])
                           edgeImg[C] = edgeImg[C];
                        else
                            edgeImg[C] = 0;
                    }
                    else if (i == (Height - 2-1))
                    {
                        if (edgeImg[C] > edgeImg[(C - (Width - 2))])
                           edgeImg[C] = edgeImg[C];
                        else
                            edgeImg[C] = 0;
                    }
                    else
                    {
                        if (edgeImg[C] > edgeImg[C - (Width - 2)] && edgeImg[C] > edgeImg[C + (Width - 2)])
                            edgeImg[C] = edgeImg[C];
                        else
                           edgeImg[C] = 0;
                    }
                }

                else if ((112.5 <=Qangle && Qangle < 157.5) || (292.5 <= Qangle && Qangle < 337.5) || (-22.5 >= Qangle && Qangle > -67.5) || (-202.5 >= Qangle && Qangle > -247.5))	// acý bu aralikta ise caprazda (135 derece)
                {
                    if ((j == 0 && i == (Height - 2)-1) || (i == 0 && j == (Width - 2)-1))
                    {
                        edgeImg[C] = edgeImg[C];
                    }

                    else if (j == 0 || i == 0)
                    {
                        if (edgeImg[C] > edgeImg[C + (Width - 2) + 1])
                            edgeImg[C] = edgeImg[C];
                        else
                            edgeImg[C] = 0;
                    }

                    else if (i == (Height - 2-1) || j == (Width - 2-1))
                    {
                        if (edgeImg[C] > edgeImg[(C - (Width - 2) - 1)])
                           edgeImg[C] = edgeImg[C];
                        else
                            edgeImg[C] = 0;
                    }

                    else
                    {
                        if (edgeImg[C] > edgeImg[(C - (Width - 2) - 1)] && edgeImg[C] > edgeImg[C + (Width - 2) + 1])
                            edgeImg[C] = edgeImg[C];
                        else
                            edgeImg[C] = 0;
                    }

                }

            }
        }



    //goruntudeki max pixel degerini bulma
    max_pixel = 0;
    for (int i = 0; i < Height - 2; i++)
        for (int j = 0; j < Width - 2; j++)
        {
            C = i * (Width - 2) + j;
            if (edgeImg[C] > max_pixel)
                max_pixel =edgeImg[C];

        }

    for (int i = 0; i < Height; i++)
        for (int j = 0; j < Width; j++)
        {
            C = i * (Width) + j;
           nonmaximumSuppresionImg[C]=0;

        }
    //goruntuyu binary cevirme
    for (int i = 0; i < Height - 2; i++)
        {
            for (int j = 0; j < Width - 2; j++)
            {
               C = i * (Width - 2) + j;
               int C2 =(i+1)*Width+(j+1);
                if (edgeImg[C] <= max_pixel/16)
                {
                    nonmaximumSuppresionImg[C2] = 0;
                }
                else
                {
                    nonmaximumSuppresionImg[C2] =255;
                }

            }
        }
//    //goruntuyu binary cevirme
//    for (int i = 0; i < Height - 2; i++)
//        {
//            for (int j = 0; j < Width - 2; j++)
//            {
//               C = i * (Width - 2) + j;
//               int C2 =(i+1)*Width+(j+1);
//                if (edgeImg[C] <= max_pixel/4)
//                {
//                    img[C2] = 0;
//                }
//                else
//                {
//                    img[C2] =255;
//                }

//            }
//        }

    return nonmaximumSuppresionImg;
}

int Canny::getHeight() const
{
    return Height;
}

void Canny::setHeight(int value)
{
    Height = value;
}

int Canny::getWidth() const
{
    return Width;
}

void Canny::setWidth(int value)
{
    Width = value;
}

BYTE *Canny::getImg() const
{
    return img;
}

void Canny::setImg(BYTE *value)
{
    img = value;
}


int *Canny::getVerticalDerivativeImg() const
{
    return verticalDerivativeImg;
}

void Canny::setVerticalDerivativeImg(int *value)
{
    verticalDerivativeImg = value;
}

int *Canny::getHorizontalDerivativeImg() const
{
    return horizontalDerivativeImg;
}

void Canny::setHorizontalDerivativeImg(int *value)
{
    horizontalDerivativeImg = value;
}

int *Canny::getEdgeImg() const
{
    return edgeImg;
}

void Canny::setEdgeImg(int *value)
{
    edgeImg = value;
}


BYTE *Canny::getNonmaximumSuppresionImg() const
{
    return nonmaximumSuppresionImg;
}

void Canny::setNonmaximumSuppresionImg(BYTE *value)
{
    nonmaximumSuppresionImg = value;
}
