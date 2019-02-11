#ifndef RECIEVER_H
#define RECIEVER_H

#include <QDebug>

#include <QBuffer>
#include <QDebug>
#include <QImage>
#include <QImageWriter>
#include <QTimer>
#include <QTcpServer>


class QBuffer;
class QTcpSocket;


class Reciever : public QTcpServer
{
    Q_OBJECT
public:
    Reciever(QObject* parent = 0);

signals:
    void sendBuffer(QBuffer* imageBuffer,qint64 bytes);

private slots:
    void addConnection();
    void removeConnection();
    void receiveImage();


private:

    QList<QTcpSocket*> connections;
    QHash<QTcpSocket*, QBuffer*> buffers;
    //int imageNumber= 0;

};

#endif // RECIEVER_H
