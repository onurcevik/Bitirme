#include "myvideoobject.h"

MyVideoObject::MyVideoObject(QWidget* parent) :
QVideoWidget(parent)
{
    moveRubberband=false;
    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rubberBand->setGeometry(0,0,50,50);//ileride silebilrisin


}

void MyVideoObject::mouseMoveEvent(QMouseEvent *ev)
{
    rubberBand->setGeometry(QRect(origin,ev->pos()).normalized());
}

void MyVideoObject::mousePressEvent(QMouseEvent *ev)
{
    origin = ev->pos();
    if(!rubberBand)
         rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rubberBand->setGeometry(QRect(origin,QSize()));
    rubberBand->show();
}

void MyVideoObject::mouseReleaseEvent(QMouseEvent *ev)
{
    rubberBand->hide();
    //bool kullanıp yok da edebilirsin
    endPoint = ev->pos();

    painter->begin(this);
    painter->drawRect(QRect(origin,endPoint));

}

void MyVideoObject::paintEvent(QPaintEvent *ev)
{
    painter = new QPainter(this);
    painter->setPen(Qt::red);

}







