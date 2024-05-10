#include "mainwindow.h"
#include <QApplication>
#include <iostream>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    for (int i = -128; i < 128; i++) {
        char c = i;
        std::cout << c << std::endl;
        std::cout << (int)(c) << std::endl;

        std::cout << (unsigned char)(c) << std::endl;
        std::cout << (int)((unsigned char)(c)) << std::endl;

        std::cout << std::endl;
    }
    MainWindow w;
    w.show();
    return a.exec();
}
