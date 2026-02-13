#include "mainwindow.h"
#include <cmath>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    flag=0;

    connect(ui->lineEdit, &QLineEdit::textChanged,
            this, &MainWindow::repit);

    connect(ui->lineEdit_2, &QLineEdit::textChanged,
            this, &MainWindow::repit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

double MainWindow::get_labl1(bool &ok){
    return  ui->lineEdit->text().toDouble(&ok);
}

double MainWindow::get_labl2(bool &ok){
    return  ui->lineEdit_2->text().toDouble(&ok);
}

void MainWindow::out_labl3(double result_num){
    QString result;
    ui->lineEdit_3->setText(result.setNum(result_num));
}


double MainWindow::sum(double a, double b){
    return fmod(a,b);
}

double MainWindow::raz(double a, double b){
    return a-b;
}

double MainWindow::delen(double a, double b){
    if (b==0){
            return 0;
        }
    return a/b;
}

double MainWindow::umnosh(double a, double b){
    return a*b;
}

void MainWindow::on_pushButton_clicked()
{
    flag=1;
    metod = &MainWindow::sum;
    repit();
}

void MainWindow::on_pushButton_raz_clicked()
{
    flag=1;
    metod = &MainWindow::raz;
    repit();
}

void MainWindow::on_pushButton_delen_clicked()
{
    flag=1;
    metod = &MainWindow::delen;
    repit();
}

void MainWindow::on_pushButton_umnosh_clicked()
{
    flag=1;
    metod = &MainWindow::umnosh;
    repit();
}


void MainWindow::repit(){
    bool ok1,ok2;
    double fr = get_labl1(ok1);
    double sk = get_labl2(ok2);
    if (!ok1 || !ok2 || !flag)
    {
        ui->lineEdit_3->clear();
        return;
    }
    double result_num = (this->*metod)(fr,sk);
    out_labl3(result_num);
}
