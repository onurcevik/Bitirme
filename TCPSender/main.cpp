#include "tcpsender.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TCPSender w;
    w.show();

    return a.exec();
}
