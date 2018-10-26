#ifndef MYVIDEOOBJECT_H
#define MYVIDEOOBJECT_H

#include <QObject>
#include <QVideoWidget>
#include <QRubberBand>
#include <QPainter>
#include <QPen>
#include <QPaintEvent>
#include <QRect>
#include <QMouseEvent>
#include <QDebug>

class MyVideoObject : public QVideoWidget
{
    Q_OBJECT
public:
    explicit MyVideoObject(QWidget *parent = 0);

    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *ev);

private:
    QRubberBand* rubberBand;
    QPainter* painter;
    bool moveRubberband;


    QPoint origin;
    QPoint endPoint;

    QRect *rectangle;

};



#endif // MYVIDEOOBJECT_H
