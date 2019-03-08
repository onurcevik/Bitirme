#include "testscreen.h"
#include "ui_testscreen.h"

TestScreen::TestScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestScreen)
{
    ui->setupUi(this);
}

TestScreen::~TestScreen()
{
    delete ui;
}

void TestScreen::setRGB24(const unsigned char *data, int xres, int yres)
{
    QImage image(data,xres,yres,QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(image);
    ui->label->setPixmap(pixmap);
}

void TestScreen::setGrayscale(const unsigned char *data, int xres, int yres)
{
    QImage image(data,xres,yres,QImage::Format_Grayscale8);
    QPixmap pixmap = QPixmap::fromImage(image);
    ui->label->setPixmap(pixmap);
}

void TestScreen::setBinaryImage(const unsigned char *data, int xres, int yres)
{
    QImage image(data,xres,yres,QImage::Format_Mono);
    QPixmap pixmap = QPixmap::fromImage(image);
    ui->label->setPixmap(pixmap);
}

void TestScreen::setBinaryImageLSB(const unsigned char *data, int xres, int yres)
{
    QImage image(data,xres,yres,QImage::Format_MonoLSB);
    QPixmap pixmap = QPixmap::fromImage(image);
    ui->label->setPixmap(pixmap);
}
