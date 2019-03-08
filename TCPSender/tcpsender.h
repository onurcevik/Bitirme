#ifndef TCPSENDER_H
#define TCPSENDER_H

#include <QTime>
#include <QWidget>
#include <QString>
#include <QTcpSocket>
#include <QTimer>
#include <camera.h>
#include "encryption.h"
#include "testscreen.h"

class QLabel;
class QPushButton;
class QTcpSocket;

namespace Ui {
class TCPSender;
}

class TCPSender : public QWidget
{
    Q_OBJECT

public:
    explicit TCPSender(QWidget *parent = 0);
    ~TCPSender();
private slots:
    void startConnection();
    void startSending();
    void startTestTimer();
    void sendFrame();

    void testCamera();

private:
    Ui::TCPSender *ui;
    QLabel *statusLabel = nullptr;
    QPushButton *startButton = nullptr;
    QTcpSocket *socket = nullptr;
    QTime time;
    QTimer timer, testTimer;
    QImage *image;
    Camera *camera;
    TestScreen testScreen;
};

#endif // TCPSENDER_H
