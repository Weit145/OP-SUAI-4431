#include "montheighteen.h"

MonthEighteen::MonthEighteen(QObject *parent)
    : QObject{parent}
{}

double MonthEighteen::getCost(Estate *value){
    return 2.8;
}