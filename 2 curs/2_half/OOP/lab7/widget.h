#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "states.h"
#include "estate.h"
#include "calculationfacade.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

// Главный виджет приложения: считывает данные с формы,
// запускает расчёт и отображает результат.
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

public slots:
    // Обновляет форму после изменения истории состояний.
    void refreshForm();

private slots:
    // Обработчик нажатия кнопки расчёта.
    void btnCalcPressed();

    // Обработчик возврата к предыдущему запросу.
    void btnUndoPressed();

private:
    // Считывает данные из виджетов и формирует объект Estate.
    Estate *processForm() const;

    // Заполняет форму данными из выбранного состояния.
    void fillForm(const Estate *value);

    // Вычисляет и показывает страховой взнос.
    void showCost(const Estate *value);

private:
    Ui::Widget *ui;
    States info;
};

#endif // WIDGET_H
