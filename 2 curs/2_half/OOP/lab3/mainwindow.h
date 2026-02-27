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

private slots:
    double get_l();
    double get_s();
    double get_t();

    void set_t(QString);
    void set_result(QString);

    void repit();

    void on_pushButton_clicked();

private:
    bool flag;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
