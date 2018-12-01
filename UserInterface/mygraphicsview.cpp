#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget *parent):
    QGraphicsView (parent), rubberBand(nullptr){}

void MyGraphicsView::mousePressEvent(QMouseEvent *ev)
{

    startPoint = ev->pos();
      qDebug()<<"START POINT!: "<<startPoint;
     if(!rubberBand)
             rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        rubberBand->setGeometry(QRect(startPoint,QSize()));
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
    qDebug()<<"END POINT!: "<<endPoint;

    rect = rubberBand->geometry();
    rubberBand->hide();
    //update(startPoint.x(),startPoint.y(),rect.width(),rect.height());
    update();
    QGraphicsView::mouseReleaseEvent(ev);

}


void MyGraphicsView::paintEvent(QPaintEvent *ev)
{

    QGraphicsView::paintEvent(ev);
    QPainter painter(this->viewport());


    painter.setBrush(QBrush(QColor(255,0,0,0)));
     if(!rect.isNull()){
          painter.drawRect(rect);
     }



}



//Get coordinates to update rectangles position
void MyGraphicsView::getCoordinates(QPoint leftTop, QPoint rightBottom)
{

}

