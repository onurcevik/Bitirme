#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QTime>
#include <QGraphicsScene>
#include<QGraphicsPixmapItem>
#include <QBuffer>
#include "mygraphicsview.h"
#include "reciever.h"
#include "decryption.h"
#include "canny.h"
#include "tracking.h"
#include "backgroundextraction.h"
#include "testscreen.h"
#include "meanshifttracker.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();


public slots:
    void updateGraphicsScene(QBuffer* imageBuffer,qint64 bytes);
    void setProcessingSettings(int x1,int y1,int x2,int y2);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *item;
    QImage *img;
    Reciever *server;
    Tracking *tracking1;
    Tracking *tracking2;
    Canny *nesne1;
    int firstFrame=0;
    int imageProcessing=0;
    int coordinat[4]={0,0,0,0};
    int tmpcoordinat[4]={0,0,0,0};
    uint8_t previusFrame[640*480];
    QTime time;

    BackgroundExtraction *background;
    int frameNumber=1;
    TestScreen *deneme;


};

#endif // MAINWINDOW_H
