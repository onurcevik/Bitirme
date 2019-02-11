#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget *parent):
    QGraphicsView (parent), rubberBand(nullptr){}

void MyGraphicsView::mousePressEvent(QMouseEvent *ev)
{

    startPoint = ev->pos();
    if(!rubberBand)
        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rubberBand->setGeometry(QRect(startPoint,QSize(0,0)));
    rubberBand->show();
    QGraphicsView::mousePressEvent(ev);
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *ev)
{
     rubberBand->setGeometry(QRect(startPoint,ev->pos()).normalized());
     QGraphicsView::mouseMoveEvent(ev);

}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *ev)
{
    endPoint = ev->pos();
    rubberBand->hide();

    update();
    getCoordinates(startPoint,endPoint);
    QGraphicsView::mouseReleaseEvent(ev);

}


//Get coordinates to update rectangles position
void MyGraphicsView::getCoordinates(QPoint leftTop, QPoint rightBottom)
{
    leftTop=startPoint;
    rightBottom=endPoint;

    emit giveCoordinates(startPoint.x(),startPoint.y(),endPoint.x(),endPoint.y());
}

