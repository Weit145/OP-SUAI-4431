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
    double_t ice_cream;

    double_t price_chokolate;
    double_t price_caremel;
    double_t price_honey;
    double_t price_peach;
    double_t price_raspberry;
    double_t price_strawberry;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    bool get_ch();
    bool get_cr();
    bool get_ho();
    bool get_pe();
    bool get_ra();
    bool get_st();

    void repit();

    void on_pushButton_clicked();

private:
    double_t result;

    bool flag;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
