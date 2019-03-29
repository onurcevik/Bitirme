#include "reciever.h"

#include <QBuffer>
#include <QTcpSocket>
#include <QImage>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>

#define XRES 640
#define YRES 480
#define SIZE 640*480*3

Reciever::Reciever(QObject* parent): QTcpServer(parent)
{
    connect(this, SIGNAL(newConnection()), this, SLOT(addConnection()));
}


void Reciever::addConnection()
{
    QTcpSocket* connection = nextPendingConnection();
    connections.append(connection);
    QBuffer* buffer = new QBuffer(this);
    buffer->open(QIODevice::ReadWrite);
    buffers.insert(connection, buffer);
    connect(connection, SIGNAL(disconnected()), SLOT(removeConnection()));
}

void Reciever::removeConnection()
{
    receiveImage();
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QBuffer* buffer = buffers.take(socket);
    buffer->close();
    buffer->deleteLater();
    connections.removeAll(socket);
    socket->deleteLater();
}

void Reciever::receiveImage()
{
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    QBuffer* buffer = buffers.value(socket);
    //Oku buffera yaz
    qint64 bytes = buffer->write(socket->readAll());
    if(bytes!=0)
        emit sendBuffer(buffer,bytes);
}
