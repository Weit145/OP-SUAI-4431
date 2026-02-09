#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int64_t MainWindow::get_labl1(){
    return  ui->lineEdit->text().toInt();
}

int64_t MainWindow::get_labl2(){
    return  ui->lineEdit_2->text().toInt();
}

void MainWindow::out_labl3(int result_num){
    QString result;
    ui->lineEdit_3->setText(result.setNum(result_num));
}


int MainWindow::sum(int a, int b){
    return a+b;
}

int MainWindow::raz(int a, int b){
    return a-b;
}

int MainWindow::delen(int a, int b){
    return a/b;
}

int MainWindow::umnosh(int a, int b){
    return a*b;
}

void MainWindow::on_pushButton_clicked()
{
    int64_t fr = get_labl1();
    int64_t sk = get_labl2();
    int result_num = sum(fr,sk);
    out_labl3(result_num);
}



void MainWindow::on_pushButton_raz_clicked()
{
    int64_t fr = get_labl1();
    int64_t sk = get_labl2();
    int result_num = raz(fr,sk);
    out_labl3(result_num);
}


void MainWindow::on_pushButton_delen_clicked()
{
    int64_t fr = get_labl1();
    int64_t sk = get_labl2();
    int result_num = delen(fr,sk);
    out_labl3(result_num);
}


void MainWindow::on_pushButton_umnosh_clicked()
{
    int64_t fr = get_labl1();
    int64_t sk = get_labl2();
    int result_num = umnosh(fr,sk);
    out_labl3(result_num);
}

