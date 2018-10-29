#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget *parent):
    QGraphicsView (parent), rubberBand(nullptr){}

void MyGraphicsView::mousePressEvent(QMouseEvent *ev)
{

    startPoint = ev->pos();
     qDebug()<<startPoint;
        if(!rubberBand)
            rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        rubberBand->setGeometry(QRect(origin,QSize()));
        rubberBand->show();
        QGraphicsView::mousePressEvent(ev);

}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *ev)
{
     rubberBand->setGeometry(QRect(origin,ev->pos()).normalized());
     QGraphicsView::mouseMoveEvent(ev);

}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *ev)
{
    endPoint = ev->pos();
    qDebug()<<endPoint;
    rect = rubberBand->geometry();
    update();
    QGraphicsView::mouseReleaseEvent(ev);

}


void MyGraphicsView::paintEvent(QPaintEvent *ev)
{

    QGraphicsView::paintEvent(ev);
    QPainter painter(this->viewport());

    painter.setBrush(Qt::red);
     if(!rect.isNull())
        painter.drawRect(rect);



}

