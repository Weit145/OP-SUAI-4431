#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Создаём и показываем главное окно приложения.
    Widget w;
    w.show();

    return a.exec();
}
