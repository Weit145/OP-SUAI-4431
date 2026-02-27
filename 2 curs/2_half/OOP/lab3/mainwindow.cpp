#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->flag=1;

    this->repit();

    connect(ui->lineEdit,&QLineEdit::textChanged,this,&MainWindow::repit);
    connect(ui->lineEdit_2,&QLineEdit::textChanged,this,&MainWindow::repit);
    connect(ui->comboBox,&QComboBox::activated,this,&MainWindow::repit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

double MainWindow::get_l(){
    return ui->lineEdit->text().toDouble();
}

double MainWindow::get_s(){
    return ui->lineEdit_2->text().toDouble();
}

double MainWindow::get_t(){
    static const QMap<QString, double> priceMap = {
        {"Шёлк", 100.0},
        {"Крепдешин", 110.0},
        {"Шерсть", 120.0},
        {"Ситец", 130.0},
        {"Хлопок", 140.0}
    };
    QString target = ui->comboBox->currentText();
    return priceMap.value(target, 0.0); //
}

void MainWindow::set_t(QString price){
    if (price==""){
        ui->lineEdit_3->clear();
    }
    else{
     ui->lineEdit_3->setText(price);
    }
}

void MainWindow::set_result(QString result){
    if (result==""){
        ui->lineEdit_4->clear();
    }
    else{
        ui->lineEdit_4->setText(result);
    }
}

void MainWindow::on_pushButton_clicked()
{
    this->flag=!this->flag;
    this->repit();
}

void MainWindow::repit(){
    double l = this->get_l();
    double s = this->get_s();

    double price = this->get_t();
    this->set_t(QString::number(price));

    if (l==0 || s==0 || flag){
        this->set_result("");
        return;
    }

    double result = l*s*price;
    this->set_result(QString::number(result));
}

