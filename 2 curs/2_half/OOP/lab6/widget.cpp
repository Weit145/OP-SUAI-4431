#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget),
    info(this)
{
    ui->setupUi(this);
    ui->btnUndo->setEnabled(false);
    // регистрация слушателя
    connect(&info, SIGNAL(notifyObservers()), this, SLOT(update()));
    connect(ui->btnCalc, SIGNAL(pressed()), this, SLOT(btnCalcPressed()));
    connect(ui->btnUndo, SIGNAL(pressed()), this, SLOT(btnUndoPressed()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::update()
{
    auto value = info.getActualData();
    if(value != nullptr){
        fillForm(value);
    }
    // update btnUndo state
    ui->btnUndo->setEnabled(info.hasStates());
    // setting value to NULL
    value = nullptr;
}

void Widget::btnCalcPressed()
{
    if (ui->age->text().isEmpty() || ui->owner->text().isEmpty() || ui->residents->text().isEmpty() || ui->area->text().isEmpty()){
        return;
    }
    auto value = processForm();
    showCost(value);
    info.add(value);
    ui->btnUndo->setEnabled(true);
    // setting value to NULL
    value = nullptr;
}
void Widget::btnUndoPressed()
{
    info.undo();

}
// private
Estate *Widget::processForm()
{
    Estate* value= new Estate();

    value->age = ui->age->text().toInt();
    value->area = ui->area->text().toInt();
    value->residents = ui->residents->text().toInt();
    value->type = static_cast<Estate::EstateType>(ui->estateType->currentIndex());
    value->months = static_cast<Estate::EstateMonths>(ui->period->currentIndex());
    value->owner = ui->owner->text();
    return value;
}
void Widget::fillForm(Estate *value)
{
    ui->age->setText(QString::number(value->age));
    ui->owner->setText(value->owner);
    ui->area->setText(QString::number(value->area));
    ui->residents->setText(QString::number(value->residents));
    ui->estateType->setCurrentIndex(static_cast<int>(value->getType()));
    ui->period->setCurrentIndex(static_cast<int>(value->getTime()));
}
void Widget::showCost(Estate *value)
{
    ui->cost->setText(QString::number(calculationfacade::getCost(value)));
}