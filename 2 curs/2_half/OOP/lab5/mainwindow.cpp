#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <stdexcept>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}


QVector<double> MainWindow::readNumbersFromFile(const QString &fileName)
{
    QVector<double> numbers;
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Не удалось открыть файл" << fileName;
        return numbers;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        for (const QString &part : parts) {
            bool ok;
            int value = part.toDouble(&ok);
            if (ok) {
                numbers.append(value);
            } else {
                qWarning() << "Не удалось преобразовать в число:" << part;
            }
        }
    }
    file.close();
    return numbers;
}

void MainWindow::saveChangeKolToFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Не удалось открыть файл для записи:" << fileName;
        return;
    }

    QTextStream out(&file);
    for (double value : chage_kol) {
        out << value << " ";
    }
    out << "\n";

    file.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clear_wind(){
    ui->error->clear();
    ui->find_elem->clear();
    ui->sum_lineEdit->clear();
}

void MainWindow::out_kol(QListWidget* kol, const QVector<double>& vector){
    kol->clear();
    for (double value : vector) {
        kol->addItem(QString::number(value));
    }
}

void MainWindow::on_get_kol_file_clicked()
{
    this->clear_wind();
    QString fileName = ui->comboBox->currentText();
    this->origin_kol = this->readNumbersFromFile(fileName);
    this->chage_kol = this->origin_kol;
    this->out_kol(ui->origin_kol,this->origin_kol);
    this->out_kol(ui->change_kol,this->chage_kol);
}


void MainWindow::on_clean_kol_clicked()
{
    this->clear_wind();
    ui->origin_kol->clear();
    ui->change_kol->clear();
    this->origin_kol.clear();
    this->chage_kol.clear();
}

int MainWindow::get_index(){
    QString result = ui->index_elem->text();
    if (result.isEmpty()){
         throw std::invalid_argument("Empty index");
    }
    bool ok;
    int value = result.toInt(&ok);
    if (!ok) {
        throw std::invalid_argument("Value is not a number");
    }
    return value;
}

double MainWindow::get_value(){
    QString result = ui->value_elem_lineEdit->text();
    if (result.isEmpty()){
        throw std::invalid_argument("Empty value");
    }
    bool ok;
    double value = result.toDouble(&ok);
    if (!ok) {
        throw std::invalid_argument("Value is not a number");
    }
    return value;
}


void MainWindow::set_sum(double sum){
    ui->sum_lineEdit->setText(QString::number(sum));
    return;
}

void MainWindow::del_index_vector(int index){
    if (index > this->chage_kol.length()-1 || index<0){
        throw std::length_error("Invalid index");
    }
    this->dop_kol.clear();
    for (int i=0;i<index;i++){
        this->dop_kol.append(this->chage_kol[i]);
    }
    for (int i=index+1;i<this->chage_kol.length();i++){
        this->dop_kol.append(this->chage_kol[i]);
    }
    this->chage_kol=this->dop_kol;
    this->dop_kol.clear();
}


void MainWindow::on_watch_elem_index_clicked()
{
    int result_int;
    double result_db;
    try{
        this->clear_wind();
        result_int = this->get_index();
        if (result_int >this->chage_kol.length() || result_int<0){
            throw std::length_error("Invalid index");
        }
        result_db = this->chage_kol[result_int];
    }catch(const std::exception &e){
        ui->error->setText("Error:" + QString(e.what()));
        return;
    }
    ui->find_elem->setText(QString::number(result_db));
}


void MainWindow::on_del_elem_index_clicked()
{
    int result_int;
    try{
        this->clear_wind();
        result_int = this->get_index();
        this->del_index_vector(result_int);
    }catch(const std::exception &e){
        this->dop_kol.clear();
        ui->error->setText("Error:" + QString(e.what()));
        return;
    }
    this->out_kol(ui->change_kol,this->chage_kol);
}


void MainWindow::on_del_elem_value_clicked()
{
    double result_db;
    int index=-1;
    try{
        this->clear_wind();
        result_db = this->get_value();
        for (int i=0;i<this->chage_kol.length();i++){
            if (result_db == this->chage_kol[i]){
                index = i;
                break;
            }
        }
        if (index == -1){
            throw std::invalid_argument("Not find value");
        }
        this->del_index_vector(index);
    }catch(const std::exception &e){
        this->dop_kol.clear();
        ui->error->setText("Error:" + QString(e.what()));
        return;
    }
    this->out_kol(ui->change_kol,this->chage_kol);
}


void MainWindow::on_add_elem_clicked()
{
    double result_db;
    try{
        this->clear_wind();
        result_db = this->get_value();
        this->chage_kol.append(result_db);
    }catch(const std::exception &e){
        ui->error->setText("Error:" + QString(e.what()));
        return;
    }
    this->out_kol(ui->change_kol,this->chage_kol);
}


void MainWindow::on_sum_elem_clicked()
{
    double sum = 0;
    try{
        this->clear_wind();
        if(this->chage_kol.isEmpty()){
            throw std::invalid_argument("Is Empty");
        }
        for (int i=0;i<this->chage_kol.length();i++){
            sum+=this->chage_kol[i];
        }
    }catch(const std::exception &e){
        ui->error->setText("Error:" + QString(e.what()));
        return;
    }
    ui->sum_lineEdit->setText(QString::number(sum));
}


void MainWindow::on_sum_even_elem_clicked()
{
    double sum = 0;
    try{
        this->clear_wind();
        if(this->chage_kol.isEmpty()){
            throw std::invalid_argument("Is Empty");
        }
        for (int i=0;i<this->chage_kol.length();i+=2){
            sum+=this->chage_kol[i];
        }
    }catch(const std::exception &e){
        ui->error->setText("Error:" + QString(e.what()));
        return;
    }
    ui->sum_lineEdit->setText(QString::number(sum));
}


void MainWindow::on_zad_1_clicked()
{
    try{
        this->clear_wind();
        this->dop_kol.clear();
        if (this->chage_kol.isEmpty()){
            throw std::invalid_argument("Is Empty");
        }
        for(int i=0; i<this->chage_kol.length();i++){
            if (this->chage_kol[i]>=0){
                 this->dop_kol.append(chage_kol[i]);
            }
        }
        for(int i=0; i<this->chage_kol.length();i++){
            if (this->chage_kol[i]<0){
                this->dop_kol.append(chage_kol[i]);
            }
        }
        this->chage_kol=this->dop_kol;
        this->dop_kol.clear();
    }catch(const std::exception &e){
        ui->error->setText("Error:" + QString(e.what()));
        return;
    }
    this->out_kol(ui->change_kol,this->chage_kol);
}


void MainWindow::on_zad_2_clicked()
{
    double sum=0;
    bool flag=1;
    try{
        this->clear_wind();
        this->dop_kol.clear();
        if (this->chage_kol.isEmpty()){
            throw std::invalid_argument("Is Empty");
        }
        for(int i=0; i<this->chage_kol.length();i++){
            if (this->chage_kol[i]>=0 && !flag){
                break;
            }
            else if (this->chage_kol[i]>=0 && flag){
                flag=!flag;
            }
            else if (!flag){
                sum+=this->chage_kol[i];
            }

        }
    }catch(const std::exception &e){
        ui->error->setText("Error:" + QString(e.what()));
        return;
    }
    ui->sum_lineEdit->setText(QString::number(sum));
}


void MainWindow::on_save_kol_file_clicked()
{
    this->clear_wind();
    QString fileName = ui->comboBox->currentText();
    this->saveChangeKolToFile(fileName);
}

