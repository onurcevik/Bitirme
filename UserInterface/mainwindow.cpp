#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QDebug>
#include<QPixmap>

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
    tracking1 = new Tracking();
    tracking2 = new Tracking();
    nesne1 =new Canny();
    time = QTime::currentTime();

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
    speed = 1000*300/speed;
    ui->label->setText(QString("%1 kb/s").arg(speed));
//    ui->label_2->setText(QString("%1 fps").arg(speed/300));
    if(ui->radioButton->isChecked())
    {
        uint8_t key[16] = {'a', 'y', 's', 'e', 't', 'a', 't', 'i', 'l', 'e', 'c', 'i', 'k', 's','i', 'n'};
        uint8_t *dataPointer = (uint8_t*)imageBuffer->data().data();
        dataPointer+=1078;
        Decryption de(dataPointer,key,128);
        for(int i=0;i<640*480;i+=16)
        {
            de.fastDecrypt();
            dataPointer+=16;
            de.setMessage(dataPointer);
        }
    }
    //+++++++++++++++++++++++++++++++++++GORUNTU ISLEM BASLANGIC++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

       BYTE *imgData =(unsigned char*)imageBuffer->data().data();
       imgData+=1078;



       if(firstFrame==1)
       {
           //1.FRAMEDEN NESNE(MASKIMG) GELDİ 2. FRAME ISLEDIM VE 2.FRAMEDE NESNEYI ARADIM VE NESNENIN 2.FRAMEDEKI KORDINATLARINI BULDUM
           //2. framede 1. framden aldığım nesneyi arıcam
           tracking2->TrackingSet(imgData,480,640,coordinat[0],coordinat[1],coordinat[2],coordinat[3],25);
           tracking2->cropSearchImg();
           nesne1->CannySet(tracking2->getSearchImg(),tracking2->getSearchHeight(),tracking2->getSearchWidth());
           nesne1->verticalDerivative();
           nesne1->horizontalDerivative();
           nesne1->edgeImage();
           tracking2->setSearchImg(nesne1->nonmaximumSuppresion());

          //arama ve yeni noktalar
           tracking2->setMaskImg(tracking1->getMaskImg());
           tracking2->createSearchMask(25,25);
           tracking2->searchObject();
           tracking2->newArea();
           int *newCoordinate;
           newCoordinate=tracking2->newArea();
           for(int i=0; i<4; i++)
               coordinat[i]=newCoordinate[i];
           //cızdırme

           for(int i=coordinat[1]; i<coordinat[3]; i++)
              {
                  imgData[i*640+coordinat[0]] = 255;
                  imgData[i*640+coordinat[2]]= 255;
              }
              for(int i=coordinat[0]; i<coordinat[2]; i++)
              {
                  imgData[coordinat[1]*640+i] = 255;
                  imgData[coordinat[3]*640+i] = 255;
              }

           //KORDINATLARI BULDUKDAN SONRA, 2.FRAME YENI KORDINATLAR ILE ISLEDIM VE NESNEYİ 3.FRAME'E VERMEK ICIN KOPYALADIM
          tracking1->TrackingSet( imgData,480,640,coordinat[0],coordinat[1],coordinat[2],coordinat[3],25);
          tracking1->cropSearchImg();
           //1. framden hem search alanını hem mask alanını cıkardım canny uyguladım
          nesne1->CannySet(tracking1->getMaskImg(),tracking1->getMaskHeight(),tracking1->getMaskWidth());
          nesne1->verticalDerivative();
          nesne1->horizontalDerivative();
          nesne1->edgeImage();
         tracking1->setMaskImg( nesne1->nonmaximumSuppresion());

       }

       //ILK FRAME
       if(imageProcessing==1)
       {
           coordinat[0]=tmpcoordinat[0];
           coordinat[1]=tmpcoordinat[1];
           coordinat[2]=tmpcoordinat[2];
           coordinat[3]=tmpcoordinat[3];

           tracking1->TrackingSet(imgData,480,640,coordinat[0],coordinat[1],coordinat[2],coordinat[3],25);
           tracking1->cropSearchImg();

           nesne1->CannySet(tracking1->getMaskImg(),tracking1->getMaskHeight(),tracking1->getMaskWidth());
           nesne1->verticalDerivative();
           nesne1->horizontalDerivative();
           nesne1->edgeImage();
           tracking1->setMaskImg( nesne1->nonmaximumSuppresion());


           firstFrame=1;
           imageProcessing=0;
       }

       //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++GORUNTU ISLEME BITIS+++++++++++++++++++++++++++++++++++++++++++++++++++


    //QImage img;
    img->loadFromData(imageBuffer->buffer());
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
    imageProcessing = 1;

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

}
