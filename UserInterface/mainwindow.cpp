#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QDebug>
#include<QPixmap>
#include <string>
#include <iostream>
using namespace std;

#define XRES 640
#define YRES 480
#define SIZE 640*480*3

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->show();
    img = new QImage();
    item = new QGraphicsPixmapItem();
    scene->addItem(item);

    server = new Reciever();
    connect(server,SIGNAL(sendBuffer(QBuffer*,qint64)),this,SLOT(updateGraphicsScene(QBuffer*,qint64)));
    server->listen(QHostAddress::Any,6667);
    connect(ui->graphicsView,SIGNAL(giveCoordinates(int,int,int,int)),this,SLOT(setProcessingSettings(int,int,int,int)));
    time = QTime::currentTime();
    background = new BackgroundExtraction(640,480,50);
    deneme = new TestScreen;
    tracker = new MeanShiftTracker;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateGraphicsScene(QBuffer* imageBuffer,qint64 bytes)
{
    //okudugun kadar geri git
    imageBuffer->seek(imageBuffer->pos() - bytes);
    int speed = time.msecsTo(QTime::currentTime());
    time = QTime::currentTime();
    speed = 1000*(imageBuffer->data().size()/1024)/speed;
    ui->label->setText(QString("%1 kb/s").arg(speed));
//    ui->label_2->setText(QString("%1 fps").arg(speed/300));
    if(ui->radioButton->isChecked())
       {
           uint8_t key[16] = {'a', 'y', 's', 'e', 't', 'a', 't', 'i', 'l', 'e', 'c', 'i', 'k', 's','i', 'n'};
           uint8_t *dataPointer = (uint8_t*)imageBuffer->data().data();
           dataPointer+=2;
           Decryption de(dataPointer,key,128);
           for(int i=0;i<(imageBuffer->data().size()-4)/16;i+=16)
           {
               de.fastDecrypt();
               dataPointer+=16;
               de.setMessage(dataPointer);
           }
     }

    //++++++++++++++++++++++++++++++++ARKA PLAN CIKARMA ISLEMLERI++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    QPixmap pix;
    pix.loadFromData((unsigned char*)imageBuffer->data().data(), imageBuffer->data().size(), "JPG");
    QImage im = pix.toImage();
    im = im.convertToFormat(QImage::Format_Grayscale8);
    QByteArray ba;
    QBuffer buffer(&ba);
    im.save(&buffer,"BMP");

   BYTE *imgData =(unsigned char*)ba.data();
   imgData+=1078;
/*
       if(a==0){
           background->setInputImgs(imgData,frameNumber);
           frameNumber++;
       }

       if(frameNumber>50 && a==0)
       {
         background->backgroundExtraction();
         a=1;
         //frameNumber=1;
       }
      if(a==1)
      {
          background->setForeground(imgData);
          background->otsu();
          //background->kMeans();
          background->erosion();
          background->dilation();

      }


       deneme->show();
       deneme->setGrayscale(background->getBinaryOutputImg(),XRES,YRES);
*/

    //+++++++++++++++++++++++++++++++++++GORUNTU ISLEM BASLANGIC++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(imageProcessing==1)
        {
            tracker->setFrame(640,480,previusFrame);
            tracker->setArea(tmpcoordinat[0],tmpcoordinat[1],tmpcoordinat[2],tmpcoordinat[3]);
            tracker->tracking(imgData, meanPoints);
            qDebug()<< "x: " << meanPoints[0] << " y: " << meanPoints[1];
            tmpcoordinat[0] = meanPoints[0]-width;
            tmpcoordinat[2] = meanPoints[0]+width;
            tmpcoordinat[1] = meanPoints[1]-height;
            tmpcoordinat[3] = meanPoints[1]+height;
            for(int i=tmpcoordinat[1]; i<tmpcoordinat[3]; i++)
                {
                    imgData[i*640+tmpcoordinat[0]] = 255;
                    imgData[i*640+tmpcoordinat[2]]= 255;
                }
                for(int i=tmpcoordinat[0]; i<tmpcoordinat[2]; i++)
                {
                    imgData[tmpcoordinat[1]*640+i] = 255;
                    imgData[tmpcoordinat[3]*640+i] = 255;
                }
        }


        memcpy(previusFrame, imgData, 640*480);

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++GORUNTU ISLEME BITIS+++++++++++++++++++++++++++++++++++++++++++++++++++



    //QImage img;
    img->loadFromData(ba);
    QPixmap pixmap = QPixmap::fromImage(*img);
    item->setPixmap(pixmap);

    ui->graphicsView->show();
//    if (img->isNull())           // Check if the image was indeed received
//            qDebug("The image is null. Something failed.");
//    qDebug("image received");
}

void MainWindow::setProcessingSettings(int x1, int y1, int x2, int y2)
{
    firstFrame = 0;
    imageProcessing = 0;

    if(x2<x1)
    {
        int swap =x2;
        x2=x1;
        x1=swap;
    }
    if(y2<y1)
    {
        int swap = y2;
        y2=y1;
        y1=swap;
    }
    tmpcoordinat[0]=x1;
    tmpcoordinat[1]=480-y2;
    tmpcoordinat[2]=x2;
    tmpcoordinat[3]=480-y1;
    imageProcessing=1;

    height = (tmpcoordinat[3]-tmpcoordinat[1])/2;
    width = (tmpcoordinat[2]-tmpcoordinat[0])/2;
    qDebug()<< "X: " << (tmpcoordinat[0]+tmpcoordinat[2])/2 << " Y: " << (tmpcoordinat[1]+tmpcoordinat[3])/2;
    tracker->setSelected(false);

}
