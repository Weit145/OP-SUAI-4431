#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    double x9x12=100;
    double x10x15 = 200;
    double x20x30 = 300;

    double glinec = 40;
    double mat = 80;

private slots:
    void on_pushButton_clicked();
    void repit();
    double get_foto();
    double get_mat();
    int get_kol();
    void set_result(QString);

private:
    Ui::MainWindow *ui;
    bool flag;
    double price_foto=0;
    double price_mat=0;
};
#endif // MAINWINDOW_H
