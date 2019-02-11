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
    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    socket = new QTcpSocket(this);
    connect(startButton, &QPushButton::clicked, this, &TCPSender::startConnection);
    connect(quitButton, &QPushButton::clicked, this, &TCPSender::close);
    connect(socket, SIGNAL(connected()), SLOT(startSending()));
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

void TCPSender::sendFrame()
{
    if(socket->state()==QAbstractSocket::ConnectedState){
        auto frame = camera->frame();
//        for(int i=0;i<SIZE;i+=512)
//        {
//            int a = socket->writeDatagram((char *)frame.data+i,512,QHostAddress::LocalHost,45454);
//             qDebug()<<i/512<<" sent"<<a<<" buffer:"<<SIZE;
//        }
//
        image = new QImage(frame.data,XRES,YRES,QImage::Format_RGB888);
        QImage im = image->convertToFormat(QImage::Format_Grayscale8);
        QByteArray ba;
        QBuffer buffer(&ba);
//        image->save(&buffer,"BMP");
        im.save(&buffer,"BMP");
        uint8_t key[16] = {'a', 'y', 's', 'e', 't', 'a', 't', 'i', 'l', 'e', 'c', 'i', 'k', 's','i', 'n'};
        uint8_t* dataPointer = (uint8_t*)ba.data();
        dataPointer+=1078;
        Encryption en(dataPointer,key,128);
        for(int i=0;i<640*480;i+=16)
        {
            en.fastEncrypt();
            dataPointer+=16;
            en.setMessage(dataPointer);
        }
        socket->write(ba);
        int speed = time.msecsTo(QTime::currentTime());
        time = QTime::currentTime();
        speed = 1000*300/speed;
        ui->label->setText(QString("%1 kb/s").arg(speed));
        delete image;

//        for(int i=0;i<SIZE;i+=512)
//        {

//            int a= socket->write((char *)frame.data+i,512);
//             qDebug()<<i/512<<" sent"<<a<<" buffer:"<<SIZE;
//        }
//    } else {
//        qDebug()<<"baglanti yok";

   }

    startConnection();

}
