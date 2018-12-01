#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    mediaPlayer = new QMediaPlayer(this);
    videoItem = new QGraphicsVideoItem;
    scene = new QGraphicsScene(this);
    QSize videoSize = ui->graphicsView->size();
    videoItem->setSize(videoSize);


    ui->graphicsView->setScene(scene);

    mediaPlayer->setVideoOutput(videoItem);
    ui->graphicsView->scene()->addItem(videoItem);
    ui->graphicsView->show();


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
