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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    double get_labl1(bool&);
    double get_labl2(bool&);

    void out_labl3(double);

    void on_pushButton_clicked();

    double sum(double,double);
    double raz(double,double);
    double delen(double,double);
    double umnosh(double,double);
    void on_pushButton_raz_clicked();

    void on_pushButton_delen_clicked();

    void on_pushButton_umnosh_clicked();

    void repit();


private:
    bool flag;
    double (MainWindow::*metod)(double,double);
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
