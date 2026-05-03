#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QTableView>
#include <QTextEdit>
#include "agencyfacade.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

// Главная форма приложения. На форме расположены вкладки для всех сущностей:
// клиенты, риелторы, объекты недвижимости, заявки, показы и сделки.
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

private slots:
    void addClientFromForm();
    void addRealtorFromForm();
    void addPropertyFromForm();
    void addRequestFromForm();
    void addShowingFromForm();
    void addDealFromForm();
    void showSuitableProperties();
    void refreshUi();

private:
    QWidget *createClientTab();
    QWidget *createRealtorTab();
    QWidget *createPropertyTab();
    QWidget *createRequestTab();
    QWidget *createShowingTab();
    QWidget *createDealTab();
    QWidget *createSearchTab();

    void fillTable(QTableView *table, const QStringList &headers,
                   const QVector<QStringList> &rows) const;
    void fillOptions(QComboBox *combo, const QVector<QPair<int, QString>> &options) const;
    int selectedId(QComboBox *combo) const;
    bool isEmpty(const QLineEdit *edit) const;

    Ui::Widget *ui;
    AgencyFacade facade;

    QTableView *clientsTable = nullptr;
    QLineEdit *clientNameEdit = nullptr;
    QLineEdit *clientPhoneEdit = nullptr;
    QDoubleSpinBox *clientBudgetSpin = nullptr;
    QLineEdit *clientDistrictEdit = nullptr;

    QTableView *realtorsTable = nullptr;
    QLineEdit *realtorNameEdit = nullptr;
    QLineEdit *realtorPhoneEdit = nullptr;
    QLineEdit *realtorSpecializationEdit = nullptr;
    QDoubleSpinBox *realtorPercentSpin = nullptr;

    QTableView *propertiesTable = nullptr;
    QLineEdit *propertyAddressEdit = nullptr;
    QLineEdit *propertyDistrictEdit = nullptr;
    QComboBox *propertyTypeCombo = nullptr;
    QSpinBox *propertyRoomsSpin = nullptr;
    QDoubleSpinBox *propertyAreaSpin = nullptr;
    QDoubleSpinBox *propertyPriceSpin = nullptr;
    QComboBox *propertyStatusCombo = nullptr;

    QTableView *requestsTable = nullptr;
    QComboBox *requestClientCombo = nullptr;
    QComboBox *requestTypeCombo = nullptr;
    QLineEdit *requestDistrictEdit = nullptr;
    QSpinBox *requestRoomsSpin = nullptr;
    QDoubleSpinBox *requestMaxPriceSpin = nullptr;

    QTableView *showingsTable = nullptr;
    QComboBox *showingClientCombo = nullptr;
    QComboBox *showingPropertyCombo = nullptr;
    QComboBox *showingRealtorCombo = nullptr;
    QDateEdit *showingDateEdit = nullptr;
    QLineEdit *showingResultEdit = nullptr;

    QTableView *dealsTable = nullptr;
    QComboBox *dealClientCombo = nullptr;
    QComboBox *dealPropertyCombo = nullptr;
    QComboBox *dealRealtorCombo = nullptr;
    QDateEdit *dealDateEdit = nullptr;
    QComboBox *dealOperationCombo = nullptr;
    QDoubleSpinBox *dealPriceSpin = nullptr;

    QComboBox *searchRequestCombo = nullptr;
    QTextEdit *searchResultEdit = nullptr;
};
#endif // WIDGET_H
