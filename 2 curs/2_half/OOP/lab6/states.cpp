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
    return this->array.back();
}

void States::add(Estate *value){
    this->array.append(value);
}

void States::undo(){
    if (this->array.empty()){
        this->actualData=nullptr_t;
    }
    else{
        this->actualData=this->array.pop_back();
    }
}