#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

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
    QVector<double> readNumbersFromFile(const QString&);
    void out_kol(QListWidget*,const QVector<double>&);
    int get_index();
    double get_value();
    void set_sum(double);
    void del_index_vector(int);
    void clear_wind();
    void saveChangeKolToFile(const QString &fileName);

    void on_get_kol_file_clicked();

    void on_clean_kol_clicked();

    void on_watch_elem_index_clicked();

    void on_del_elem_index_clicked();

    void on_del_elem_value_clicked();

    void on_add_elem_clicked();

    void on_sum_elem_clicked();

    void on_sum_even_elem_clicked();

    void on_zad_1_clicked();

    void on_zad_2_clicked();

    void on_save_kol_file_clicked();

private:
    QVector<double> origin_kol;
    QVector<double> chage_kol;
    QVector<double> dop_kol;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
