#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QMediaPlayer>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QSize>
#include "mygraphicsview.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsVideoItem *videoItem;
    QMediaPlayer* mediaPlayer;

};

#endif // MAINWINDOW_H
