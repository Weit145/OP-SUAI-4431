#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pix("ice.png");
    ui->_photo->setPixmap(pix);

    ice_cream=1000;
    flag=false;

    price_chokolate=60;
    price_caremel=70;
    price_honey=80;
    price_peach=90;
    price_raspberry=100;
    price_strawberry=110;

    connect(ui->checkBox_chokolad, &QCheckBox::clicked,this,&MainWindow::repit);
    connect(ui->checkBox_caremel, &QCheckBox::clicked,this,&MainWindow::repit);
    connect(ui->checkBox_honey, &QCheckBox::clicked,this,&MainWindow::repit);
    connect(ui->checkBox_peach, &QCheckBox::clicked,this,&MainWindow::repit);
    connect(ui->checkBox_raspberry, &QCheckBox::clicked,this,&MainWindow::repit);
    connect(ui->checkBox_strawberry, &QCheckBox::clicked,this,&MainWindow::repit);

}

MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::get_ch(){
    return ui->checkBox_chokolad->isChecked();
}

bool MainWindow::get_cr(){
    return ui->checkBox_caremel->isChecked();
}

bool MainWindow::get_ho(){
    return ui->checkBox_honey->isChecked();
}

bool MainWindow::get_pe(){
    return ui->checkBox_peach->isChecked();
}

bool MainWindow::get_ra(){
    return ui->checkBox_raspberry->isChecked();
}

bool MainWindow::get_st(){
    return ui->checkBox_strawberry->isChecked();
}

void MainWindow::on_pushButton_clicked()
{
    flag =!flag;
    this->repit();
}


void MainWindow::repit(){
    if (!flag){
        ui->textBrowser_2->clear();
        return;
    }
    double_t result_chokolate = this->get_ch()*this->price_chokolate;
    double_t result_caremel = this->get_cr()*this->price_caremel;
    double_t result_honey = this->get_ho()*this->price_honey;
    double_t result_peach = this->get_pe()*this->price_peach;
    double_t result_raspberry = this->get_ra()*this->price_raspberry;
    double_t result_strawberry = this->get_st()*this->price_strawberry;

    double_t result_dop = result_chokolate+result_caremel+result_honey+result_peach+result_raspberry+result_strawberry;

    this->result = result_dop+ice_cream;


    QString ice = QString::number(ice_cream);
    QString dop = QString::number(result_dop);

    QString out = "Мороженное: " + ice + "\nДополнения: " + dop;

    if (result_chokolate&&result_caremel&&result_honey&&result_peach&&
        result_raspberry&&result_strawberry){
        this->result*=0.9;
        out=out +"\nСкидка: 10%";
    }
    out+= "\nИтог: " + QString::number(this->result);

    ui->textBrowser_2->setText(out);
}

