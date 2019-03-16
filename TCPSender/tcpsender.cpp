#include "tcpsender.h"
#include "ui_tcpsender.h"

#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>

#include <QDebug>
#include <QBuffer>

#define XRES 640
#define YRES 480
#define SIZE 640*480*3


TCPSender::TCPSender(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TCPSender)
{
    ui->setupUi(this);
    statusLabel = new QLabel(tr("Ready to send frames on port 6667"));
    statusLabel->setWordWrap(true);
    startButton = new QPushButton(tr("&Start"));
    auto quitButton = new QPushButton(tr("&Quit"));
    auto testButton = new QPushButton(tr("&Test Camera"));
    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(testButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    socket = new QTcpSocket(this);
    connect(startButton, &QPushButton::clicked, this, &TCPSender::startConnection);
    connect(quitButton, &QPushButton::clicked, this, &TCPSender::close);
    connect(testButton, &QPushButton::clicked, this, &TCPSender::startTestTimer);
    connect(&testTimer, &QTimer::timeout, this, &TCPSender::testCamera);
    connect(socket, SIGNAL(connected()), SLOT(startSending()));
    //connect(&testScreen, SIGNAL(), SLOT(startSending()));
    connect(&timer, &QTimer::timeout, this, &TCPSender::sendFrame);


    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Broadcast Sender"));
    camera = new Camera("/dev/video0", XRES, YRES);
    time = QTime::currentTime();

}

TCPSender::~TCPSender()
{
    delete ui;
    testScreen.close();
}

void TCPSender::startConnection()
{
    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
//        qDebug()<<"Connecting.";
//        socket->connectToHost(QHostAddress::Any, 6667, QIODevice::WriteOnly);
        socket->connectToHost(ui->lineEdit->text(), 6667, QIODevice::WriteOnly);
    }
    else
    {
        socket->disconnectFromHost();
//        qDebug()<<"Disconnecting";
       }

}

void TCPSender::startSending()
{
    startButton->setEnabled(false);
    //timer.start(1000/(24*1800));
    timer.start(1000/48);
}

void TCPSender::startTestTimer()
{
    testTimer.start(1000/24);
}

void TCPSender::sendFrame()
{
    if(socket->state()==QAbstractSocket::ConnectedState){
        auto frame = camera->frame();


        image = new QImage(frame.data,XRES,YRES,QImage::Format_RGB888);
        QImage im = image->convertToFormat(QImage::Format_Grayscale8);
        QByteArray ba;
        QBuffer buffer(&ba);
        im.save(&buffer,"JPEG");
        //qDebug()<<ba.size();

        uint8_t key[16] = {'a', 'y', 's', 'e', 't', 'a', 't', 'i', 'l', 'e', 'c', 'i', 'k', 's','i', 'n'};
               uint8_t* dataPointer = (uint8_t*)ba.data();
               dataPointer+=2;
               Encryption en(dataPointer,key,128);
               for(int i=0;i<(ba.size()-4)/16;i+=16)
               {
                   en.fastEncrypt();
                   dataPointer+=16;
                   en.setMessage(dataPointer);
       }

        socket->write(ba);
        int speed = time.msecsTo(QTime::currentTime());
        time = QTime::currentTime();
        speed = 1000*(ba.size()/1024)/speed;
        ui->label->setText(QString("%1 kb/s").arg(speed));
        delete image;

   }
    startConnection();

}

void TCPSender::testCamera()
{
    testScreen.show();
    auto frame = camera->frame();
    testScreen.setRGB24(frame.data,XRES,YRES);
    //startTestTimer();
}
