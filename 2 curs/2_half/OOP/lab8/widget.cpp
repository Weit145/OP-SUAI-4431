#include "widget.h"
#include "ui_widget.h"

#include <QAbstractItemView>
#include <QFormLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTabWidget>
#include <QStandardItemModel>
#include <QVBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("ЛР8 — Агентство недвижимости");
    resize(1050, 720);

    auto *tabs = new QTabWidget(this);
    tabs->setObjectName("tabMainMenu");
    tabs->addTab(createClientTab(), "Клиенты");
    tabs->addTab(createRealtorTab(), "Риелторы");
    tabs->addTab(createPropertyTab(), "Объекты");
    tabs->addTab(createRequestTab(), "Заявки");
    tabs->addTab(createShowingTab(), "Показы");
    tabs->addTab(createDealTab(), "Сделки");
    tabs->addTab(createSearchTab(), "Подбор");

    auto *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(new QLabel("Информационная система: агентство недвижимости", this));
    mainLayout->addWidget(tabs);
    setLayout(mainLayout);

    connect(&facade, &AgencyFacade::dataChanged, this, &Widget::refreshUi);
    refreshUi();
}

Widget::~Widget()
{
    delete ui;
}

QWidget *Widget::createClientTab()
{
    auto *tab = new QWidget(this);
    auto *layout = new QVBoxLayout(tab);
    auto *form = new QFormLayout();

    clientNameEdit = new QLineEdit(tab);
    clientNameEdit->setObjectName("editClientFullName");
    clientPhoneEdit = new QLineEdit(tab);
    clientPhoneEdit->setObjectName("editClientPhone");
    clientBudgetSpin = new QDoubleSpinBox(tab);
    clientBudgetSpin->setObjectName("spinClientBudget");
    clientBudgetSpin->setRange(0, 1000000000);
    clientBudgetSpin->setDecimals(2);
    clientBudgetSpin->setSingleStep(100000);
    clientDistrictEdit = new QLineEdit(tab);
    clientDistrictEdit->setObjectName("editClientDistrict");

    auto *button = new QPushButton("Добавить клиента", tab);
    button->setObjectName("btnAddClient");
    connect(button, &QPushButton::clicked, this, &Widget::addClientFromForm);

    form->addRow("ФИО:", clientNameEdit);
    form->addRow("Телефон:", clientPhoneEdit);
    form->addRow("Бюджет:", clientBudgetSpin);
    form->addRow("Желаемый район:", clientDistrictEdit);
    form->addRow(button);

    clientsTable = new QTableView(tab);
    clientsTable->setObjectName("tableClients");

    layout->addLayout(form);
    layout->addWidget(clientsTable);
    return tab;
}

QWidget *Widget::createRealtorTab()
{
    auto *tab = new QWidget(this);
    auto *layout = new QVBoxLayout(tab);
    auto *form = new QFormLayout();

    realtorNameEdit = new QLineEdit(tab);
    realtorNameEdit->setObjectName("editRealtorFullName");
    realtorPhoneEdit = new QLineEdit(tab);
    realtorPhoneEdit->setObjectName("editRealtorPhone");
    realtorSpecializationEdit = new QLineEdit(tab);
    realtorSpecializationEdit->setObjectName("editRealtorSpecialization");
    realtorPercentSpin = new QDoubleSpinBox(tab);
    realtorPercentSpin->setObjectName("spinRealtorPercent");
    realtorPercentSpin->setRange(0, 20);
    realtorPercentSpin->setDecimals(2);
    realtorPercentSpin->setValue(3.0);

    auto *button = new QPushButton("Добавить риелтора", tab);
    button->setObjectName("btnAddRealtor");
    connect(button, &QPushButton::clicked, this, &Widget::addRealtorFromForm);

    form->addRow("ФИО:", realtorNameEdit);
    form->addRow("Телефон:", realtorPhoneEdit);
    form->addRow("Специализация:", realtorSpecializationEdit);
    form->addRow("Комиссия, %:", realtorPercentSpin);
    form->addRow(button);

    realtorsTable = new QTableView(tab);
    realtorsTable->setObjectName("tableRealtors");

    layout->addLayout(form);
    layout->addWidget(realtorsTable);
    return tab;
}

QWidget *Widget::createPropertyTab()
{
    auto *tab = new QWidget(this);
    auto *layout = new QVBoxLayout(tab);
    auto *form = new QFormLayout();

    propertyAddressEdit = new QLineEdit(tab);
    propertyAddressEdit->setObjectName("editPropertyAddress");
    propertyDistrictEdit = new QLineEdit(tab);
    propertyDistrictEdit->setObjectName("editPropertyDistrict");
    propertyTypeCombo = new QComboBox(tab);
    propertyTypeCombo->setObjectName("comboPropertyType");
    propertyTypeCombo->addItems({"Квартира", "Дом", "Коммерческая"});
    propertyRoomsSpin = new QSpinBox(tab);
    propertyRoomsSpin->setObjectName("spinPropertyRooms");
    propertyRoomsSpin->setRange(0, 30);
    propertyRoomsSpin->setValue(1);
    propertyAreaSpin = new QDoubleSpinBox(tab);
    propertyAreaSpin->setObjectName("spinPropertyArea");
    propertyAreaSpin->setRange(1, 10000);
    propertyAreaSpin->setDecimals(1);
    propertyAreaSpin->setValue(40);
    propertyPriceSpin = new QDoubleSpinBox(tab);
    propertyPriceSpin->setObjectName("spinPropertyPrice");
    propertyPriceSpin->setRange(0, 1000000000);
    propertyPriceSpin->setDecimals(2);
    propertyPriceSpin->setSingleStep(100000);
    propertyStatusCombo = new QComboBox(tab);
    propertyStatusCombo->setObjectName("comboPropertyStatus");
    propertyStatusCombo->addItems({"Свободен", "Забронирован", "Продан", "Сдан"});

    auto *button = new QPushButton("Добавить объект", tab);
    button->setObjectName("btnAddProperty");
    connect(button, &QPushButton::clicked, this, &Widget::addPropertyFromForm);

    form->addRow("Адрес:", propertyAddressEdit);
    form->addRow("Район:", propertyDistrictEdit);
    form->addRow("Тип:", propertyTypeCombo);
    form->addRow("Комнаты:", propertyRoomsSpin);
    form->addRow("Площадь:", propertyAreaSpin);
    form->addRow("Цена:", propertyPriceSpin);
    form->addRow("Статус:", propertyStatusCombo);
    form->addRow(button);

    propertiesTable = new QTableView(tab);
    propertiesTable->setObjectName("tableProperties");

    layout->addLayout(form);
    layout->addWidget(propertiesTable);
    return tab;
}

QWidget *Widget::createRequestTab()
{
    auto *tab = new QWidget(this);
    auto *layout = new QVBoxLayout(tab);
    auto *form = new QFormLayout();

    requestClientCombo = new QComboBox(tab);
    requestClientCombo->setObjectName("comboRequestClient");
    requestTypeCombo = new QComboBox(tab);
    requestTypeCombo->setObjectName("comboRequestType");
    requestTypeCombo->addItems({"Квартира", "Дом", "Коммерческая"});
    requestDistrictEdit = new QLineEdit(tab);
    requestDistrictEdit->setObjectName("editRequestDistrict");
    requestRoomsSpin = new QSpinBox(tab);
    requestRoomsSpin->setObjectName("spinRequestMinRooms");
    requestRoomsSpin->setRange(0, 30);
    requestRoomsSpin->setValue(1);
    requestMaxPriceSpin = new QDoubleSpinBox(tab);
    requestMaxPriceSpin->setObjectName("spinRequestMaxPrice");
    requestMaxPriceSpin->setRange(0, 1000000000);
    requestMaxPriceSpin->setDecimals(2);
    requestMaxPriceSpin->setSingleStep(100000);

    auto *button = new QPushButton("Добавить заявку", tab);
    button->setObjectName("btnAddRequest");
    connect(button, &QPushButton::clicked, this, &Widget::addRequestFromForm);

    form->addRow("Клиент:", requestClientCombo);
    form->addRow("Тип объекта:", requestTypeCombo);
    form->addRow("Район:", requestDistrictEdit);
    form->addRow("Мин. комнат:", requestRoomsSpin);
    form->addRow("Макс. цена:", requestMaxPriceSpin);
    form->addRow(button);

    requestsTable = new QTableView(tab);
    requestsTable->setObjectName("tableRequests");

    layout->addLayout(form);
    layout->addWidget(requestsTable);
    return tab;
}

QWidget *Widget::createShowingTab()
{
    auto *tab = new QWidget(this);
    auto *layout = new QVBoxLayout(tab);
    auto *form = new QFormLayout();

    showingClientCombo = new QComboBox(tab);
    showingClientCombo->setObjectName("comboShowingClient");
    showingPropertyCombo = new QComboBox(tab);
    showingPropertyCombo->setObjectName("comboShowingProperty");
    showingRealtorCombo = new QComboBox(tab);
    showingRealtorCombo->setObjectName("comboShowingRealtor");
    showingDateEdit = new QDateEdit(QDate::currentDate(), tab);
    showingDateEdit->setObjectName("dateShowing");
    showingDateEdit->setCalendarPopup(true);
    showingResultEdit = new QLineEdit(tab);
    showingResultEdit->setObjectName("editShowingResult");
    showingResultEdit->setText("Клиент думает");

    auto *button = new QPushButton("Добавить показ", tab);
    button->setObjectName("btnAddShowing");
    connect(button, &QPushButton::clicked, this, &Widget::addShowingFromForm);

    form->addRow("Клиент:", showingClientCombo);
    form->addRow("Объект:", showingPropertyCombo);
    form->addRow("Риелтор:", showingRealtorCombo);
    form->addRow("Дата:", showingDateEdit);
    form->addRow("Результат:", showingResultEdit);
    form->addRow(button);

    showingsTable = new QTableView(tab);
    showingsTable->setObjectName("tableShowings");

    layout->addLayout(form);
    layout->addWidget(showingsTable);
    return tab;
}

QWidget *Widget::createDealTab()
{
    auto *tab = new QWidget(this);
    auto *layout = new QVBoxLayout(tab);
    auto *form = new QFormLayout();

    dealClientCombo = new QComboBox(tab);
    dealClientCombo->setObjectName("comboDealClient");
    dealPropertyCombo = new QComboBox(tab);
    dealPropertyCombo->setObjectName("comboDealProperty");
    dealRealtorCombo = new QComboBox(tab);
    dealRealtorCombo->setObjectName("comboDealRealtor");
    dealDateEdit = new QDateEdit(QDate::currentDate(), tab);
    dealDateEdit->setObjectName("dateDeal");
    dealDateEdit->setCalendarPopup(true);
    dealOperationCombo = new QComboBox(tab);
    dealOperationCombo->setObjectName("comboDealOperation");
    dealOperationCombo->addItems({"Продажа", "Аренда"});
    dealPriceSpin = new QDoubleSpinBox(tab);
    dealPriceSpin->setObjectName("spinDealPrice");
    dealPriceSpin->setRange(0, 1000000000);
    dealPriceSpin->setDecimals(2);
    dealPriceSpin->setSingleStep(100000);

    auto *button = new QPushButton("Заключить сделку", tab);
    button->setObjectName("btnAddDeal");
    connect(button, &QPushButton::clicked, this, &Widget::addDealFromForm);

    form->addRow("Клиент:", dealClientCombo);
    form->addRow("Свободный объект:", dealPropertyCombo);
    form->addRow("Риелтор:", dealRealtorCombo);
    form->addRow("Дата:", dealDateEdit);
    form->addRow("Операция:", dealOperationCombo);
    form->addRow("Итоговая цена:", dealPriceSpin);
    form->addRow(button);

    dealsTable = new QTableView(tab);
    dealsTable->setObjectName("tableDeals");

    layout->addLayout(form);
    layout->addWidget(dealsTable);
    return tab;
}

QWidget *Widget::createSearchTab()
{
    auto *tab = new QWidget(this);
    auto *layout = new QVBoxLayout(tab);
    auto *form = new QFormLayout();

    searchRequestCombo = new QComboBox(tab);
    searchRequestCombo->setObjectName("comboSearchRequest");

    auto *button = new QPushButton("Подобрать объекты по заявке", tab);
    button->setObjectName("btnFindProperties");
    connect(button, &QPushButton::clicked, this, &Widget::showSuitableProperties);

    searchResultEdit = new QTextEdit(tab);
    searchResultEdit->setObjectName("textSearchResult");
    searchResultEdit->setReadOnly(true);

    form->addRow("Заявка:", searchRequestCombo);
    form->addRow(button);

    layout->addLayout(form);
    layout->addWidget(searchResultEdit);
    return tab;
}

void Widget::addClientFromForm()
{
    if (isEmpty(clientNameEdit) || isEmpty(clientPhoneEdit)) {
        QMessageBox::warning(this, "Ошибка", "Заполните ФИО и телефон клиента.");
        return;
    }

    facade.addClient(clientNameEdit->text(), clientPhoneEdit->text(),
                     clientBudgetSpin->value(), clientDistrictEdit->text());
    clientNameEdit->clear();
    clientPhoneEdit->clear();
    clientDistrictEdit->clear();
    clientBudgetSpin->setValue(0);
}

void Widget::addRealtorFromForm()
{
    if (isEmpty(realtorNameEdit) || isEmpty(realtorPhoneEdit)) {
        QMessageBox::warning(this, "Ошибка", "Заполните ФИО и телефон риелтора.");
        return;
    }

    facade.addRealtor(realtorNameEdit->text(), realtorPhoneEdit->text(),
                      realtorSpecializationEdit->text(), realtorPercentSpin->value());
    realtorNameEdit->clear();
    realtorPhoneEdit->clear();
    realtorSpecializationEdit->clear();
}

void Widget::addPropertyFromForm()
{
    if (isEmpty(propertyAddressEdit) || isEmpty(propertyDistrictEdit)) {
        QMessageBox::warning(this, "Ошибка", "Заполните адрес и район объекта.");
        return;
    }

    facade.addProperty(propertyAddressEdit->text(), propertyDistrictEdit->text(),
                       static_cast<Property::Type>(propertyTypeCombo->currentIndex()),
                       propertyRoomsSpin->value(), propertyAreaSpin->value(),
                       propertyPriceSpin->value(),
                       static_cast<Property::Status>(propertyStatusCombo->currentIndex()));
    propertyAddressEdit->clear();
    propertyDistrictEdit->clear();
}

void Widget::addRequestFromForm()
{
    if (requestClientCombo->count() == 0) {
        QMessageBox::warning(this, "Ошибка", "Сначала добавьте клиента.");
        return;
    }

    facade.addRequest(selectedId(requestClientCombo),
                      static_cast<Property::Type>(requestTypeCombo->currentIndex()),
                      requestDistrictEdit->text(), requestRoomsSpin->value(),
                      requestMaxPriceSpin->value());
}

void Widget::addShowingFromForm()
{
    if (showingClientCombo->count() == 0 || showingPropertyCombo->count() == 0 ||
        showingRealtorCombo->count() == 0) {
        QMessageBox::warning(this, "Ошибка", "Для показа нужен клиент, объект и риелтор.");
        return;
    }

    facade.addShowing(selectedId(showingClientCombo), selectedId(showingPropertyCombo),
                      selectedId(showingRealtorCombo), showingDateEdit->date(),
                      showingResultEdit->text());
}

void Widget::addDealFromForm()
{
    if (dealClientCombo->count() == 0 || dealPropertyCombo->count() == 0 ||
        dealRealtorCombo->count() == 0) {
        QMessageBox::warning(this, "Ошибка", "Для сделки нужен клиент, свободный объект и риелтор.");
        return;
    }

    if (dealPriceSpin->value() <= 0) {
        QMessageBox::warning(this, "Ошибка", "Введите итоговую цену сделки.");
        return;
    }

    facade.addDeal(selectedId(dealClientCombo), selectedId(dealPropertyCombo),
                   selectedId(dealRealtorCombo), dealDateEdit->date(),
                   static_cast<Deal::Operation>(dealOperationCombo->currentIndex()),
                   dealPriceSpin->value());
}

void Widget::showSuitableProperties()
{
    if (searchRequestCombo->count() == 0) {
        searchResultEdit->setText("Нет заявок для подбора.");
        return;
    }

    searchResultEdit->setText(facade.suitablePropertiesForRequest(selectedId(searchRequestCombo)).join('\n'));
}

void Widget::refreshUi()
{
    fillTable(clientsTable, {"ID", "ФИО", "Телефон", "Бюджет", "Район"}, facade.clientRows());
    fillTable(realtorsTable, {"ID", "ФИО", "Телефон", "Специализация", "% комиссии"}, facade.realtorRows());
    fillTable(propertiesTable, {"ID", "Адрес", "Район", "Тип", "Комнаты", "Площадь", "Цена", "Статус"}, facade.propertyRows());
    fillTable(requestsTable, {"ID", "Клиент", "Тип", "Район", "Мин. комнат", "Макс. цена", "Статус"}, facade.requestRows());
    fillTable(showingsTable, {"ID", "Клиент", "Объект", "Риелтор", "Дата", "Результат"}, facade.showingRows());
    fillTable(dealsTable, {"ID", "Клиент", "Объект", "Риелтор", "Дата", "Операция", "Цена", "Комиссия", "Статус"}, facade.dealRows());

    fillOptions(requestClientCombo, facade.clientOptions());
    fillOptions(showingClientCombo, facade.clientOptions());
    fillOptions(dealClientCombo, facade.clientOptions());

    fillOptions(showingRealtorCombo, facade.realtorOptions());
    fillOptions(dealRealtorCombo, facade.realtorOptions());

    fillOptions(showingPropertyCombo, facade.propertyOptions(false));
    fillOptions(dealPropertyCombo, facade.propertyOptions(true));

    fillOptions(searchRequestCombo, facade.requestOptions());
}

void Widget::fillTable(QTableView *table, const QStringList &headers,
                       const QVector<QStringList> &rows) const
{
    if (table == nullptr) {
        return;
    }

    auto *model = new QStandardItemModel(rows.size(), headers.size(), table);
    for (int column = 0; column < headers.size(); ++column) {
        model->setHeaderData(column, Qt::Horizontal, headers.at(column));
    }

    for (int row = 0; row < rows.size(); ++row) {
        const QStringList values = rows.at(row);
        for (int column = 0; column < headers.size(); ++column) {
            const QString value = column < values.size() ? values.at(column) : QString();
            auto *item = new QStandardItem(value);
            item->setEditable(false);
            model->setItem(row, column, item);
        }
    }

    QAbstractItemModel *oldModel = table->model();
    table->setModel(model);
    if (oldModel != nullptr) {
        oldModel->deleteLater();
    }

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setAlternatingRowColors(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void Widget::fillOptions(QComboBox *combo, const QVector<QPair<int, QString>> &options) const
{
    if (combo == nullptr) {
        return;
    }

    const int oldId = combo->currentData().toInt();
    combo->blockSignals(true);
    combo->clear();
    for (const auto &option : options) {
        combo->addItem(option.second, option.first);
    }

    for (int index = 0; index < combo->count(); ++index) {
        if (combo->itemData(index).toInt() == oldId) {
            combo->setCurrentIndex(index);
            break;
        }
    }
    combo->blockSignals(false);
}

int Widget::selectedId(QComboBox *combo) const
{
    return combo->currentData().toInt();
}

bool Widget::isEmpty(const QLineEdit *edit) const
{
    return edit == nullptr || edit->text().trimmed().isEmpty();
}
