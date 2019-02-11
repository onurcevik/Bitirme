#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPainter>
#include <QRubberBand>

#include <QPoint>
#include <QDebug>
#include <QLabel>



class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget *parent = nullptr);
protected:

    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);


    void getCoordinates(QPoint leftTop, QPoint rightBottom);
signals:
    void giveCoordinates(int x1,int y1,int x2,int y2);
private:
    QRubberBand *rubberBand;
    QPoint startPoint, endPoint;
};

#endif // MYGRAPHICSVIEW_H
