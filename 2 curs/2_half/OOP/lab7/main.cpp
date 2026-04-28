#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Создаём и показываем главное окно приложения.
    Widget w;
    w.setWindowTitle("Кочнев Дмитрий 4431");
    w.show();

    return a.exec();
}
