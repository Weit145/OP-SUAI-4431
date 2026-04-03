#include "states.h"

States::States(QObject *parent)
    : QObject{parent}
{
    actualData = nullptr;
}
States::~States()
{
    // delete: actualData
    if(actualData){
        delete actualData;
        actualData = nullptr;
    }
    // delete and cleare: arra
    qDeleteAll(array);
    array.clear();
}

bool States::hasStates(){
    if (this->array.empty()){
        return false;
    }
    return true;
}

Estate *States::getActualData(){
    return actualData;
}

void States::add(Estate *value){
    this->array.append(value);
    actualData = value;
    emit notifyObservers();
}

void States::undo(){
    if (this->array.empty()){
        this->actualData=nullptr;
    }
    else{
        this->array.pop_back();
        this->actualData = array.isEmpty() ? nullptr : array.back();
    }
    emit notifyObservers();
}