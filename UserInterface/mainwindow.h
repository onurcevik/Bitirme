#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QFileDialog>
#include <QVBoxLayout>
#include "myvideoobject.h"


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

    QMediaPlayer* mediaPlayer;
    QVideoWidget* videoWidget;
    MyVideoObject* videoObject;

};

#endif // MAINWINDOW_H
