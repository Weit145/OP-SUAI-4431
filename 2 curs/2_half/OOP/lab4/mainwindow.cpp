#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->flag = 0;

    connect(ui->x9x12, &QRadioButton::clicked, this, &MainWindow::repit);
    connect(ui->x10x15, &QRadioButton::clicked, this, &MainWindow::repit);
    connect(ui->x20x30, &QRadioButton::clicked, this, &MainWindow::repit);
    connect(ui->glinec, &QRadioButton::clicked, this, &MainWindow::repit);
    connect(ui->mat, &QRadioButton::clicked, this, &MainWindow::repit);

    connect(ui->Kol, &QLineEdit::textChanged, this, &MainWindow::repit);
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    this->flag=!this->flag;
    this->repit();
}

double MainWindow::get_foto(){
    if (ui->x9x12->isChecked()) return this->x9x12;
    else if (ui->x10x15->isChecked()) return this->x10x15;
    else if (ui->x20x30->isChecked()) return this->x20x30;
    else return 0;
}

double MainWindow::get_mat(){
    if (ui->glinec->isChecked()) return this->glinec;
    else if (ui->mat->isChecked()) return this->mat;
    else return 0;
}

int MainWindow::get_kol(){
    return ui->Kol->text().toInt();
}

void MainWindow::set_result(QString result){
    if (result==""){
        ui->Result->clear();
    }
    else{
        ui->Result->setText(result);
    }
}


void MainWindow::repit(){
    double f =this->get_foto();
    double m = this->get_mat();
    int kol = this->get_kol();
    if (f and m and kol and this->flag){
        this->set_result(QString::number((f+m)*kol));
    }
    else{
        this->set_result("");
    }
}


