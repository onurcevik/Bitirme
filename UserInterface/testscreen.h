#ifndef TESTSCREEN_H
#define TESTSCREEN_H

#include <QWidget>

namespace Ui {
class TestScreen;
}

class TestScreen : public QWidget
{
    Q_OBJECT

public:
    explicit TestScreen(QWidget *parent = 0);
    ~TestScreen();
    void setRGB24(const unsigned char* data, int xres, int yres);
    void setGrayscale(const unsigned char* data, int xres, int yres);
    void setBinaryImage(const unsigned char* data, int xres, int yres);
    void setBinaryImageLSB(const unsigned char* data, int xres, int yres);
private:
    Ui::TestScreen *ui;
};

#endif // TESTSCREEN_H
