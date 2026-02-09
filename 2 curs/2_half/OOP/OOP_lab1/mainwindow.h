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
    int64_t get_labl1();
    int64_t get_labl2();

    void out_labl3(int);

    void on_pushButton_clicked();

    int sum(int,int);
    int raz(int,int);
    int delen(int,int);
    int umnosh(int,int);
    void on_pushButton_raz_clicked();

    void on_pushButton_delen_clicked();

    void on_pushButton_umnosh_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
