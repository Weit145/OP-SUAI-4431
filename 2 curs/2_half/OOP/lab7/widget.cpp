#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , info(this)
{
    ui->setupUi(this);
    ui->btnUndo->setEnabled(false);

    // Подписываемся на изменения истории и нажатия кнопок.
    connect(&info, &States::notifyObservers, this, &Widget::refreshForm);
    connect(ui->btnCalc, &QPushButton::pressed, this, &Widget::btnCalcPressed);
    connect(ui->btnUndo, &QPushButton::pressed, this, &Widget::btnUndoPressed);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::refreshForm()
{
    const Estate *value = info.getActualData();

    if (value != nullptr) {
        fillForm(value);
        showCost(value);
    }

    // Кнопка возврата активна только тогда, когда история не пуста.
    ui->btnUndo->setEnabled(info.hasStates());
}

void Widget::btnCalcPressed()
{
    // Если хотя бы одно из обязательных полей пустое, расчёт не выполняется.
    if (ui->age->text().isEmpty() || ui->owner->text().isEmpty() ||
        ui->residents->text().isEmpty() || ui->area->text().isEmpty()) {
        return;
    }

    Estate *value = processForm();
    showCost(value);
    info.add(value);
}

void Widget::btnUndoPressed()
{
    info.undo();
}

Estate *Widget::processForm() const
{
    Estate *value = new Estate();

    value->age = ui->age->text().toInt();
    value->area = ui->area->text().toInt();
    value->residents = ui->residents->text().toInt();
    value->type = static_cast<Estate::EstateType>(ui->estateType->currentIndex());
    value->months = static_cast<Estate::EstateMonths>(ui->period->currentIndex());
    value->owner = ui->owner->text();

    return value;
}

void Widget::fillForm(const Estate *value)
{
    ui->age->setText(QString::number(value->age));
    ui->owner->setText(value->owner);
    ui->area->setText(QString::number(value->area));
    ui->residents->setText(QString::number(value->residents));
    ui->estateType->setCurrentIndex(static_cast<int>(value->getType()));
    ui->period->setCurrentIndex(static_cast<int>(value->getTime()));
}

void Widget::showCost(const Estate *value)
{
    ui->cost->setText(QString::number(CalculationFacade::getCost(value)));
}
