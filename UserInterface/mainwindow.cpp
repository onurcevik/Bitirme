#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mediaPlayer = new QMediaPlayer(this);

    videoObject = new MyVideoObject(this);
    ui->horizontalLayout_5->addWidget(videoObject);
    videoObject->resize(500,400);
    mediaPlayer->setVideoOutput(videoObject);




}

MainWindow::~MainWindow()
{
    delete ui;
}






void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Select Video","","video Files (*.mp4)");

    mediaPlayer->stop();
    mediaPlayer->setMedia(QUrl::fromLocalFile(fileName));

    mediaPlayer->play();

}
